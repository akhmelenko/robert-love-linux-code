
#include <unistd.h> // fork() sleep()
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h> // exit()
#include <signal.h>
#include <stdbool.h>      // bool
#include <sys/wait.h>     // wait
#include <sys/resource.h> // struct rusage

void Test1()
{
    pid_t ppid = getppid();
    printf("my ppid=%d\n", ppid);
    pid_t pid = getpid();
    printf("my pid=%d\n", pid);
    uid_t uid = getuid();
    printf("my uid=%d\n", uid);
    uid_t euid = geteuid();
    printf("my euid=%d\n", euid);
    gid_t gid = getgid();
    printf("my gid=%d\n", gid);
    pid_t sid = getsid(pid);
    printf("my sid=%d\n", sid);

    if (setuid(uid) == -1)
    {
        fprintf(stderr, "setuid() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("eid set to %d\n", uid);
}

// create new session by process witch is not a liader (create child process)
void NewSession()
{
    printf("initial pid=%d\n", getpid());
    printf("initial sid=%d\n", getsid(getpid()));
    printf("initial gid=%d\n", getgid());
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
    printf("check sid=%d\n", getsid(getpid()));
    printf("the same: sid=%d\n", getsid(0));
    printf("new pgid=%d\n", getpgid(0));
    if (setpgid(0, 0) != 0)
    { // 0 - for current process
        fprintf(stderr, "setpgid() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

int main(int argc, char *argv[])
{
    // Test1();
    NewSession();
    return 0;
};
