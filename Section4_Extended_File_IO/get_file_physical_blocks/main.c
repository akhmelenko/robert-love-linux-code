
#define _GNU_SOURCE

#include <stdio.h>
// #include <sys/mman.h>
#include <sys/stat.h>
// #include <sys/epoll.h>
// #include <sys/uio.h>
#include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <linux/fs.h>
// #include <sys/stat.h>
// int fstat(int fd, // file descriptor
//           struct stat * sb); // stat struct
// returns file info
// If no error, returns != -1 .
// On error, returned -1 (and errno set).

int GetLogicalBlocksQty(int fd)
{
    struct stat buf;
    if (fstat(fd, &buf) == -1)
    {
        fprintf(stderr, "fstat() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    return buf.st_blocks; // index descriptor number
}

int GetPhyBlockNumber(int fd, int logicBlockNumber)
{
    if (ioctl(fd, FIBMAP, &logicBlockNumber) == -1)
    {
        fprintf(stderr, "ioctl() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    return logicBlockNumber;
}

int main()
{
    // open file
    int fd = open("test.txt", O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "open() error=%s\n", strerror(errno));
        return -1;
    }
    int logicBlockQty = GetLogicalBlocksQty(fd);
    printf("fd=%d has %d logical blocks\n", fd, logicBlockQty);

    // iterate all logical blocks
    for (int i = 0; i < logicBlockQty; i++)
    {
        printf("logic block = %d -> phy block = %d\n", i, GetPhyBlockNumber(fd, i));
    }

    // close file
    close(fd);

    return 0;
};
