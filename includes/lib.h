#ifndef _LIB_H_
#define _LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "ipc.h"
#include "slave.h"
#include "errors.h"
#include <sys/stat.h>

#define MD5_LENGTH 32
#define MD5_PATH "/usr/bin/md5sum"

typedef struct md5Data{
    int pid;
    char md5[MD5_LENGTH + 1];
    char file[256];
    int isFinished;
} md5Data;

void error(char * message, int exitCode);

FILE * openFile(char * path, char * mode);
int isFile(char * path);

void generatePipe(int fd[2]);
void dupPipe(int oldfd, int newfd); 
void closePipe(int fd);

void generateShMem(memData * sharedMem);
void unlinkShMem(memData * sharedMem);

void * generateSem(semData * semaphore, char * semName);
void unlinkSem(semData * semaphore);

int generateSlave();

void openIPC(memData * sMem, semData * semRead, semData * semDone);
void closeIPC(memData * sMem, semData * semRead, semData * semDone);
void readFromSMem(memData * sMem, const void * buffer, size_t size, int offset, semData * semDone);

#endif