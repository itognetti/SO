#ifndef _LIB_H_
#define _LIB_H_

#include <stdio.h>

void error(char * message, int exitCode);

FILE * openFile(char * path, char * mode);

typedef struct md5Data{
    int pid;
    char md5[MD5_LENGTH + 1];
    char file[256];
    int isFinished;
} md5Data;

int isFile(char * path);

#endif