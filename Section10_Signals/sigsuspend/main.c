
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

int usr_interrupt = 0;

void child_handler(int signo)
{
    usr_interrupt = 1;
    printf("child handler: sig=%d (%s)\n", signo, strsignal(signo));
}

void parent_handler(int signo)
{
    printf("parent handler: sig=%d (%s)\n", signo, strsignal(signo));
    exit(EXIT_SUCCESS);
}

void CriticalOnTime()
{
    pid_t pid = fork();
    if (pid > 0)
    {
        printf("parent:\n");
        // set parent signal handler
        void *ret = signal(SIGUSR2, parent_handler);
        if (ret == SIG_ERR)
        {
            fprintf(stderr, "signal() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        sleep(1);
        // send signal to the child
        if (kill(pid, SIGUSR1) == -1)
        {
            fprintf(stderr, "kill() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        sleep(1);
        // send signal to self
        if (raise(SIGUSR2) != 0)
        {
            fprintf(stderr, "raise() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    else if (pid == 0)
    {
        printf("child\n");
        void *ret = signal(SIGUSR1, child_handler);
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
        // add SIGUSR1
        if (sigaddset(&set, SIGUSR1) == -1)
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
        // !!! START OF CRITICAL SECTION
        sleep(3);
        // !!! END OF CRITICAL SECTION
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
}

void CriticalOnSignalEvent()
{
    pid_t pid = fork();
    if (pid > 0)
    {
        printf("parent:\n");
        // set parent signal handler
        void *ret = signal(SIGUSR2, parent_handler);
        if (ret == SIG_ERR)
        {
            fprintf(stderr, "signal() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        sleep(1);
        // send signal to the child
        if (kill(pid, SIGUSR1) == -1)
        {
            fprintf(stderr, "kill() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        sleep(1);
        // send signal to self
        if (raise(SIGUSR2) != 0)
        {
            fprintf(stderr, "raise() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    else if (pid == 0)
    {
        printf("child\n");
        void *ret = signal(SIGUSR1, child_handler);
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
        // add SIGUSR1
        if (sigaddset(&set, SIGUSR1) == -1)
        {
            fprintf(stderr, "sigaddset() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        // start block
        sigset_t oldSet;
        // unblock if it already blocked for further sigsuspend() - WORKING BUT (NOT ALLOWED TO UNBLOCK NOT BLOCKED SIGNAL)
        if (sigprocmask(SIG_UNBLOCK, &set, &oldSet) == -1)
        {
            fprintf(stderr, "sigprocmask() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        // block signal and save oldSet for sigsuspend()
        if (sigprocmask(SIG_SETMASK, &set, &oldSet) == -1)
        {
            fprintf(stderr, "sigprocmask() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        // !!! START OF CRITICAL SECTION (wait signal)
        int rets = sigsuspend(&oldSet);
        printf("sets=%d\n", rets);
        if (rets == -1)
        {
            if (errno != EINTR)
            {
                fprintf(stderr, "sigsuspend() error=%d (%s)\n", errno, strerror(errno));
                exit(EXIT_FAILURE);
            }
            else
            {
                printf("Signal received, end critical section\n");
            }
        }
        // !!!END OF CRITICAL SECTION(no need usr_interrupt flag)
        // clean mask
        if (sigprocmask(SIG_UNBLOCK, &set, NULL) == -1)
        {
            fprintf(stderr, "sigprocmask() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main()
{
    CriticalOnSignalEvent();
    return 0;
}