
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

// #include <sys/mman.h>
// void *mmap(void* addr, // recomendded addres for mapping (usually == 0)
//              size_t len, // size of mapped area in bytes
//              int prot, // access rights (PROT_NONE, PROT_READ, PROT_WRITE, PROT_EXEC)
//              int flags, // flags (MAP_FIXED - use addr strict,
// MAP_PRIVATE - copy of file to process map, MAP_SHARED - map shared with other processes)
//              int fd, // file descriptor
//              off_t offset); // offset of start data in file in bytes
// return:
// If no error, returns mapped address.
// On error, returned MAP_FAILED (and errno set).
//
// int munmap(void* addr, // unmapping start address
//              size_t len); // unmapping size in bytes
// return:
// If no error, returns 0.
// On error, returned -1 (and errno set).
//
// #include <unistd.h>
// long sysconf(int name);
// return:
// If no error, returns value of conf element.
// On error, returned -1 (and errno == EINVAL).
// Exception - no limits (== -1). Recommendation to clean "errno" before call "sysconf"
//
// #include <sys/stat.h>
// int fstat(int fd, // file descriptor
//           struct stat * sb); // stat struct
// returns file info
// If no error, returns != -1 .
// On error, returned -1 (and errno set).
//
// S_ISREG(); // check file is usual
// If usual, returns 1.
// If unusual, returned 0.
//
// #define _GNU_SOURCE // before includes
// void *mremap(void* addr, // mapped area addres ptr
//              size_t old_size, // size of old mapped area in bytes
//              size_t new_size, // size of new mapped area in bytes
//              unsigned long flags); // 0 or MREMAP_MAYMOVE
// return:
// If no error, returns remapped address.
// On error, returned MAP_FAILED (and errno set).
//
// int mprotect(const void * addr, // start addr of area
//              size_t len, // area len
//              int prot); // access rights (PROT_NONE, PROT_READ, PROT_WRITE, PROT_EXEC)
// return:
// If no error, returns 0.
// On error, returned -1 (and errno set).
//
// sync area with file on disk:
// int msync(const void * addr, // start addr of area
//              size_t len, // area len
//              int flags); // sync flags (MS_SYNC - wait sync in block, MS_ASYNC - not wait sync in block,
// MS_INVALIDATE - refresh other copyes of area)
// return:
// If no error, returns 0.
// On error, returned -1 (and errno set).
//
// int madvice(const void * addr, // start addr of area
//              size_t len, // area len (if 0 - use all memory range starts from "addr")
//              int advice); // how to use memory ()
// advice:
// MADV_NORMAL - usual mode
// MADV_RANDOM - advanced reading disabled in kernel
// MADV_SEQUENTIAL - aggressive advanced reading in kernel
// MADV_WILLNEED - advanced copy of pages to memory
// MADV_DONTNEED - kernel free all sources of this area and syncronise them
// MADV_DONTFORK - ! not copy memory area to child processes !
// MADV_DOFORK - cancel MADV_DONTFORK
// return:
// If no error, returns 0.
// On error, returned -1 (and errno set).

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
    // check file
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        fprintf(stderr, "fstat() error=%s\n", strerror(errno));
        return -1;
    }
    if (!S_ISREG(sb.st_mode))
    {
        fprintf(stderr, "fd=%d is not a file\n", fd);
        return -1;
    }
    // map file
    char *p = mmap(NULL,
                   pageSize,
                   PROT_READ | PROT_WRITE,
                   MAP_SHARED,
                   fd,
                   0);
    if (p == MAP_FAILED)
    {
        fprintf(stderr, "mmap() == MAP_FAILED with error=%d == %s\n", errno, strerror(errno));
        return -1;
    }
    printf("mmap p=%ld\n", (size_t)p);
    // close file
    close(fd);
    // advice to kernel
    if (madvise(p, pageSize, MADV_SEQUENTIAL) == -1)
    {
        fprintf(stderr, "madvice() error=%s\n", strerror(errno));
    }
    // write to mapped area
    for (int i = 0; i < 21; i++)
        p[i] = 'c' + i;
    // read from mapped area
    for (int i = 0; i < 21; i++)
        printf("%c", p[i]);
    // remap area (I do not know why max size == page size!!!)
    p = mremap(p, pageSize, pageSize, MREMAP_MAYMOVE);
    if (p == MAP_FAILED)
    {
        fprintf(stderr, "mremap() == MAP_FAILED with error=%s\n", strerror(errno));
        return -1;
    }
    printf("mremap p=%ld\n", (size_t)p);
    // int mapSize = look_up_mapping_size(p);
    // write to extended mapped area
    for (int i = 1; i < 25; i++)
        p[pageSize - i] = 'A' + i;
    // read from remapped area
    for (int i = 1; i < 25; i++)
        printf("%c", p[pageSize - i]);
    // change protect mode
    if (mprotect(p, pageSize, PROT_READ) == -1)
    {
        fprintf(stderr, "mproect() error=%s\n", strerror(errno));
        return -1;
    }
    printf("\ntry to read:\n");
    for (int i = 1; i < 25; i++)
        printf("%c", p[i]);
    // printf("\ntry to write:\n");
    // for (int i = 1; i < 25; i++)
    //     p[i] = 'g' + i;
    // sync mapped area with file on disk
    if (msync(p, pageSize, MS_SYNC) == -1)
    {
        fprintf(stderr, "msync() error=%s\n", strerror(errno));
        return -1;
    }
    // unmap mapped area
    if (munmap(p, pageSize) == -1)
    {
        fprintf(stderr, "munmap() error=%s\n", strerror(errno));
        return -1;
    }
    return 0;
};
