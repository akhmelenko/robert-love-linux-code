
#include <stdio.h>
#include <signal.h> // signal()
#include <stdlib.h> // exit()
#include <unistd.h> // pause()
#include <errno.h>
#include <string.h>

// #include <signal.h>
// typedef void (*sighandler_t)(int);
// sighandler_t signal(int signum, sighandler_t handler);
// sets the disposition of the signal signum to handler,
// which is either SIG_IGN, SIG_DFL, or the address of a programmer-
// defined function (a "signal handler")
// Returns the previous value of the signal handler.
// On failure, it returns SIG_ERR, and errno is set to indicate the error
//
// #include <unistd.h>
// int pause(void);
// causes the calling process (or thread) to sleep until a
// signal is delivered that either terminates the process or causes
// the invocation of a signal-catching function
// Returns only when a signal was caught and the signal-catching
// function returned. In this case, pause() returns -1, and errno is set to EINTR

void handler(int signo)
{
    if (signo == SIGINT)
        printf("cached signal SIGINT\n");
    else if (signo == SIGTERM)
        printf("cached signal SIGTERM\n");
    else
    {
        fprintf(stderr, "not expected signal=%d\n", signo);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

int main()
{
    // add handler to SIGINT processing
    void *ret = signal(SIGINT, handler);
    if (ret == SIG_ERR)
    {
        fprintf(stderr, "signal() error=%d (%s) unable to process SIGINT\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // add handler to SIGTERM processing
    ret = signal(SIGTERM, handler);
    if (ret == SIG_ERR)
    {
        fprintf(stderr, "signal() error=%d (%s) unable to process SIGTERM\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // make default processing for SIGPROF
    ret = signal(SIGPROF, SIG_DFL);
    if (ret == SIG_ERR)
    {
        fprintf(stderr, "signal() error=%d (%s) unable to process SIGPROF\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // make ignore for SIGHUP
    ret = signal(SIGHUP, SIG_IGN);
    if (ret == SIG_ERR)
    {
        fprintf(stderr, "signal() error=%d (%s) unable to process SIGHUP\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    while (1)
        pause();

    exit(EXIT_SUCCESS);
}