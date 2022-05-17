
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

// #include <fcntl.h>
// int posix_fadvise(int fd, // file descriptor
//                      off_t offset, // offset from file start
//                      off_t len, // area len (if 0 - use all file memory starts from offset)
//                      int advise); // how to use file memory
// advice:
// POSIX_FADV_NORMAL - usual mode
// POSIX_FADV_RANDOM - advanced reading disabled in kernel
// POSIX_FADV_SEQUENTIAL - aggressive advanced reading in kernel
// POSIX_FADV_NOREUSE - app will use this data soon, but only once
// POSIX_FADV_WILLNEED - advanced copy of pages to memory
// POSIX_FADV_DONTNEED - kernel free all sources of this area and syncronise them
// return:
// If no error, returns 0.
// On error, returned -1 (and errno set).
//
// int readahead(int fd, // file descriptor
//                  off64_t offset, // offset from file start
//                  size_t count); //

int main()
{
    // get page size
    errno = 0;
    long pageSize = sysconf(_SC_PAGE_SIZE);
    printf("page size=%ld\n", pageSize);
    // open file
    int fd = open("test.txt", O_RDWR);
    if (fd == -1)
    {
        fprintf(stderr, "open() error=%s\n", strerror(errno));
        return -1;
    }

    // get end of file
    int size = lseek(fd, 0L, SEEK_END);
    printf("file size=%d\n", size);
    if (size == -1)
    {
        fprintf(stderr, "lseek() error=%s\n", strerror(errno));
        return -1;
    }

    // advise 1
    if (posix_fadvise(fd, 0, 0, POSIX_FADV_RANDOM) == -1)
    {
        fprintf(stderr, "posix_fadvise() error=%s\n", strerror(errno));
        return -1;
    }

    // write to file
    char *buf = "NEW END";
    int nw = write(fd, buf, strlen(buf));
    if (nw == -1)
    {
        fprintf(stderr, "write() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    printf("written %d bytes\n", nw);

    // advise 2
    if (readahead(fd, 0, size) == -1)
    {
        fprintf(stderr, "readahead() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }

    // close file
    close(fd);

    return 0;
};
