#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <poll.h>

// #include <poll.h>
// int poll(struct pollfd *fds, // array of FDs structures
//          nfds_t nfds, // number of FDs
//          int timeout); // timeout in milliseconds
// struct pollfd {
//  int fd;        /* file descriptor */
//  short events;  /* requested events */
//  short revents; /* returned events */
// };
// Return: number of nonzero revents; -1 is error; 0 - timeout
//
// Events:
// POLLIN: There is data to read.
// POLLRDNORM: Equivalent to POLLIN.
// POLLRDBAND: There is priority data to read.
// POLLPRI: There is urgent data to read.
// POLLOUT: Write will not block
// POLLWRNORM: Equivalent to POLLOUT.
// POLLWRBAND: Priority data Write will not block.
// POLLMSG: Message SIGPOLL is available

// Revents:
// POLLER: Error of FD
// POLLHUP: Hang of FD
// POLLNVAL: FD is not allowed

#define FD_NUMBER 2
#define TIMEOUT_IN_SECONDS 10

int main()
{
    // poll by stdin and stdout
    struct pollfd fds[FD_NUMBER];
    fds[0].fd = STDIN_FILENO;
    fds[1].fd = STDOUT_FILENO;
    fds[0].events = POLLIN;
    fds[1].events = POLLOUT;

    int ret = poll(fds, FD_NUMBER, TIMEOUT_IN_SECONDS * 1000);
    if (ret == -1)
    {
        fprintf(stderr, "poll() error=%s\n", strerror(errno));
        return -1;
    }
    else if (ret == 0)
    {
        printf("Timeout\n");
        return 0;
    }

    // check results:
    if (fds[0].revents & POLLIN)
    {
        printf("STDIN readable\n");
        char buf[32];
        int rn = read(STDIN_FILENO, buf, sizeof(buf));
        if (rn == -1)
        {
            fprintf(stderr, "read() error=%s\n", strerror(errno));
            return -1;
        }
        printf("data:\n");
        write(STDOUT_FILENO, buf, rn);
    }

    if (fds[1].revents & POLLOUT)
    {
        printf("\nSTDOUT writeable\n");
        char *str = "Data for output";
        write(STDOUT_FILENO, str, strlen(str));
    }

    return 0;
};
