
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

// #include <malloc.h>
// int mallopt(int param, int value);
// adjusts parameters that control the
// behavior of the memory-allocation functions
// Return: On success, returns 1.  On error, it returns 0.

int main()
{
    if (mallopt(M_MMAP_THRESHOLD, 64 * 1024) == 0)
    {
        fprintf(stderr, "mallopt() error!\n");
        exit(EXIT_FAILURE);
    }
    return 0;
}