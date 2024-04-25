#include "./includes/app.h"

int main(int argc, char * argv[]){

    char * files[argc];
    int filesCount = 0;
    FILE * file = openFile("result.txt", "w");

    fd_set readFds, readFdsBackup;
    FD_ZERO(&readFds);

    if (argc <= 1){
        error("No files found", FILE_ERROR);
    }

    for(int i = 1; i < argc; i++){
        if (isFile(argv[i])){
            files[filesCount++] = argv[i];
        }
    }

    int slavesCount = filesCount / NUM_SLAVE_FILES + 1;

    slave slaves[slavesCount];

    for(int i = 0; i < slavesCount; i++){
        generatePipe(slaves[i].appToSlave);
        generatePipe(slaves[i].slaveToApp);
        FD_SET(slaves[i].slaveToApp[STDIN_FD], &readFds);
    }

    readFdsBackup = readFds;

    memData sharedMem;
    generateShMem(&sharedMem); 

    semData semDone, semRead;

    if (generateSem(&semRead, SEM_READ_NAME) == SEM_FAILED){
        unlinkShMem(&sharedMem);
        error("An error ocurred while generating the semaphores", SEMAPHORE_ERROR);
    }
    
    if (generateSem(&semDone, SEM_DONE_NAME) == SEM_FAILED){
        unlinkShMem(&sharedMem);
        unlinkSem(&semRead);
        error("An error ocurred while generating the semaphores", SEMAPHORE_ERROR);
    }

    sem_post(semDone.address);

    setvbuf(stdout, NULL, _IONBF, 0);

    printf("%s\n", MEM_NAME);

    sleep(2);

    int currentSlaveIndex = 0;
    int currentPid = 1;
    slave currentSlave;

    for( ; currentSlaveIndex < slavesCount && currentPid != 0; currentSlaveIndex++){
            currentPid = generateSlave();
            slaves[currentSlaveIndex].pid = currentPid;
    }

    if(currentPid == 0){
        for(int i = 0; i < slavesCount; i++){
            if(i != currentSlaveIndex - 1){
                closePipe(slaves[i].appToSlave[STDIN_FD]);
                closePipe(slaves[i].appToSlave[STDOUT_FD]);
                closePipe(slaves[i].slaveToApp[STDIN_FD]);
                closePipe(slaves[i].slaveToApp[STDOUT_FD]);
            }
        }

        currentSlave = slaves[currentSlaveIndex - 1];
        closePipe(currentSlave.appToSlave[STDOUT_FD]);
        closePipe(currentSlave.slaveToApp[STDIN_FD]);
        slaveProcess(currentSlave.appToSlave, currentSlave.slaveToApp);
    }
    else {
        for(int i = 0; i < slavesCount; i++){
            closePipe(slaves[i].appToSlave[STDIN_FD]);
            closePipe(slaves[i].slaveToApp[STDOUT_FD]);
        }

        int filesRead = 0;
        md5Data hashBuffer;
        char actualHash[MD5_LENGTH + 1] = {0};
        int currentFile = 0;

        memset(&hashBuffer, 0, sizeof(md5Data));
        
        for(int i = 0; currentFile < slavesCount; i++ ){
            write(slaves[i].appToSlave[STDOUT_FD], &(files[currentFile]), sizeof(char *)); 
            slaves[i].filename = files[currentFile++];
        }

        while(filesRead < filesCount){
            if(select(FD_SETSIZE, &readFds, NULL, NULL, NULL) == -1){
                error("An error ocurred while executing the select", SELECT_ERROR);
            }

            for(int i = 0; i < slavesCount && filesRead < filesCount; i++){
                if(FD_ISSET(slaves[i].slaveToApp[STDIN_FD], &readFds)){
                    if(read(slaves[i].slaveToApp[STDIN_FD], actualHash, MD5_LENGTH + 1) == -1){
                        error("An error ocurred while reading from the pipe", PIPE_ERROR);
                    }

                    hashBuffer.pid = slaves[i].pid;
                    strcpy(hashBuffer.md5, actualHash);
                    strcpy(hashBuffer.file, slaves[i].filename);

                    if(filesCount - filesRead <= 1){
                        hashBuffer.isFinished = 1;
                    }

                    fprintf(file, "File: %s - MD5: %s - PID: %d \n", hashBuffer.file, hashBuffer.md5, hashBuffer.pid);

                    writeToShMem(sharedMem.fd, &hashBuffer, sizeof(md5Data), filesRead);

                    sem_post(semRead.address);

                    filesRead++;

                    if(currentFile < filesCount){
                        write(slaves[i].appToSlave[1], &(files[currentFile]), sizeof(char *));
                        slaves[i].filename = files[currentFile];
                        currentFile++;
                    }
                }
            }
            readFds = readFdsBackup;
        }

        for(int i = 0; i < slavesCount; i++){   
            closePipe(slaves[i].appToSlave[STDOUT_FD]);                     
            closePipe(slaves[i].slaveToApp[STDIN_FD]);
            kill(slaves[i].pid, SIGKILL);
        }

        sem_wait(semDone.address);

        closeComms(&sharedMem, &semRead, &semDone);

        closeFile(file);

        unlinkShMem(&sharedMem);
        unlinkSem(&semRead);
        unlinkSem(&semDone);
    }

    return 0;
}