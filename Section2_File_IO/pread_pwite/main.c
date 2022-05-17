#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

// #include <sys/types.h>
// #include <unistd.h>
// "ssize_t pread(int fd, void *buf, size_t count, off_t offset);"
// reads up to count bytes from file descriptor fd at offset
// offset (from the start of the file) into the buffer starting at
// buf.  The file offset is not changed.
//
// "ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);"
// writes up to count bytes from the buffer starting at buf
// to the file descriptor fd at offset offset.  The file offset is
// not changed.
//
// return:
// On success, pread() returns the number of bytes read (a return of
// zero indicates end of file) and pwrite() returns the number of
// bytes written.
// On error, -1 is returned and errno is set to indicate the error


// off_t == long int

int main()
{    
    printf("off_t size=%ld\n", sizeof(off_t));
    int fd = open("test.txt", O_RDWR);
    // write phrase to 50 position:
    char *bufW = "Write to extact position 50";
    int ret = pwrite(fd, bufW, strlen(bufW), (off_t)50);
    if (ret == -1)
        fprintf(stderr, "pwrite() error\n");
    
    // read data from exact position
    char bufR[32];
    ret = pread(fd, bufR, sizeof(bufR), (off_t)50);
    if (ret == -1)
        fprintf(stderr, "pwrite() error\n");

    write(1, bufR, ret); // write to stdout

    if (close(fd) == -1)
        fprintf(stderr, "close error=%s\n", strerror(errno));
    return 0;
};
