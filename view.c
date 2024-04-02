#include "./includes/view.h"

int main(int argc, char * argv[]){
    md5Data buffer;

    int finish = 0;

    while(!finish){
        printf("File: %d - MD5: %s - PID: %s", buffer.file, buffer.md5, buffer.pid);
        if(buffer.isFinished)
            finish = 1;
    }

    return 0;
}