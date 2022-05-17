
#include <unistd.h> // fork()
#include <stdio.h>
#include <errno.h>
#include <string.h>
// #include <sys/wait.h>
#include <stdlib.h> // exit()
#include <signal.h> // signal() kill()

#define CHILDREN_NUMBER 10

void ChildExitHandler()
{
    printf("Child pid=%d exited\n", getpid());
}

void KillAllChildren()
{
    pid_t pid[CHILDREN_NUMBER];
    for (int i = 0; i < CHILDREN_NUMBER; i++)
    {
        pid[i] = fork();
        if (pid[i] < 0)
        {
            fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
            return;
        }
        else if (pid[i] == 0)
        {
            printf("Child#%d started\n", i);
            atexit(ChildExitHandler);
            sleep(10);
            exit(1);
        }
    }
    sleep(1);
    signal(SIGQUIT, SIG_IGN); // set parent to ignore SIGQUIT
    kill(0, SIGQUIT);         // send SIGQUIT to all children
    printf("All children killed\n");
}

int main(int argc, char *argv[])
{
    KillAllChildren();
    return 0;
};
