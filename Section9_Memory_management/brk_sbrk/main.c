
#define _GNU_SOURCE
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

// #include <unistd.h>
// int brk(void *addr);
// sets the end of the data segment to the value specified by addr
// Return: On success, brk() returns zero.  On error, -1 is returned, and errno is set to ENOMEM
// void *sbrk(intptr_t increment);
// increments the program's data space by increment bytes
// Return: On success, sbrk() returns the previous program break

int main()
{

    printf("Current dynamic memory break:%p\n", sbrk(0));

    char *buf = malloc(10);

    printf("Buf address=%p\n", buf);

    if (brk(buf))
    {
        fprintf(stderr, "brk() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Next dynamic memory break:%p\n", sbrk(0));

    char *buf2 = malloc(1);
    if (buf2 == NULL)
    {
        fprintf(stderr, "malloc() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("Buf2 address=%p\n", buf2);

    return 0;
}
// test