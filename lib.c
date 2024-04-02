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