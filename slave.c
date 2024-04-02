#include "./includes/slave.h"

int slaveProcess(int * masterToSlave, int * slaveToMaster){
    
    int slaveToSlaveMaster[2];
    char *params[] = {"/usr/bin/md5sum", NULL, NULL};
    char * MyfileName;
    char output[MD5_LENGTH + 1] = {0};

    while(1){
        read(masterToSlave[STDIN_FD], &MyfileName, sizeof(char *));
        createPipe(slaveToSlaveMaster);
        if(createSlave() == 0){
            closePipe(STDIN_FD);
            closePipe(slaveToSlaveMaster[STDIN_FD]);
            dupPipe(slaveToSlaveMaster[STDOUT_FD], STDOUT_FD);
            closePipe(slaveToSlaveMaster[STDOUT_FD]);
            params[1] = MyfileName;
            execvp("md5sum", params);
        }
        else{
            closePipe(slaveToSlaveMaster[STDOUT_FD]);
            dupPipe(slaveToSlaveMaster[STDIN_FD], STDIN_FD);
            closePipe(slaveToSlaveMaster[STDIN_FD]);
            wait(NULL);
            read(STDIN_FD, output, MD5_LENGTH);
            write(slaveToMaster[STDOUT_FD], output, MD5_LENGTH);        }
    }
    return 0;
}
