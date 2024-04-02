#include "./includes/app.h"

int main(int argc, char * argv[]){
    char * files[argc];
    int filesCount = 0;

    FILE * file = openFile("result.txt", "w");
    fd_set readFds, readFdsCopy;
    FD_ZERO(&readFds);

    if (argc <= 1){
        error()
    }

    return 0;
}