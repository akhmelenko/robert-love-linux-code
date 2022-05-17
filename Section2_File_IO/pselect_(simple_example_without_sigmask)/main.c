#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <errno.h>
#include <string.h>
#include <time.h>

// #include <sys/select.h>
// int select(int nfds,
//              fd_set *restrict readfds,
//              fd_set *restrict writefds,
//              fd_set *restrict exceptfds,
//              struct timeval *restrict timeout);

// void FD_CLR(int fd, fd_set *set);
// int FD_ISSET(int fd, fd_set *set);
// void FD_SET(int fd, fd_set *set);
// void FD_ZERO(fd_set *set);

// #include <time.h>
// int pselect(int nfds,
//              fd_set *restrict readfds,
//              fd_set *restrict writefds,
//              fd_set *restrict exceptfds,
//              const struct timespec *restrict timeout,
//              const sigset_t *restrict sigmask);

void PselectAsSelect()
{
    fd_set readfds;
    struct timespec timeout;

    // set timeout (5sec 0usec)
    timeout.tv_sec = 10;
    timeout.tv_nsec = 0;

    // set read file descriptors structure
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    while (1)
    {
        // call select
        int ret = pselect(STDIN_FILENO + 1,
                          &readfds,
                          NULL,
                          NULL,
                          &timeout,
                          NULL);
        printf("select ret=%d\n", ret);
        if (ret == -1)
        {
            fprintf(stderr, "select() error=%s\n", strerror(errno));
            return;
        }
        else if (!ret)
        {
            printf("Timeout\n");
            // !!! NO NEED REINIT TIMER STRUCTURE !!!
            // // again set timeout (5sec 0usec)
            // timeout.tv_sec = 5;
            // timeout.tv_usec = 0;
            // again set read file descriptors structure
            FD_ZERO(&readfds);
            FD_SET(STDIN_FILENO, &readfds);
            continue;
        }

        // check FD
        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            char buf[] = {[0 ... 32] 1};
            int len = read(STDIN_FILENO, buf, sizeof(buf)); // !!! at this moment read() operation (WITH STDIN_FILENO FD) is NON BLOCKING!!!
            if (len == -1)
            {
                fprintf(stderr, "read(STDIN_FILENO) error=%s\n", strerror(errno));
                return;
            }
            else if (len == 0)
            {
                printf("Empty data!\n");
                continue;
            }

            write(STDOUT_FILENO, buf, len);
        }
    }
}

void PselectAsDelay()
{
    printf("start delay of 8 sec\n");
    struct timespec timeout;
    timeout.tv_sec = 8;
    timeout.tv_nsec = 0;
    int ret = pselect(0, NULL, NULL, NULL, &timeout, NULL);
    if (ret == -1)
    {
        fprintf(stderr, "select() error=%s\n", strerror(errno));
        return;
    }
    else if (ret > 0)
    {
        fprintf(stdout, "select() strange situation\n");
        return;
    }
    printf("Timeout event!\n");
}

int main()
{
    PselectAsDelay();
    PselectAsSelect();
    return 0;
};
