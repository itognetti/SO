#include "./includes/view.h"

int main(int argc, char * argv[]){

    char semReadName[VIEW_BUFFER_SIZE], semDoneName[VIEW_BUFFER_SIZE], sharedMemName[VIEW_BUFFER_SIZE];
    semData semRead, semDone;
    memData sharedMem;
    md5Data hashBuffer;

    if(argc >= 4){
        strncpy(sharedMemName, argv[1], VIEW_BUFFER_SIZE);
        strncpy(semReadName, argv[2], VIEW_BUFFER_SIZE);
        strncpy(semDoneName, argv[3], VIEW_BUFFER_SIZE);
    }
    else {
        scanf("%s", sharedMemName);
        scanf("%s", semReadName);
        scanf("%s", semDoneName);
    }

    sharedMem.name = sharedMemName;
    sharedMem.size = MEM_SIZE;
    semDone.name = semDoneName;
    semRead.name = semReadName;

    openComms(&sharedMem, &semRead, &semDone);

    sem_wait(semDone.address);
    
    setvbuf(stdout, NULL, _IONBF, 0); //Si no esta no se imprimen los hashes

    for(int i = 0, finished = 0; !finished; i++){
        sem_wait(semRead.address);
        readFromShMem(&sharedMem, &hashBuffer, sizeof(hashBuffer), i, &semDone);

        printf("File: %s - MD5: %s - PID: %d", hashBuffer.file, hashBuffer.md5, hashBuffer.pid);

        if(hashBuffer.isFinished){
            finished = 1;
        }
    }

    sem_post(semDone.address);

    closeComms(&sharedMem, &semRead, &semDone);

    return 0;
}