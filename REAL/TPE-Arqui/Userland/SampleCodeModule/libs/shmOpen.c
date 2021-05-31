#include <shmOpen.h>

void * shmOpen(int fd, int size) {
    void *ptr = NULL;
    smhOpenSyscall(fd,size,&ptr);
    return (void *) ptr;
}