
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

void Fork()
{
    int a = 5;
    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    else if (pid == 0)
    {
        a = 7;
        printf("Child: pid=%d ppid=%d a=%d\n", getpid(), getppid(), a);
    }
    else
    {
        a = 6;
        printf("Parent: pid=%d child pid=%d  a=%d\n", getpid(), pid, a);
    }
}

void ForkExecl()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    else if (pid == 0)
    {
        printf("Child: pid=%d ppid=%d\n", getpid(), getppid());
        sleep(1);
        if (execl("bin", "arg0", "arg1", NULL) == -1)
        {
            fprintf(stderr, "execl() error=%d (%s)\n", errno, strerror(errno));
            return;
        }
    }
    else
    {
        printf("Parent: pid=%d child pid=%d\n", getpid(), pid);
        waitpid(pid, NULL, WUNTRACED | WCONTINUED);
        printf("Child finished\n");
    }
}

void VFork() // ! without "_exit()" - segmentation fault!
{
    // int a = 5;
    pid_t pid = vfork();
    if (pid < 0)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    else if (pid == 0)
    {
        // a = 7;
        printf("Child: pid=%d ppid=%d\n", getpid(), getppid());
        _exit(1); // !!! without "_exit()" at child faced with seg fault !!!
    }
    else
    {
        // a = 6;
        printf("Parent: pid=%d child pid=%d\n", getpid(), pid);
        // waitpid(pid, NULL, WUNTRACED | WCONTINUED);
    }
}

int main(int argc, char *argv[])
{
    // Fork();
    // ForkExecl();
    VFork(); // ! without "_exit()" - segmentation fault!
    return 0;
};
