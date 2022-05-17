#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

// #include <unistd.h>
// int fsync(int fd); - block until file with metadata will be phyzicaly written to the disk
// int fdatasync(int fd); - block until file (without metadata) will be phyzicaly written to the disk
// void sync(void); - block until all files with metadata will be phyzicaly written to the disk (NOT RECOMENDED TO USE PROGRAMELY, ONLY AS UTILITE)


int main()
{    
    int fd = open("test.txt", O_WRONLY | O_TRUNC);
    if (fd == -1){
        fprintf(stderr, "Error open file!\n");
        return -1;
    }
    char *buf = "My data to write to the file!";
    size_t len = strlen(buf);
    size_t nw;
    printf("len=%ld\n",len);
    while (len > 0 && (nw = write(fd, buf, len)) != 0){
        if (nw == -1){            
            if (errno == EINTR) // system call was interrupted
                continue; // repeat write
            else{
                fprintf(stderr, "serious write() error\n");
                break;
            }
        }
            len -= nw;
            buf += nw;
    }
    printf("len=%ld, data written\n", len);

    // fsync:
    if (fsync(fd) == -1)
        fprintf(stderr, "fsync() error=%s\n", strerror(errno));
    // or
    // fdatasync:
    if (fdatasync(fd) == -1)
        fprintf(stderr, "fsync() error=%s\n", strerror(errno));
    // !!! no reasonable to use fsync and fdatasync together
    // sync(): - not recommended to use prigramely
    sync();

    close(fd);

    // O_SYNC at file oppening with syncronizatoin (O_DSYNC - the same byt without metadata)
    fd = open("test.txt", O_WRONLY | O_SYNC | O_APPEND);
    char *b = "Appended data with syncronization";
    size_t l = strlen(b);
    write(fd, b, l);
    close(fd);

    // add -GNU_SOURCE flag to Makefile compiler command
    // O_DIRECT - direct operation with disk (no cashing and buffering)
    
    fd = open("test.txt", O_WRONLY | O_DIRECT | O_APPEND);
    char *b2 = "Appended data with direct access";
    size_t l2 = strlen(b2);
    write(fd, b2, l2);
    close(fd);

    return 0;
};
