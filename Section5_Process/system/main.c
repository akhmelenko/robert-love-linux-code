
#include <unistd.h> // fork() sleep()
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h> // exit()
// #include <signal.h>
#include <stdbool.h>  // bool
#include <sys/wait.h> // wait
// #include <sys/resource.h> // struct rusage

int MySystem(char *cmd)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    else if (pid == 0)
    {
        // use execv()
        // char *argv[4];
        // argv[0] = "sh";
        // argv[1] = "-c";
        // argv[2] = cmd;
        // argv[3] = NULL;
        // if (execv("/bin/sh", argv) == -1)

        // use execl()
        if (execl("/bin/sh", "sh", "-c", cmd, NULL) == -1)
        {
            fprintf(stderr, "execl() error=%d (%s)\n", errno, strerror(errno));
            return -1;
        }
    }
    else
    {
        int status;
        if (waitpid(pid, &status, 0) == -1)
        {
            fprintf(stderr, "waitpid() error=%d (%s)\n", errno, strerror(errno));
            return -1;
        }
        if (WIFEXITED(status))
            return WIFEXITED(status);
    }
}

void System(const char *cmd)
{
    if (system(cmd) == -1)
    {
        fprintf(stderr, "system() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

int main(int argc, char *argv[])
{
    // MySystem("./bin");
    MySystem("ls -all");
    // System("ls -all");
    return 0;
};
