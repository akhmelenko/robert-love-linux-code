
#include <unistd.h> // fork() sleep()
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h> // exit()
#include <signal.h>
#include <stdbool.h>      // bool
#include <sys/wait.h>     // wait
#include <sys/resource.h> // struct rusage

// pid_t wait3(int *wstatus, int options, struct rusage *rusage);
// wait any changed child process
void Wait3()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    else if (pid == 0)
    {
        printf("Child: %d Start\n", getpid());
        sleep(1);
        raise(SIGTERM);
        printf("Child: stopping\n");
    }
    else
    {
        printf("Parent: %d Start\n", getpid());
        int status;
        struct rusage usage;
        pid_t stopedPid = wait3(&status, 0, &usage); // wait any stopped child process
        if (stopedPid == -1)
        {
            fprintf(stderr, "wait3() error=%d (%s)\n", errno, strerror(errno));
            return;
        }
        printf("Parent: child finished\n");
        printf("rusage child_pid=%d printf:\n", stopedPid);
        printf("--used user time=%ld us\n", usage.ru_utime.tv_usec);
        printf("--used system time=%ld us\n", usage.ru_stime.tv_usec);
        printf("--max size of resident part=%ld\n", usage.ru_maxrss);
        printf("--size of all memory=%ld\n", usage.ru_ixrss);
        printf("--size of own data=%ld\n", usage.ru_idrss);
        printf("--size of own stack=%ld\n", usage.ru_isrss);
        printf("--number of restored exceptions=%ld\n", usage.ru_minflt);
        printf("--number of exceptions in pages=%ld\n", usage.ru_majflt);
        printf("--number of page swaps=%ld\n", usage.ru_nswap);
        printf("--number of block input operations=%ld\n", usage.ru_inblock);
        printf("--number of block output operations=%ld\n", usage.ru_oublock);
        printf("--number of sent messages=%ld\n", usage.ru_msgsnd);
        printf("--number of received messages=%ld\n", usage.ru_msgrcv);
        printf("--number of received signals=%ld\n", usage.ru_nsignals);
        printf("--number of volintary context switch=%ld\n", usage.ru_nvcsw);
        printf("--number of obligated context switch=%ld\n", usage.ru_nivcsw);
    }
}

// pid_t wait4(pid_t pid, int *wstatus, int options, struct rusage *rusage);
// wait exact changed child process
void Wait4()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    else if (pid == 0)
    {
        printf("Child: %d Start\n", getpid());
        sleep(1);
        raise(SIGTERM);
        printf("Child: stopping\n");
    }
    else
    {
        printf("Parent: %d Start\n", getpid());
        int status;
        struct rusage usage;
        pid_t stopedPid = wait4(pid, &status, 0, &usage); // wait any stopped child process
        if (stopedPid == -1)
        {
            fprintf(stderr, "wait3() error=%d (%s)\n", errno, strerror(errno));
            return;
        }
        printf("Parent: child finished\n");
        printf("rusage child_pid=%d printf:\n", stopedPid);
        printf("--used user time=%ld us\n", usage.ru_utime.tv_usec);
        printf("--used system time=%ld us\n", usage.ru_stime.tv_usec);
        printf("--max size of resident part=%ld\n", usage.ru_maxrss);
        printf("--size of all memory=%ld\n", usage.ru_ixrss);
        printf("--size of own data=%ld\n", usage.ru_idrss);
        printf("--size of own stack=%ld\n", usage.ru_isrss);
        printf("--number of restored exceptions=%ld\n", usage.ru_minflt);
        printf("--number of exceptions in pages=%ld\n", usage.ru_majflt);
        printf("--number of page swaps=%ld\n", usage.ru_nswap);
        printf("--number of block input operations=%ld\n", usage.ru_inblock);
        printf("--number of block output operations=%ld\n", usage.ru_oublock);
        printf("--number of sent messages=%ld\n", usage.ru_msgsnd);
        printf("--number of received messages=%ld\n", usage.ru_msgrcv);
        printf("--number of received signals=%ld\n", usage.ru_nsignals);
        printf("--number of volintary context switch=%ld\n", usage.ru_nvcsw);
        printf("--number of obligated context switch=%ld\n", usage.ru_nivcsw);
    }
}

int main(int argc, char *argv[])
{
    // Wait3();
    Wait4();
    // int a = 7;
    // int res = (a++, a++, a++, a - 2);
    // printf("a=%d res=%d\n", a, res);
    return 0;
};
