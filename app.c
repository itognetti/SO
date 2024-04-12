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
        generatePipe(slaves[i].slaveToApp);
        generatePipe(slaves[i].appToSlave);
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
    printf("%s\n", SEM_READ_NAME);
    printf("%s\n", SEM_DONE_NAME);

    sleep(4);

    int currentSlaveIndex = 0;
    int currentPid;
    slave currentSlave;

    for(currentSlaveIndex; currentSlaveIndex < slavesCount && currentPid != 0; currentSlaveIndex++){
            currentPid = generateSlave();
            slaves[currentSlaveIndex].pid = currentPid;
    }

    if(currentPid == 0){
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
        char ActualHash[MD5_LENGTH + 1]={0};
        int filesCountRead=0,filesCountSent=0;
        hashInfo buffer; //de tipo hashInfo
        memset(&buffer,0,sizeof(hasInfo));
        
        for(int i=0;FilesSent <slavesCount;i++ ){
            write(slaves[i].appToSlave[STDIN_FD], &(files[filesCountSent]), sizeof(char *)); 
            slaves[i].fileName = files[FilesSent++];
        }

        while(filesCountRead<filesCount){
            if(select(FD_SETSIZE ,&readFds,NULL,NULL,NULL)==-1){ //FD_SETSIZE
                perror("File Descpritor Error");
                exit(SELECT_ERROR);
            }
            for(int i=0;i<slavesCount && filesCountRead<filesCount;i++){
                if(FD_ISSET(slaves[i].slaveToMaster[STDIN], &readFds)){
                    if(read(slaves[i].slaveToMaster[STDIN], hash, MD5_LENGTH + 1) == -1){
                        error("Pipe Error", PIPE_ERROR);
                    }
                    buffer.pid = slaves[i].pid;
                    strcpy(buffer.hash, hash);
                    strcpy(buffer.file, slaves[i].name);

                }

            }

        }
        

        // To be continued...

    }

    return 0;
}