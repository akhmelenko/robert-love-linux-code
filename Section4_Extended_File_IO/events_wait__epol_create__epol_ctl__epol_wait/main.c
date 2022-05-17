#include <stdio.h>
#include <sys/epoll.h>
// #include <sys/uio.h>
#include <unistd.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#include <fcntl.h>
// #include <unistd.h>
#include <errno.h>
#include <string.h>

// #include <sys/uio.h>
// struct epoll_event
// {
//     uint32_t events;
//     union
//     {
//         void *ptr;
//         int fd;
//         uint32_t u32;
//         uint64_t u64;
//     } data;
// };
//
// "int epfd = epoll_create1(0);" // creating fd for epoll, parameter == 0
//
// "int epoll_ctl(int epfd, // epoll file descriptor
//                  int op, // operation type (EPOLL_CTL_ADD, EPOLL_CTL_DEL, EPOLL_CTL_MOD)
//                  int fd, // file descriptor of file with event
//                  struct epoll_event *event); // structure""
// return:
// If no error, returns 0.
// On error, returned -1 (and errno set).
//
// "ssize_t writev(int fd, const struct iovec *iov);"
// count - number of buffers to write
// return:
// If no error, returns number of bytes.
// On error, not 0 is returned -1 (and errno set).

#define MAX_EPOLL_EVENTS 2

struct epoll_event events[MAX_EPOLL_EVENTS];

void TestWrite(int epfd, int fdWr)
{
    // add epoll event for write
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fdWr;
    int ret = epoll_ctl(epfd,
                        EPOLL_CTL_ADD,
                        fdWr,
                        &event);
    if (ret < 0)
        printf("epoll_ctl() error=%s\n", strerror(errno));
    // wait epoll
    int eventNumber = epoll_wait(epfd, events, MAX_EPOLL_EVENTS, 1000); // timeout == 1 sec
    if (eventNumber < 0)
        printf("epoll_wait() error=%s\n", strerror(errno));

    for (int i = 0; i < eventNumber; i++)
    {
        printf("event=%d on fd=%d \n", events[i].events, events[i].data.fd);
    }

    // write to file
    char *buf = "Hello";
    ret = write(fdWr, buf, strlen(buf));
    printf("write ret=%d buf size=%ld\n", ret, sizeof(buf));
    if (ret < 0)
        printf("write() error=%s\n", strerror(errno));
}

void TestRead(int epfd, int fdRd)
{
    // add epoll event for read
    struct epoll_event event;
    event.events = EPOLLOUT;
    event.data.fd = fdRd;
    int ret = epoll_ctl(epfd,
                        EPOLL_CTL_ADD,
                        fdRd,
                        &event);
    if (ret < 0)
        printf("epoll_ctl() error=%s\n", strerror(errno));

    int eventNumber = epoll_wait(epfd, events, MAX_EPOLL_EVENTS, 1000);
    if (eventNumber < 0)
        printf("epoll_wait() error=%s\n", strerror(errno));

    for (int i = 0; i < eventNumber; i++)
    {
        printf("event=%d on fd=%d\n", events[i].events, events[i].data.fd);
    }

    char bufR[8];
    ret = read(fdRd, bufR, sizeof(bufR));
    printf("%s", bufR);
}

int main()
{
    // create epoll
    int epfd = epoll_create1(0);
    if (epfd < 0)
        printf("epoll_create1() error=%s\n", strerror(errno));

    int fdWr = open("test.txt", O_WRONLY | O_TRUNC);
    printf("test.txt fdWr=%d\n", fdWr);
    int fdRd = open("test.txt", O_RDONLY);
    printf("test.txt fdRd=%d\n", fdRd);

    TestWrite(epfd, fdWr);
    TestRead(epfd, fdRd);

    close(epfd);
    close(fdWr);
    close(fdRd);
    return 0;
};
