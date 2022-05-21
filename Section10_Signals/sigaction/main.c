
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>     // fork() pause()
#include <sys/signal.h> // struct sigaction

// #include <signal.h>
// int sigaction(int signum,
//                  const struct sigaction *restrict act,
//                  struct sigaction *restrict oldact);
// system call is used to change the action taken by a process on receipt of a specific signal
// struct sigaction {
//        void     (*sa_handler)(int);
//        void     (*sa_sigaction)(int, siginfo_t *, void *);
//        sigset_t   sa_mask; // which signal to block during signal handling
//        int        sa_flags; // choice between sa_handler and sa_sigaction
//        void     (*sa_restorer)(void); // not used
//    };
// Returns 0 on success; on error, -1 is returned, and errno is set to indicate the error
//
// int sigqueue(pid_t pid, int sig, const union sigval value);
// sends the signal specified in sig to the process whose PID is given in pid
// The value argument is used to specify an accompanying item of data (either an integer or a pointer value)
// to be sent with the signal, and has the following type:
// union sigval {
//     int   sival_int;
//     void *sival_ptr;
// };

void child_simple_handler(int signo)
{
    printf("child simple handler: sig=%d (%s)\n", signo, strsignal(signo));
}

void child_sa_sigaction_handler(int signo, siginfo_t *siginfo, void *ucontext)
{
    printf("child child_sa_sigaction_handler start: sig=%d (%s)\n", signo, strsignal(signo));
    printf("siginfo:\n");
    printf("signal no=%d\n", siginfo->si_signo);
    printf("errno value=%d\n", siginfo->si_errno);
    printf("signal code=%d\n", siginfo->si_code);
    printf("sender pid=%d\n", siginfo->si_pid);
    printf("sender real uid=%d\n", siginfo->si_uid);
    printf("exit value or signal=%d\n", siginfo->si_status);
    printf("spent time in user mode=%ld\n", siginfo->si_utime);
    printf("spent time in kernel mode=%ld\n", siginfo->si_stime);
    printf("value of useful load of signal=%d\n", siginfo->si_value.sival_int);
    printf("POSIX.1b signal=%d\n", siginfo->si_int);
    printf("POSIX.1b signal=%p\n", siginfo->si_ptr);
    printf("address of error=%p\n", siginfo->si_addr);
    printf("band event=%ld\n", siginfo->si_band);
    printf("file descriptor=%d\n", siginfo->si_fd);
    sleep(5);
    printf("child child_sa_sigaction_handler stop: sig=%d (%s)\n", signo, strsignal(signo));
}

void HandleSignalWithTempBlockOtherSignals()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        printf("parent\n");
        sleep(1);
        // send first signal
        if (kill(pid, SIGUSR1) == -1)
        {
            fprintf(stderr, "kill() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        sleep(1);
        // send second signal
        if (kill(pid, SIGUSR2) == -1)
        {
            fprintf(stderr, "kill() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("child\n");
        // register simple handler for SIGUSR2
        void *oldhandler = signal(SIGUSR2, child_simple_handler);
        if (oldhandler == SIG_ERR)
        {
            fprintf(stderr, "signal() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        // prepare sigaction struct (chose sa_sigaction_handler and mask SIGUSR2)
        sigset_t setBlock;
        // init empty set of signals
        if (sigemptyset(&setBlock) == -1)
        {
            fprintf(stderr, "sigemptyset() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        // add SIGUSR2
        if (sigaddset(&setBlock, SIGUSR2) == -1)
        {
            fprintf(stderr, "sigaddset() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        struct sigaction act =
            {
                .sa_sigaction = child_sa_sigaction_handler,
                .sa_mask = setBlock,   // set of blocking signals
                .sa_flags = SA_SIGINFO // choose sa_sigaction_handler
            };
        struct sigaction oldact;
        // register signal SIGUSR1
        if (sigaction(SIGUSR1, &act, &oldact) == -1)
        {
            fprintf(stderr, "sigaction() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        printf("child: sigaction registered\n");
        pause();
    }
}

void Handle_Plus_SendValueWithSignal()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        printf("parent\n");
        sleep(1);
        // send first signal with value (can be "int" or "void*")
        const union sigval value = {.sival_int = 77};
        if (sigqueue(pid, SIGUSR1, value) == -1)
        {
            fprintf(stderr, "sigqueue() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        sleep(1);
        // send second signal
        if (kill(pid, SIGUSR2) == -1)
        {
            fprintf(stderr, "kill() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("child\n");
        // register simple handler for SIGUSR2
        void *oldhandler = signal(SIGUSR2, child_simple_handler);
        if (oldhandler == SIG_ERR)
        {
            fprintf(stderr, "signal() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        // prepare sigaction struct (chose sa_sigaction_handler and mask SIGUSR2)
        sigset_t setBlock;
        // init empty set of signals
        if (sigemptyset(&setBlock) == -1)
        {
            fprintf(stderr, "sigemptyset() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        // add SIGUSR2
        if (sigaddset(&setBlock, SIGUSR2) == -1)
        {
            fprintf(stderr, "sigaddset() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        struct sigaction act =
            {
                .sa_sigaction = child_sa_sigaction_handler,
                .sa_mask = setBlock,   // set of blocking signals
                .sa_flags = SA_SIGINFO // choose sa_sigaction_handler
            };
        struct sigaction oldact;
        // register signal SIGUSR1
        if (sigaction(SIGUSR1, &act, &oldact) == -1)
        {
            fprintf(stderr, "sigaction() error=%d (%s)\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }
        printf("child: sigaction registered\n");
        pause();
    }
}

int main()
{
    Handle_Plus_SendValueWithSignal();
    return 0;
}