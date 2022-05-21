
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // fork() pause()

// #include <signal.h>
// int kill(pid_t pid, int sig);
// system call can be used to send any signal to any process group or process
//     If pid is positive, then signal sig is sent to the process with
// the ID specified by pid.
//     If pid equals 0, then sig is sent to every process in the process
// group of the calling process.
//     If pid equals -1, then sig is sent to every process for which the
// calling process has permission to send signals, except for process 1 (init), but see below.
//     If pid is less than -1, then sig is sent to every process in the
// process group whose ID is -pid.
//     If sig is 0, then no signal is sent, but existence and permission checks are still performed;
// this can be used to check for the existence of a process ID or process group ID that the caller is permitted to signal.
// Return: On success (at least one signal was sent), zero is returned.  On
//    error, -1 is returned, and errno is set to indicate the error
//
// int raise(int sig);
// sends a signal to the calling process or
// thread.  In a single-threaded program it is equivalent to
//     kill(getpid(), sig);
// In a multithreaded program it is equivalent to
//     pthread_kill(pthread_self(), sig);
// If the signal causes a handler to be called, raise() will return
// only after the signal handler has returned.
// Returns 0 on success, and nonzero for failure.

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
    }
    else
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return 0;
}