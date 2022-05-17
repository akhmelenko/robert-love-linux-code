
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // open()
#include <sys/ioctl.h>
#include <linux/cdrom.h>

// #include <sys/ioctl.h>
// int ioctl(int fd, unsigned long request, ...);
// Return:
// Usually, on success zero is returned.  A few ioctl() requests use
//    the return value as an output parameter and return a nonnegative
//    value on success.  On error, -1 is returned, and errno is set to
//    indicate the error.

int main()
{
    // open cd-rom device
    int fd = open("/dev/cdrom", O_RDONLY | O_NONBLOCK);
    if (fd == -1)
    {
        fprintf(stderr, "open() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // send cmd eject
    int ret = ioctl(fd, CDROMEJECT, 0);
    if (ret == -1)
    {
        fprintf(stderr, "ioctl() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // close
    if (close(fd) == -1)
    {
        fprintf(stderr, "close() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}