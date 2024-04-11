#include "./includes/view.h"
#include "./includes/errors.h"

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

    openIPC(&sharedMem, &semRead, &semDone);

    sem_wait(semDone.address);
    setvbuf(stdout, NULL, _IONBF, 0); //Si no esta no se imprimen los hashes

    int finished = 0;
    int i = 0;
    while(!finished){
        sem_wait(semRead.address);
        if(pread(sharedMem.fd, &hashBuffer, sizeof(hashBuffer), i * sizeof(hashBuffer)) == -1){
            sem_post(semRead.address);
            exit("Problem reading from shared memory", SHMEM_ERROR);
        }
        i++;

        printf("File: %s - MD5: %s - PID: %d", hashBuffer.file, hashBuffer.md5, hashBuffer.pid);
        if(hashBuffer.isFinished)
            finished = 1;
    }

    sem_post(semDone.address);

    closeIPC(&sharedMem, &semRead, &semDone);

    return 0;
}