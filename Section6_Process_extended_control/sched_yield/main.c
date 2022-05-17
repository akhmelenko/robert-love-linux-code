
#include <unistd.h> // fork() sleep()
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sched.h> // sched_yeld()

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid == -1)
    {
        fprintf(stderr, "fork() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    else if (!pid)
    {
        printf("child: started\n");
        int cnt = 0;
        while (1)
        {
            if (10 == cnt++)
            {
                cnt = 0;
                if (sched_yield() == -1)
                {
                    fprintf(stderr, "sched_yield() error=%d (%s)\n", errno, strerror(errno));
                    return -1;
                }
            }
            printf("child: while %d\n", cnt);
        }
    }
    else
    {
        printf("parent: started\n");
        int cnt = 0;
        while (1)
        {
            if (10 == cnt++)
            {
                cnt = 0;
                if (sched_yield() == -1)
                {
                    fprintf(stderr, "sched_yield() error=%d (%s)\n", errno, strerror(errno));
                    return -1;
                }
            }
            printf("parent: while %d\n", cnt);
        }
    }

    return 0;
};
