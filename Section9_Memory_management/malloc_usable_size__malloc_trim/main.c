
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

// #include <malloc.h>
// size_t malloc_usable_size(void *ptr);
// returns the number of usable bytes in the
// block of allocated memory pointed to by ptr.  If ptr is NULL, 0
// is returned.
//
// int malloc_trim(size_t pad);
// function attempts to release free memory from
// the heap (by calling sbrk(2) or madvise(2) with suitable
// arguments)
// The pad argument specifies the amount of free space to leave
// untrimmed at the top of the heap
// Returns 1 if memory was actually
// released back to the system, or 0 if it was not possible to
// release any memory

int main()
{
    int need = 17;
    char *buf = malloc(need);
    if (buf == NULL)
    {
        fprintf(stderr, "malloc() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // check actual allocated space
    size_t allocated = malloc_usable_size(buf);
    printf("need=%d allocated=%ld\n", need, allocated);

    // trim extra space
    if (malloc_trim(24) == 0)
    {
        fprintf(stderr, "malloc_trim() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    char *buf2 = malloc(1024);
    if (buf2 == NULL)
    {
        fprintf(stderr, "malloc() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("trim threshold=%d\n", M_TRIM_THRESHOLD);

    return 0;
}