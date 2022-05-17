
#include <unistd.h> // nice()
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/resource.h> // getpriority() setpriority()

// linux low priority form -20(highest) to 19(lowest)

void Nice()
{
    errno = 0;
    int lowPrio = nice(0); // make lowPriority = current lowPriority + 0
    if (lowPrio == -1 && errno != 0)
    {
        fprintf(stderr, "nice() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("initial lowPriority=%d\n", lowPrio);

    lowPrio = nice(10); // make lowPriority = current lowPriority + 10
    if (lowPrio == -1 && errno != 0)
    {
        fprintf(stderr, "nice() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("new lowPriority=%d\n", lowPrio);

    lowPrio = nice(-10); // to increase lowPriority possible only for root! (nice() return error)
    if (lowPrio == -1 && errno != 0)
    {
        fprintf(stderr, "nice() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("new lowPriority=%d\n", lowPrio);
}

void Gerpriority_Setpriority()
{
    int lowPrio = getpriority(PRIO_PROCESS, // PRIO_PROCESS, PRIO_PGRP, PRIO_USER
                           0);           // id(process, group, user) 0 - for caller (process, group, user)
    if (lowPrio == -1 && errno != 0)
    {
        fprintf(stderr, "getpriority() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("initial priority=%d\n", lowPrio);

    int ret = setpriority(PRIO_PROCESS, // PRIO_PROCESS, PRIO_PGRP, PRIO_USER
                          0,            // id(process, group, user) 0 - for caller (process, group, user)
                          10);          // make priority = 10
    if (ret == -1)
    {
        fprintf(stderr, "setpriority() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("new lowPriority=%d\n", getpriority(PRIO_PROCESS, 0));

    ret = setpriority(PRIO_PROCESS, // PRIO_PROCESS, PRIO_PGRP, PRIO_USER
                      0,            // id(process, group, user) 0 - for caller (process, group, user)
                      9);           // make priority = 9 - ERROR (increase priority can only root)

    if (ret == -1)
    {
        fprintf(stderr, "setpriority() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("new lowPriority=%d\n", getpriority(PRIO_PROCESS, 0));
}

int main(int argc, char *argv[])
{
    // Nice();
    Gerpriority_Setpriority();

    return 0;
};
