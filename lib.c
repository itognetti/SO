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
        error("An error ocurred while generating a pipe", PIPE_ERROR);
    }
}

void dupPipe(int oldFd, int newFd){
    if (dup2(oldFd, newFd) == -1){
        error("An error ocurred while generating a copy of a file descriptor", PIPE_ERROR);
    }
}

void closePipe(int fd){
    if(close(fd) == -1){
        error("An error ocurred while closing a pipe", PIPE_ERROR);
    }
}


void generateShMem(memData * sharedMem){
    sharedMem->name = MEM_NAME;
    sharedMem->size = MEM_SIZE;
    sharedMem->fd = shm_open(sharedMem->name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (sharedMem->fd == -1){
        error("An error ocurred while generating the shared memory", SHMEM_ERROR);
    }

    if(ftruncate(sharedMem->fd, sharedMem->size) == -1){
        error("An error ocurred while truncating the shared memory", SHMEM_ERROR);
    }

    sharedMem->address = mmap(NULL, sharedMem->size, PROT_READ | PROT_WRITE, MAP_SHARED, sharedMem->fd, 0);

    if(sharedMem->address == MAP_FAILED){
        error("An error ocurred while mapping the shared memory", SHMEM_ERROR);
    }
}

void unlinkShMem(memData * sharedMem){
    if(shm_unlink(sharedMem->name) == -1){
        error("An error ocurred while unlinking the shared memory", SHMEM_ERROR);
    }
}

void * generateSem(semData * semaphore, char * semName){
    semaphore->name = semName;
    return (semaphore->address = sem_open(semaphore->name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, 0));   
}

void unlinkSem(semData * semaphore){
    if(shm_unlink(semaphore->name) == -1){
        error("An error ocurred while unlinking the semaphores", SEMAPHORE_ERROR);
    }
}

int generateSlave(){
    int pid;
    if ((pid = fork()) == -1){
        error("An error ocurred while generating a slave", SLAVE_ERROR);
    }
    return pid;
}

void openIPC(memData * sMem, semData * semRead, semData * semDone){
    if((sMem->fd = shm_open(sMem->name, O_RDONLY, S_IRUSR)) == -1)
        error("An error ocurred while opening the shared memory", SHMEM_ERROR);
    if((sMem->address = mmap(NULL, sMem->size, PROT_READ, MAP_SHARED, sMem->fd, 0)) == MAP_FAILED)
        error("An error ocurred while mapping the shared memory", SHMEM_ERROR);
    if((semRead->address = sem_open(semRead->name, O_RDONLY, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
        error("An error ocurred while opening a semaphore", SEMAPHORE_ERROR);
    if((semDone->address = sem_open(semDone->name, O_RDONLY, S_IRUSR | S_IWUSR, 0)) == SEM_FAILED)
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

void readFromSMem(memData * sMem, const void * buffer, size_t size, int offset, semData * semDone){
    if(pread(sMem->fd, buffer, size, offset * size) == -1){
        sem_post(semDone->address);
        error("An error ocurred while reading from shared memory", SHMEM_ERROR);
    }
}