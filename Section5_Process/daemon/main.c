
#include <unistd.h> // fork() sleep()
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h> // exit()
// #include <signal.h>
// #include <stdbool.h>      // bool
// #include <sys/wait.h>     // wait
#include <sys/resource.h> // struct rusage
#include <fcntl.h>        // open()
#include <limits.h>

// create new session by process witch is not a liader (create child process)
void MyDaemon()
{
    printf("initial pid=%d\n", getpid());
    pid_t pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    else if (pid != 0)
    {
        // exit from parent process
        exit(EXIT_SUCCESS);
    }

    // child process
    printf("new pid=%d\n", getpid());
    printf("new gid=%d\n", getgid());
    pid_t nPid;
    if ((nPid = setsid()) == -1)
    {
        fprintf(stderr, "setsid() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("new sid=%d\n", nPid);
    char cwd[250];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        fprintf(stderr, "getcwd() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("current dir=%s\n", cwd);

    // change dir to root dir
    if (chdir("/") == -1)
    {
        fprintf(stderr, "chdir() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        fprintf(stderr, "getcwd() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("new dir=%s\n", cwd);

    // close all open files
    for (int i = 0; i < _SC_OPEN_MAX; i++)
        close(i);

    // redirection of descriptors files 0, 1, 2 to  /dev/null
    open("dev/null", O_RDWR); // stdin
    dup(0);                   // stdout
    dup(0);                   // stderr

    // do something...
    printf("something\n");
    return;
}

void Daemon()
{
    if (daemon(0, 0) == -1)
    {
        fprintf(stderr, "daemon() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

int main(int argc, char *argv[])
{
    MyDaemon();
    // Daemon();

    return 0;
};
