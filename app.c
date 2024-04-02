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

    

    return 0;
}