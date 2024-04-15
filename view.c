#include "./includes/view.h"

int main(int argc, char * argv[]){

    char sharedMemName[VIEW_BUFFER_SIZE];
    semData semRead, semDone;
    memData sharedMem;
    md5Data hashBuffer;

    if(argc >= 2){
        strncpy(sharedMemName, argv[1], VIEW_BUFFER_SIZE);
    }
    else {
        scanf("%s", sharedMemName);
    }

    sharedMem.name = sharedMemName;
    sharedMem.size = MEM_SIZE;
    semDone.name = SEM_DONE_NAME;
    semRead.name = SEM_READ_NAME;

    openComms(&sharedMem, &semRead, &semDone);

    sem_wait(semDone.address);
    
    setvbuf(stdout, NULL, _IONBF, 0);

    for(int i = 0, finished = 0; !finished; i++){
        sem_wait(semRead.address);
        readFromShMem(&sharedMem, &hashBuffer, sizeof(hashBuffer), i, &semDone);

        printf("File: %s - MD5: %s - PID: %d \n", hashBuffer.file, hashBuffer.md5, hashBuffer.pid);

        if(hashBuffer.isFinished){
            finished = 1;
        }
    }

    sem_post(semDone.address);

    closeComms(&sharedMem, &semRead, &semDone);

    return 0;
}