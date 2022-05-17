
#include <unistd.h> // fork() sleep()
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h> // exit()
#include <signal.h>
#include <stdbool.h> // bool
#include <sys/wait.h>

bool finish = false;

void EndChildProcess()
{
    printf("End child process\n");
    // exit(0);
    finish = true;
}

void Sigchld()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    else if (pid == 0)
    {
        printf("Child: Start\n");
        sleep(1);
    }
    else
    {
        printf("Parent: Start\n");
        signal(SIGCHLD, EndChildProcess);
        while (!finish)
            ;
    }
}

void WaitSingle()
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
        printf("Child: stopping\n");
        // raise(SIGTERM); // "killed by signal=15"
        raise(SIGABRT); // "killed by signal=6 core dumped"
        // raise(SIGSTOP); // ?
        // raise(SIGCONT); // ?
        // *(size_t *)0 = 0; // "killed by signal=11 core dumped"
        // exit(0); "normal exiting, status=1"
    }
    else
    {
        printf("Parent: %d Start\n", getpid());
        int status;
        sleep(2);
        pid_t stopedPid = wait(&status);
        printf("Parent: pid=%d stoped\n", getpid());
        if (WIFEXITED(status))
            printf("normal exiting, status=%d\n", WIFEXITED(status));
        if (WIFSIGNALED(status))
            printf("killed by signal=%d %s\n", WTERMSIG(status), WCOREDUMP(status) ? "core dumped" : "");
        if (WIFSTOPPED(status))
            printf("stoped by signal=%d\n", WSTOPSIG(status));
        if (WIFCONTINUED(status))
            printf("continued\n");
    }
}

void WaitMultiple()
{
    pid_t pid1 = fork();
    if (pid1 < 0)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    else if (pid1 == 0)
    {
        printf("Child1: %d Start\n", getpid());
        sleep(2);
        exit(1);
    }
    else
    {
        printf("Parent: %d Start1\n", getpid());
        pid_t pid2 = fork();
        if (pid2 < 0)
        {
            fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
            return;
        }
        else if (pid2 == 0)
        {
            printf("Child2: %d Start\n", getpid());
            sleep(1);
            exit(2);
        }
        else
        {
            printf("Parent: %d Start2\n", getpid());
            int status;
            int exitCnt = 0;
            while (exitCnt < 2)
            {
                int exitPid = wait(&status);
                exitCnt++;
                printf("Parent: %d exited child=%d with status=%d\n", getpid(), exitPid, status);
            }
        }
    }
}

// pid_t waitpid(pid_t pid, int *status, int options);
// pid:
// < -1: meaning wait for any child process whose process group ID is equal to the absolute value of pid.
// -1 meaning wait for any child process.
// 0 meaning wait for any child process whose process group ID is equal to that of the calling process.
// > 0 meaning wait for the child whose process ID is equal to the value of pid.
// RETURN: on success, returns the process ID of the child whose state has changed;
// if WNOHANG was specified and one or more child(ren) specified by pid exist,
// but have not yet changed state, then 0 is returned. On error, -1 is returned.
void WaitpidMinus1()
{
    pid_t pid1 = fork();
    if (pid1 < 0)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    else if (pid1 == 0)
    {
        printf("Child1: %d Start\n", getpid());
        sleep(2);
        exit(1);
    }
    else
    {
        printf("Parent: %d Start1\n", getpid());
        pid_t pid2 = fork();
        if (pid2 < 0)
        {
            fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
            return;
        }
        else if (pid2 == 0)
        {
            printf("Child2: %d Start\n", getpid());
            sleep(1);
            exit(2);
        }
        else
        {
            printf("Parent: %d Start2\n", getpid());
            int status;
            int exitCnt = 0;
            while (exitCnt < 2)
            {
                int exitPid = waitpid(-1, &status, 0); // if "-1" work as wait()
                exitCnt++;
                printf("Parent: %d exited child=%d with status=%d\n", getpid(), exitPid, status);
            }
        }
    }
}

void WaitpidExact()
{
    pid_t pid1 = fork();
    if (pid1 < 0)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    else if (pid1 == 0)
    {
        printf("Child1: %d Start\n", getpid());
        sleep(1);
        printf("Child1: stopping\n");
        exit(1);
    }
    else
    {
        printf("Parent: %d Start1\n", getpid());
        pid_t pid2 = fork();
        if (pid2 < 0)
        {
            fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
            return;
        }
        else if (pid2 == 0)
        {
            printf("Child2: %d Start\n", getpid());
            sleep(2);
            printf("Child2: stopping\n");
            exit(2);
        }
        else
        {
            printf("Parent: %d Start2\n", getpid());
            int status;
            int exitPid = waitpid(pid1, &status, 0);
            printf("Parent: %d exited child=%d with status=%d\n", getpid(), exitPid, status);
            exitPid = waitpid(pid2, &status, 0);
            printf("Parent: %d exited child=%d with status=%d\n", getpid(), exitPid, status);
        }
    }
}

void WaitpidNoblock()
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
        printf("Child: stopping\n");
    }
    else
    {
        printf("Parent: %d Start\n", getpid());
        pid_t retPid = 0;
        int status;
        while (!retPid)
        {
            retPid = waitpid(pid, &status, WNOHANG); // when "WNOHANG" - not blocked
            printf("Parent: wait cycle\n");
        }
        printf("Parent: child pid=%d finished with status=%d\n", retPid, status);
    }
}

// int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options);
// returns 0 on success or if WNOHANG was specified and no child(ren) specified by id has yet changed state;
// on error, -1 is returned. Each of these calls sets errno to an appropriate value in the case of an error.
void Waitid()
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
        printf("Child: stopping\n");
        raise(SIGTERM);
        exit(1);
    }
    else
    {
        printf("Parent: %d Start\n", getpid());
        pid_t retPid = 0;
        siginfo_t info;
        if (waitid(P_PID, (id_t)pid, &info, WEXITED) == -1)
        {
            fprintf(stderr, "waitid() error=%d (%s)\n", errno, strerror(errno));
            return;
        }
        printf("Parent: child pid=%d finished with signal code=%d\n", retPid, info.si_code);
    }
}

int main(int argc, char *argv[])
{
    // Sigchld();
    // WaitSingle();
    // WaitMultiple();
    // WaitpidMinus1();
    // WaitpidExact();
    // WaitpidNoblock();
    Waitid();
    return 0;
};
