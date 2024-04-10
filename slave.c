#include "./includes/slave.h"

int slaveProcess(int * appToSlave, int * slaveToApp){

    int subSlaveToSlave[2];
    char * filename;
    char * params[] = {MD5_PATH, NULL, NULL};
    char output[MD5_LENGTH + 1] = {0};

    while(1){
        read(appToSlave[STDIN_FD], &filename, sizeof(char *));
        generatePipe(subSlaveToSlave);

        if(generateSlave() == 0){
            closePipe(STDIN_FD);
            closePipe(subSlaveToSlave[STDIN_FD]);
            dupPipe(subSlaveToSlave[STDOUT_FD], 1);
            closePipe(subSlaveToSlave[STDOUT_FD]);

            params[1] = filename;
            execvp("md5sum", params);
        } 
        else {
            closePipe(subSlaveToSlave[STDOUT_FD]);
            dupPipe(subSlaveToSlave[STDIN_FD], 0);
            closePipe(subSlaveToSlave[STDIN_FD]);

            wait(NULL);
            read(STDIN_FD, output, MD5_LENGTH);
            write(slaveToApp[STDOUT_FD], output, MD5_LENGTH);        
        }
    }

    return 0;
}
