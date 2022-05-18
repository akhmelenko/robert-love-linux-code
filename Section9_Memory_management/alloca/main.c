
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <alloca.h>
#include <fcntl.h>  // open()
#include <unistd.h> // close()

// #include <alloca.h>
// void *alloca(size_t size);
// allocates size bytes of space in the stack
// frame of the caller.  This temporary space is automatically freed
// when the function that called alloca() returns to its caller
// Returns a pointer to the beginning of the allocated space

int open_sysconf(const char *filename, int flags)
{
    const char *etc = "/etc/";
    char *path = alloca(strlen(etc) + strlen(filename) + 1);
    strcpy(path, etc);
    strcat(path, filename);
    return open(path, flags);
}

int main()
{
    int fd = open_sysconf("at.deny", O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "open_sysconf() error\n");
        exit(EXIT_FAILURE);
    }

    char buf[20];
    read(fd, buf, 20);
    printf("buf=%s\n", buf);

    close(fd);

    return 0;
}