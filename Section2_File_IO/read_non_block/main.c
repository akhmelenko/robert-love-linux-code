#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

// #include <unistd.h>
// ssize_t read(int fd, void *buf, size_t count);


void ReadNonBlock(){
    int fd = open("test1.txt", O_NONBLOCK | O_RDONLY);
    if (fd == -1){
        fprintf(stderr, "Error open file!\n");
        return;
    }
    size_t nr;
    char buf[] = {[0 ... 32]1};
    int size = 3;

start_read:
    nr = read(fd, buf, size);
    if (nr == -1){
        if (errno == EINTR) // system call was interrupted
            goto start_read; // start to read again
        else if (errno == EAGAIN){ // data not ready for reading            
            // start timer to repeat reading (now start another job)
            int i = INT_MAX;
            while (i--);
            goto start_read;
        }
        else
            fprintf(stderr, "Error reading file!\n");
    }
    // check for partial write
    printf("nr=%ld, data: \n%s", nr, buf);
    if (nr < size){
        size -= nr;
        goto start_read;
    }
    close(fd);
}


int main()
{    
    ReadNonBlock();
    return 0;
};
