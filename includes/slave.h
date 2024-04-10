#ifndef _SLAVE_H_
#define _SLAVE_H_

#include "lib.h"

#define STDIN_FD 0
#define STDOUT_FD 1
#define NUM_SLAVE_FILES 2

typedef struct slave {
    int appToSlave[2];
    int slaveToApp[2];
    char *filename;
    int pid;
} slave;

int slaveProcess(int *appToSlave, int *slaveToApp);

#endif