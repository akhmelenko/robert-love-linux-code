
#include <stdio.h>
#include <sys/mman.h> // mlock() munlock()
#include <stdlib.h>   // exit()
#include <string.h>   // memset()
#include <errno.h>

// #include <sys/mman.h>
// int mlock(const void *addr, size_t len);
// int munlock(const void *addr, size_t len);
// lock part or all of the calling
// process's virtual address space into RAM, preventing that memory
// from being paged to the swap area
// Return: On success, these system calls return 0.  On error, -1 is
// returned, errno is set to indicate the error, and no changes are
// made to any locks in the address space of the process
//
// int mlockall(int flags);
// int munlockall(void);
// converse operation, unlocking part or all of the calling process's virtual address
// space, so that pages in the specified virtual address range may
// once more to be swapped out if required by the kernel memory manager
// Return: On success, these system calls return 0.  On error, -1 is
// returned, errno is set to indicate the error, and no changes are
// made to any locks in the address space of the process
//    MCL_CURRENT
//           Lock all pages which are currently mapped into the address
//           space of the process.

//    MCL_FUTURE
//           Lock all pages which will become mapped into the address
//           space of the process in the future.  These could be, for
//           instance, new pages required by a growing heap and stack
//           as well as new memory-mapped files or shared memory
//           regions.

// #include <sys/mman.h>
// int mincore(void *addr, size_t length, unsigned char *vec);
// vec - a vector that indicates whether pages of the
// calling process's virtual memory are resident in core (RAM), and
// so will not cause a disk access (page fault) if referenced.  The
// kernel returns residency information about the pages starting at
// the address addr, and continuing for length bytes
// On success, mincore() returns zero.  On error, -1 is returned, and errno is set to indicate the error

// block (from swapping) part of address space of process
void BlockPartMem()
{
    printf("\n--block part memory--\n");
    int size = 512;
    char buf[size];
    buf[size - 1] = 0;
    if (mlock(buf, size) == -1)
    {
        fprintf(stderr, "mlock() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    memset(buf, 's', size - 1);
    printf("buf=%s\n", buf);
    if (munlock(buf, size) == -1)
    {
        fprintf(stderr, "munlock() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

// block (from swapping) all address space of process (current or future)
void BlockAllMem()
{
    printf("\n--block part memory--\n");
    int size = 512;
    char buf[size];
    buf[size - 1] = 0;
    if (mlockall(MCL_CURRENT | MCL_FUTURE) == -1)
    {
        fprintf(stderr, "mlockall() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    memset(buf, 's', size - 1);
    printf("buf=%s\n", buf);
    if (munlockall() == -1)
    {
        fprintf(stderr, "munlockall() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void CheckPageInMem()
{
    printf("\n--check page in memory--\n");
    unsigned char vec[20];
    if (mincore(0, 512, vec) == -1)
    {
        fprintf(stderr, "mincore() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 20; i++)
    {
        printf("vec[%d]=%d\n", i, vec[i]);
    }
}

int main()
{
    BlockPartMem();
    // BlockAllMem();
    CheckPageInMem();
    exit(EXIT_SUCCESS);
}