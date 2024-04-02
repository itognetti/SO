#ifndef _LIB_H_
#define _LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ipc.h"
#include "slave.h"
#include "errors.h"

void error(char * message, int exitCode);
FILE * openFile(char * path, char * mode);

#endif