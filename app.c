#include "./includes/app.h"

int main(int argc, char * argv[]){
    char * files[argc];
    int filesCount = 0;

    FILE * file = openFile("result.txt", "w");
    fd_set readFds, readFdsCopy;
    FD_ZERO(&readFds);

    if (argc <= 1){
        error("No files found", FILE_ERROR);
    }

    for(int i = 1; i < argc; i++){
        if (isFile(argv[i])){
            files[filesCount++] = argv[i];
        }
    }

    int slavesCount = filesCount / NUM_SLAVE_FILES + 1;

    slave slaves[slavesCount];

    for(int i = 0; i < slavesCount; i++){
        createPipe(slaves[i].appToSlave);
        createPipe(slaves[i].slaveToApp);
        FD_SET(slaves[i].slaveToApp[STDIN_FD], &readFds);
    }

    readFdsCopy = readFds;

    // Shared memory
    memData sharedMem;
    createShMem(&sharedMem); 

    

    return 0;
}