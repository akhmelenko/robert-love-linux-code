
#define _GNU_SOURCE

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
// #include <sys/epoll.h>
// #include <sys/uio.h>
#include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

// #include <sys/stat.h>
// int fstat(int fd, // file descriptor
//           struct stat * sb); // stat struct
// returns file info
// If no error, returns != -1 .
// On error, returned -1 (and errno set).

int GetInode(int fd)
{
    struct stat buf;
    if (fstat(fd, &buf) == -1)
    {
        fprintf(stderr, "fstat() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    return buf.st_ino; // index descriptor number
}

int main()
{
    // open file
    int fd = open("test.txt", O_RDWR);
    if (fd == -1)
    {
        fprintf(stderr, "open() error=%s\n", strerror(errno));
        return -1;
    }
    int id = GetInode(fd);
    printf("fd=%d has index desctiptor=%d\n", fd, id);

    // close file
    close(fd);

    return 0;
};
