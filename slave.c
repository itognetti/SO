#include "./includes/slave.h"

int slaveProcess(int * appToSlave, int * slaveToApp){
    
    int SlaveCommunMaster[2];
    char * params[] = {"/usr/bin/md5sum", NULL, NULL};
    char * filename;
    char output[MD5_LENGTH + 1] = {0};

    while(1){
        read(appToSlave[STDIN_FD], &filename, sizeof(char *));
        createPipe(SlaveCommunMaster);
        if(createSlave() == 0){
            closePipe(STDIN_FD);
            closePipe(SlaveCommunMaster[STDIN_FD]);
            dupPipe(SlaveCommunMaster[STDOUT_FD], STDOUT_FD);
            closePipe(SlaveCommunMaster[STDOUT_FD]);
            params[1] = filename;
            execvp("md5sum", params);
        }
        else{
            closePipe(SlaveCommunMaster[STDOUT_FD]);
            dupPipe(SlaveCommunMaster[STDIN_FD], STDIN_FD);
            closePipe(SlaveCommunMaster[STDIN_FD]);
            wait(NULL);
            read(STDIN_FD, output, MD5_LENGTH);
            write(slaveToApp[STDOUT_FD], output, MD5_LENGTH);        
            }
    }
    return 0;
}
