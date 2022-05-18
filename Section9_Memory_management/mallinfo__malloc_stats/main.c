
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

// #include <malloc.h>
// struct mallinfo mallinfo(void);
// Return a copy of a structure containing
// information about memory allocations performed by malloc(3) and related functions
//
// struct mallinfo2 {
//        size_t arena;     /* Non-mmapped space allocated (bytes) */
//        size_t ordblks;   /* Number of free chunks */
//        size_t smblks;    /* Number of free fastbin blocks */
//        size_t hblks;     /* Number of mmapped regions */
//        size_t hblkhd;    /* Space allocated in mmapped regions
//                             (bytes) */
//        size_t usmblks;   /* See below */
//        size_t fsmblks;   /* Space in freed fastbin blocks (bytes) */
//        size_t uordblks;  /* Total allocated space (bytes) */
//        size_t fordblks;  /* Total free space (bytes) */
//        size_t keepcost;  /* Top-most, releasable space (bytes) */
//    };
//
// malloc_stats();
// function prints (on standard error) statistics
// about memory allocated by malloc(3) and related functions

void Mallinfo_and_malloc_stats()
{
    struct mallinfo m = mallinfo();
    printf("Init mallinfo:\n");
    printf("malloc data segment size=%d\n", m.arena);
    printf("free fragments number=%d\n", m.ordblks);
    printf("fast buffers number=%d\n", m.smblks);
    printf("anonymous maps number=%d\n", m.hblks);
    printf("anonymous maps size=%d\n", m.hblkhd);
    printf("max available size for allocation=%d\n", m.usmblks);
    printf("fast buffers total size=%d\n", m.fsmblks);
    printf("total allocated size=%d\n", m.uordblks);
    printf("available fragments size=%d\n", m.fordblks);
    printf("size of memory available to trim =%d\n\n", m.keepcost);

    malloc_stats();

    // allocate memory
    char *buf = malloc(21);
    // buf = "dummy"; -- wrong!!!
    strcpy(buf, "dummy");
    if (buf)
    {
        ;
    }

    m = mallinfo();
    printf("\nSecond mallinfo:\n");
    printf("malloc data segment size=%d\n", m.arena);
    printf("free fragments number=%d\n", m.ordblks);
    printf("fast buffers number=%d\n", m.smblks);
    printf("anonymous maps number=%d\n", m.hblks);
    printf("anonymous maps size=%d\n", m.hblkhd);
    printf("max available size for allocation=%d\n", m.usmblks);
    printf("fast buffers total size=%d\n", m.fsmblks);
    printf("total allocated size=%d\n", m.uordblks);
    printf("available fragments size=%d\n", m.fordblks);
    printf("size of memory available to trim =%d\n\n", m.keepcost);

    malloc_stats();

    // allocate memory
    char *buf2 = malloc(21);
    // buf2 = "dummy"; -- wrong!!!
    strcpy(buf2, "dummy");
    if (buf2)
    {
        ;
    }

    m = mallinfo();
    printf("\nThird mallinfo:\n");
    printf("malloc data segment size=%d\n", m.arena);
    printf("free fragments number=%d\n", m.ordblks);
    printf("fast buffers number=%d\n", m.smblks);
    printf("anonymous maps number=%d\n", m.hblks);
    printf("anonymous maps size=%d\n", m.hblkhd);
    printf("max available size for allocation=%d\n", m.usmblks);
    printf("fast buffers total size=%d\n", m.fsmblks);
    printf("total allocated size=%d\n", m.uordblks);
    printf("available fragments size=%d\n", m.fordblks);
    printf("size of memory available to trim =%d\n\n", m.keepcost);

    malloc_stats();

    // free memory
    free(buf);
    free(buf2);

    m = mallinfo();
    printf("\nThird mallinfo:\n");
    printf("malloc data segment size=%d\n", m.arena);
    printf("free fragments number=%d\n", m.ordblks);
    printf("fast buffers number=%d\n", m.smblks);
    printf("anonymous maps number=%d\n", m.hblks);
    printf("anonymous maps size=%d\n", m.hblkhd);
    printf("max available size for allocation=%d\n", m.usmblks);
    printf("fast buffers total size=%d\n", m.fsmblks);
    printf("total allocated size=%d\n", m.uordblks);
    printf("available fragments size=%d\n", m.fordblks);
    printf("size of memory available to trim =%d\n\n", m.keepcost);

    malloc_stats();
}

int main()
{
    Mallinfo_and_malloc_stats();

    return 0;
}