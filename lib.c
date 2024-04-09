#include "./includes/lib.h"

void error(char * message, int exitCode){
    perror(message);
    exit(exitCode);
}

FILE * openFile(char * path, char * mode){
    FILE * file = fopen(path, mode);
    if (file == NULL){
        error("An error ocurred while opening a file", FILE_ERROR);
    }
    return file;
}

int isFile(char * path){
    struct stat buffer;
    stat(path, &buffer);
    return S_ISREG(buffer.st_mode);
}

void generatePipe(int fd[2]){
    if(pipe(fd) == -1){
        error("An error ocurred while creating a pipe", PIPE_ERROR);
    }
}

void createShMem(memData * sharedMem){
    sharedMem->name = MEM_NAME;
    sharedMem->size = MEM_SIZE;
    int memFd = shm_open(sharedMem->name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (memFd == -1){
        error("An error ocurred while creating the shared memory", SHMEM_ERROR);
    }

    if(ftruncate(memFd, sharedMem->size) == -1){
        error("An error ocurred while truncating the shared memory", SHMEM_ERROR);
    }

    sharedMem->fd = memFd;
    sharedMem->address = mmap(NULL, sharedMem->size, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0);

    if(sharedMem->address == MAP_FAILED){
        error("An error ocurred while mapping the shared memory", SHMEM_ERROR);
    }
}

void openIPC(memData * sMem, semData * semRead, semData * semDone){
    if((sMem->fd = shm_open(sMem->name, O_RDONLY, S_IRUSR)) == -1)
        error("An error ocurred while opening the shared memory", SHMEM_ERROR);
    if((sMem->address = mmap(NULL, sMem->size, PROT_READ, MAP_SHARED, sMem->fd, 0)) == MAP_FAILED)
        error("An error ocurred while mapping the shared memory", SHMEM_ERROR);
    if((semRead->sem = sem_open(semRead->name, O_RDONLY, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
        error("An error ocurred while opening a semaphore", SEMAPHORE_ERROR);
    if((semDone->sem = sem_open(semDone->name, O_RDONLY, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
        error("An error ocurred while opening a semaphore", SEMAPHORE_ERROR);
}

void closeIPC(memData * sMem, semData * semRead, semData * semDone){
    if(munmap(sMem->address, sMem->size) == -1)
        error("An error ocurred while unmapping the shared memory", SHMEM_ERROR);
    if(close(sMem->fd) == -1)
        error("An error ocurred while closing a pipe", PIPE_ERROR);
    if(sem_close(semRead->name) == -1)
        error("An error ocurred while closing a semaphore", SEMAPHORE_ERROR);
    if(sem_close(semDone->name) == -1)
        error("An error ocurred while closing a semaphore", SEMAPHORE_ERROR);
}

int generateSlave(){
    int pid;
    if ((pid = fork()) == -1){
        error("An error ocurred while creating a slave", SLAVE_ERROR);
    }
}

void dupPipe(int oldFd, int newFd){
    if (dup2(oldFd, newFd) == -1){
        error("An error ocurred while creating a copy of a file descriptor", PIPE_ERROR);
    }
}

