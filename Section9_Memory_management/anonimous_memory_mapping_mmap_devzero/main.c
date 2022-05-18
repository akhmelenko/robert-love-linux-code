
#include <stdio.h>
#include <sys/mman.h> // mmap() munmap()
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>  // open()
#include <unistd.h> // close()

// #include <sys/mman.h>
// void *mmap(void *addr, size_t length, int prot, int flags,
//             int fd, off_t offset);
// Return: On success, a pointer to the mapped area.  On
// error, the value MAP_FAILED (that is, (void *) -1) is returned,
// and errno is set to indicate the error
//
// int munmap(void *addr, size_t length);
// On success, munmap() returns 0.  On failure, it returns -1, and
//    errno is set to indicate the error (probably to EINVAL

int main()
{
    // ANONYMOUS MMAP
    char *buf = mmap(NULL,                        // recommended address of allocation
                     1024,                        // size
                     PROT_READ | PROT_WRITE,      // rights
                     MAP_ANONYMOUS | MAP_PRIVATE, // properties
                     -1,                          // fd - ignored
                     0);                          // shift - ignored
    if (buf == MAP_FAILED)
    {
        fprintf(stderr, "mmap() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    buf = "Anonymous memory";
    printf("anon mem: adr=%p data=%s\n", buf, buf);

    // if (munmap(buf, 1024) == -1)
    // {
    //     fprintf(stderr, "!munmap() error=%d (%s)\n", errno, strerror(errno));
    //     exit(EXIT_FAILURE);
    // }
    munmap(buf, 1024);

    // dev/zero
    int fd = open("dev/zero", O_RDWR);
    char *buf2 = mmap(NULL,                       // recommended address of allocation
                      1024,                       // size
                      PROT_READ | PROT_WRITE,     // rights
                      MAP_ANONYMOUS | MAP_SHARED, // properties
                      fd,                         // fd - ignored
                      0);
    if (buf2 == MAP_FAILED)
    {
        fprintf(stderr, "mmap(dev/zero) error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // now can close the dev/zero file
    close(fd);

    buf2 = "Anonymous memory (dev/zero)";
    printf("anon (dev/zero) mem: adr=%p data=%s\n", buf2, buf2);

    // if (munmap(buf2, 1024) == -1)
    // {
    //     fprintf(stderr, "munmap() error=%d (%s)\n", errno, strerror(errno));
    //     exit(EXIT_FAILURE);
    // }
    munmap(buf2, 1024);

    return 0;
}
