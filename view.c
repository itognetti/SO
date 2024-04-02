#include "./includes/view.h"

int main(int argc, char * argv[]){
    char sMemName[VIEW_BUFFER_SIZE], semReadName[VIEW_BUFFER_SIZE], semDoneName[VIEW_BUFFER_SIZE];
    memData sMem;
    semData semRead, semDone;
    md5Data buffer;

    if(argc >= 4){
        strncpy(sMemName, argv[1], VIEW_BUFFER_SIZE);
        strncpy(semReadName, argv[2], VIEW_BUFFER_SIZE);
        strncpy(semDoneName, argv[3], VIEW_BUFFER_SIZE);
    }else{
        scanf("%s", sMemName);
        scanf("%s", semReadName);
        scanf("%s", semDoneName);
    }

    sMem.name = sMemName;
    sMem.size = MEM_SIZE;
    semRead.name = semReadName;
    semDone.name = semDoneName;

    openIPC(&sMem, &semRead, &semDone);

    int finish = 0;

    while(!finish){
        printf("File: %s - MD5: %s - PID: %d", buffer.file, buffer.md5, buffer.pid);
        if(buffer.isFinished)
            finish = 1;
    }

    closeIPC(&sMem, &semRead, &semDone);

    return 0;
}