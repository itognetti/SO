#include "./includes/slave.h"

int slaveProcess(int * appToSlave, int * slaveToApp){
    
    int slaveToSlaveMaster[2];
    char * params[] = {"/usr/bin/md5sum", NULL, NULL};
    char * filename;
    char output[MD5_LENGTH + 1] = {0};

    while(1){
        read(appToSlave[STDIN_FD], &filename, sizeof(char *));
        createPipe(slaveToSlaveMaster);
        if(createSlave() == 0){
            closePipe(STDIN_FD);
            closePipe(slaveToSlaveMaster[STDIN_FD]);
            dupPipe(slaveToSlaveMaster[STDOUT_FD], STDOUT_FD);
            closePipe(slaveToSlaveMaster[STDOUT_FD]);
            params[1] = filename;
            execvp("md5sum", params);
        }
        else{
            closePipe(slaveToSlaveMaster[STDOUT_FD]);
            dupPipe(slaveToSlaveMaster[STDIN_FD], STDIN_FD);
            closePipe(slaveToSlaveMaster[STDIN_FD]);
            wait(NULL);
            read(STDIN_FD, output, MD5_LENGTH);
            write(slaveToApp[STDOUT_FD], output, MD5_LENGTH);        
            }
    }
    return 0;
}
