#include "./includes/slave.h"

int slaveProcess(int * appToSlave, int * slaveToApp){

    int pipeSlaveToApp[2];
    char * filename;
    char * params[] = {MD5_PATH, NULL, NULL};
    char output[MD5_LENGTH + 1] = {0};

    while(1){
        read(appToSlave[STDIN_FD], &filename, sizeof(char *));
        generatePipe(pipeSlaveToApp);

        if(generateSlave() == 0){
            closePipe(STDIN_FD);
            closePipe(pipeSlaveToApp[STDIN_FD]);
            dupPipe(pipeSlaveToApp[STDOUT_FD], STDOUT_FD);
            closePipe(pipeSlaveToApp[STDOUT_FD]);

            params[1] = filename;
            execvp("md5sum", params);
        } 
        else {
            closePipe(pipeSlaveToApp[STDOUT_FD]);
            dupPipe(pipeSlaveToApp[STDIN_FD], STDIN_FD);
            closePipe(pipeSlaveToApp[STDIN_FD]);

            wait(NULL);
            read(STDIN_FD, output, MD5_LENGTH);
            write(slaveToApp[STDOUT_FD], output, MD5_LENGTH);        
        }
    }

    return 0;
}
