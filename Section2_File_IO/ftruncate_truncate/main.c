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
// "int truncate(const char *path, off_t length);"
// "int ftruncate(int fd, off_t length);"
// functions cause the regular file named by path or referenced by fd to be truncated to a size of precisely length bytes.
// ftruncate(), the file must be open for writing; 
// truncate(), the file must be writable. 
// length - new size of file
//
// return: On success, zero is returned. On error, -1 is returned, and errno is set appropriately.

int main()
{        
    // reduce file size
    // ftruncate:
    int fd = open("test.txt", O_RDWR);
    if (fd == -1)
        fprintf(stderr, "open() error=%s\n", strerror(errno));
    int ret = ftruncate(fd, 15);
    if (ret == -1)
        fprintf(stderr, "ftruncate() error=%s\n", strerror(errno));
    if (close(fd) == -1)
        fprintf(stderr, "close error=%s\n", strerror(errno));

    // truncate:
    ret = truncate("./test.txt", 10);
    if (ret == -1)
        fprintf(stderr, "truncate() error=%s\n", strerror(errno));



    // increase file size
    // ftruncate:
    fd = open("test.txt", O_RDWR);
    if (fd == -1)
        fprintf(stderr, "open() error=%s\n", strerror(errno));
    ret = ftruncate(fd, 25);
    if (ret == -1)
        fprintf(stderr, "ftruncate() error=%s\n", strerror(errno));
    if (close(fd) == -1)
        fprintf(stderr, "close error=%s\n", strerror(errno));

    // truncate:
    ret = truncate("./test.txt", 30);
    if (ret == -1)
        fprintf(stderr, "truncate() error=%s\n", strerror(errno));
    
    return 0;
};
