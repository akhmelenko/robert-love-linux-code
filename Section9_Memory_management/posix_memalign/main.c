
#define _GNU_SOURCE
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// #include <malloc.h>
// void *memalign(size_t alignment, size_t size);
// allocates size bytes and returns
// a pointer to the allocated memory.  The memory address will be a
// multiple of alignment, which must be a power of two
// Return a pointer to the allocated memory on success.  On error, NULL is
// returned, and errno is set to indicate the error.

// #include <stdlib.h>
// int posix_memalign(void **memptr, size_t alignment, size_t size);
// allocates size bytes and places the
// address of the allocated memory in *memptr.  The address of the
// allocated memory will be a multiple of alignment, which must be a
// power of two and a multiple of sizeof(void *)
// Returns zero on success, or one of the error
// values listed in the next section on failure.  The value of errno
// is not set

int main()
{

    char *buf;
    int ret = posix_memalign((void **)&buf, 1048576, 1024);
    if (ret)
    {
        fprintf(stderr, "posix_memalign() error=%d (%s)\n", ret, strerror(ret));
        exit(EXIT_FAILURE);
    }
    buf = "allocated data";
    printf("buf addr=%p buf data=%s\n", buf, buf);

    char *buf2 = memalign(1024, 512);
    if (ret)
    {
        fprintf(stderr, "memalign() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    buf2 = "allocated data2";
    printf("buf2 addr=%p buf2 data=%s\n", buf2, buf2);
    return 0;
}