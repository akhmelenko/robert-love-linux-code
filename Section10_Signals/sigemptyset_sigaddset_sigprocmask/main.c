
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // fork() pause()

// #include <signal.h>
// int sigemptyset(sigset_t *set);
// initializes the signal set pointed to by set, such that all signals defined in POSIX.1‐2008 are excluded
// Return: Upon successful completion, sigemptyset() shall return 0; otherwise, it shall return -1 and set errno to indicate the error
//
// int sigaddset(sigset_t *set, int signo);
// adds the individual signal specified by the signo to the signal set pointed to by set
// Return: Upon successful completion, sigaddset() shall return 0; otherwise, it shall return -1 and set errno to indicate the error
//
// int sigprocmask(int how, const sigset_t *restrict set, sigset_t *restrict oldset);
// is used to fetch and/or change the signal mask of the calling thread.
// The signal mask is the set of signals whose delivery is currently blocked for the caller
// The behavior of the call is dependent on the value of how, as follows:
// SIG_BLOCK
//     The set of blocked signals is the union of (added to) the current set
//     and the set argument.
// SIG_UNBLOCK
//     The signals in set are removed from the current set of
//     blocked signals.  It is permissible to attempt to unblock
//     a signal which is not blocked.
// SIG_SETMASK
//     The set of blocked signals is set to the argument set.
// Returns 0 on success.  On failure, -1 is returned and errno is set to indicate the error

void child_handler(int signo)
{
    printf("childe handler: sig=%d (%s)\n", signo, strsignal(signo));
    exit(EXIT_SUCCESS);
}

void parent_handler(int signo)
{
    printf("parent handler: sig=%d (%s)\n", signo, strsignal(signo));
    exit(EXIT_SUCCESS);
}

int main()
{
    pid_t pid = fork();
    if (pid > 0)
    {
        printf("parent:\n");
        // set parent signal handler
        void *ret = signal(SIGTERM, parent_handler);
        if (ret == SIG_ERR)
        {
            fprintf(stderr, "signal() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        sleep(1);
        // send signal to the group (not working)
        // gid_t gid = getgid();
        // if (killpg(gid, SIGTERM) == -1)
        // send signal to the child
        if (kill(pid, SIGTERM) == -1)
        {
            fprintf(stderr, "kill() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        sleep(1);
        // send signal to self
        if (raise(SIGTERM) != 0)
        {
            fprintf(stderr, "raise() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    else if (pid == 0)
    {
        printf("child\n");
        void *ret = signal(SIGTERM, child_handler);
        if (ret == SIG_ERR)
        {
            fprintf(stderr, "signal() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        // block signals for 3 seconds:
        sigset_t set;
        // init empty set of signals
        if (sigemptyset(&set) == -1)
        {
            fprintf(stderr, "sigemptyset() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        // add SIGTERM
        if (sigaddset(&set, SIGTERM) == -1)
        {
            fprintf(stderr, "sigaddset() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        // start block
        sigset_t oldSet;
        if (sigprocmask(SIG_SETMASK, &set, &oldSet) == -1)
        {
            fprintf(stderr, "sigprocmask() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        sleep(3);
        if (sigprocmask(SIG_UNBLOCK, &set, NULL) == -1)
        {
            fprintf(stderr, "sigprocmask() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        pause();
    }
    else
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return 0;
}