#include <stdio.h>
#include <sys/uio.h>
#include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#include <fcntl.h>
// #include <unistd.h>
#include <errno.h>
#include <string.h>

// #include <sys/uio.h>
// struct iovec {
//     void *iov_base; // ptr to buf start
//     size_t iov_let; // buf size in bytes
// }
//
// "ssize_t readv(int fd, const struct iovec *iov, int count);"
// count - number of buffers to read
// return:
// If no error, returns number of bytes.
// On error, not 0 is returned -1 (and errno set).
//
// "ssize_t writev(int fd, const struct iovec *iov);"
// count - number of buffers to write
// return:
// If no error, returns number of bytes.
// On error, not 0 is returned -1 (and errno set).

void FragmentedWrite()
{
    char *buf[] = {"Line One\n", "Line Two\n", "Line Three\n"};
    struct iovec iov[3];
    int fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd < 0)
    {
        fprintf(stderr, "open() error is %s\n", strerror(errno));
        return;
    }

    // init iovec structures
    for (int i = 0; i < 3; i++)
    {
        iov[i].iov_base = buf[i];
        iov[i].iov_len = strlen(buf[i]) + 1;
        printf("len buf[%d]=%ld\n", i, strlen(buf[i]));
    }

    // write fd from iovec structures
    ssize_t nw = writev(fd, iov, 3);
    if (nw == -1)
    {
        fprintf(stderr, "writev() error = %s/n", strerror(errno));
        return;
    }
    printf("written bytes = %ld\n", nw);

    close(fd);
}

void FragmentedRead()
{
    char one[32];
    char two[32];
    char three[32];
    struct iovec iov[3];
    int fd = open("test.txt", O_RDONLY);
    if (fd < 0)
    {
        fprintf(stderr, "open() error is %s\n", strerror(errno));
        return;
    }

    // init iovec structures
    iov[0].iov_base = one;
    iov[0].iov_len = 9 + 1;
    iov[1].iov_base = two;
    iov[1].iov_len = 9 + 1;
    iov[2].iov_base = three;
    iov[2].iov_len = 11 + 1;

    // read fd to iovec structures
    ssize_t nw = readv(fd, iov, 3);
    if (nw == -1)
    {
        fprintf(stderr, "read() error = %s/n", strerror(errno));
        return;
    }
    printf("read bytes = %ld\n", nw);

    // for (int i = 0; i < 3; i++)
    //     printf("%d: %s", i, (char *)iov[i].iov_base);
    printf("%s", one);
    printf("%s", two);
    printf("%s", three);

    close(fd);
}

void TestParamOrder(int first, int second, int third)
{
    printf("first=%d second=%d third=%d\n", first, second, third);
}

int main()
{
    FragmentedWrite();
    FragmentedRead();

    int a = 10;
    TestParamOrder(a++, a++, a++);

    return 0;
};
