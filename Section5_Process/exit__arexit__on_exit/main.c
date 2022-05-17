
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void Exit1NoArg()
{
    printf("Exit1\n");
}

void Exit2NoArg()
{
    printf("Exit2\n");
}

void AtExit()
{
    if (atexit(Exit1NoArg) == -1)
    {
        fprintf(stderr, "atexit() %d (%s)\n", errno, strerror(errno));
        exit(-1);
    }
    if (atexit(Exit2NoArg) == -1)
    {
        fprintf(stderr, "atexit() %d (%s)\n", errno, strerror(errno));
        exit(-1);
    }
    exit(-1);
}

void Exit1Arg(int status, void *arg)
{
    printf("Exit1 status=%d arg=%d\n", status, *(int *)arg);
}

void Exit2Arg(int status, void *arg)
{
    printf("Exit2 status=%d arg=%d\n", status, *(int *)arg);
}

void On_Exit()
{
    int arg1 = 11;
    if (on_exit(Exit1Arg, (void *)&arg1) == -1)
    {
        fprintf(stderr, "on_exit() error=%d (%s)\n", errno, strerror(errno));
        exit(-1);
    }
    int arg2 = 22;
    if (on_exit(Exit2Arg, (void *)&arg2) == -1)
    {
        fprintf(stderr, "on_exit() error=%d (%s)\n", errno, strerror(errno));
        exit(-1);
    }
    int arg3 = 33;
    if (on_exit(Exit1Arg, (void *)&arg3) == -1)
    {
        fprintf(stderr, "on_exit() error=%d (%s)\n", errno, strerror(errno));
        exit(-1);
    }
    exit(-1);
}

int main(int argc, char *argv[])
{
    // AtExit();
    On_Exit();
    return 0;
};
