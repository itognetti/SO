#ifndef _IPC_H_
#define _IPC_H_

#include <semaphore.h>
#include <sys/mman.h>

#define MEM_SIZE 16384
#define MEM_NAME "md5_mem"
#define SEM_READ_NAME "md5_sem_read"
#define SEM_DONE_NAME "md5_sem_done"

typedef struct memData{
    char * name;
    void * address;
    size_t size;
    int fd;
} memData;

typedef struct semData{
    char * name;
    sem_t * sem;
} semData;

#endif
