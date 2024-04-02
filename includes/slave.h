#ifndef _SLAVE_H_
#define _SLAVE_H_

#include "lib.h"

#define MD5_LENGTH 32
#define STDIN_FD 0
#define STDOUT_FD 1
#define NUM_SLAVE_FILES 2

typedef struct slave {
    int master_to_slave_pipe[2];
    int slave_to_master_pipe[2];
    char *filename;
    int process_id;
} slave;

int slaveProcess(int *app_to_slave_pipe, int *slave_to_app_pipe);

#endif
