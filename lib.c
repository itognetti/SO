#include "./includes/lib.h"

void error(char * message, int exitCode){
    perror(message);
    exit(exitCode);
}

FILE * openFile(char * path, char * mode){
    FILE * file = fopen(path, mode);
    if (file == NULL){
        error("Problem opening file", FILE_ERROR);
    }
    return file;
}

int isFile(char * path){
    struct stat buffer;
    stat(path, &buffer);
    return S_ISREG(buffer.st_mode);
}

void createPipe(int fd[2]){
    if(pipe(fd) == -1){
        error("Problem creating pipe", PIPE_ERROR);
    }
}

void createShMem(memData * sharedMem){
    sharedMem->name = MEM_NAME;
    sharedMem->size = MEM_SIZE;
    int memFd = shm_open(sharedMem->name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

    if (memFd == -1){
        error("Problem creating shared memory", SHMEM_ERROR);
    }

    if(ftruncate(memFd, sharedMem->size) == -1){
        error("Problem truncating shared memory", SHMEM_ERROR);
    }

    sharedMem->fd = memFd;
    sharedMem->address = mmap(NULL, sharedMem->size, PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0);

    if(sharedMem->address == MAP_FAILED){
        error("Problem mapping shared memory", SHMEM_ERROR);
    }
}