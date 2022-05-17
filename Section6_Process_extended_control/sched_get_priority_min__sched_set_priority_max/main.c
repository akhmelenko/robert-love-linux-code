
#include <unistd.h> // nice()
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sched.h> // sched_getscheduler() sched_setscheduler()

// int sched_get_priority_max(int policy);
// int sched_get_priority_min(int policy);
//
// policy: SCHED_FIFO, SCHED_RR, SCHED_OTHER
// return: -1 on error or priority

int main(int argc, char *argv[])
{
    int min, max;

    // FIFO
    min = sched_get_priority_min(SCHED_FIFO);
    if (min == -1)
    {
        fprintf(stderr, "sched_get_priority_min() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    printf("min priority for FIFO=%d\n", min);

    max = sched_get_priority_max(SCHED_FIFO);
    if (max == -1)
    {
        fprintf(stderr, "sched_get_priority_max() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    printf("max priority for FIFO=%d\n", max);

    // OTHER
    min = sched_get_priority_min(SCHED_OTHER);
    if (min == -1)
    {
        fprintf(stderr, "sched_get_priority_min() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    printf("min priority for OTHER=%d\n", min);

    max = sched_get_priority_max(SCHED_OTHER);
    if (max == -1)
    {
        fprintf(stderr, "sched_get_priority_max() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    printf("max priority for OTHER=%d\n", max);

    // RR
    min = sched_get_priority_min(SCHED_RR);
    if (min == -1)
    {
        fprintf(stderr, "sched_get_priority_min() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    printf("min priority for RR=%d\n", min);

    max = sched_get_priority_max(SCHED_RR);
    if (max == -1)
    {
        fprintf(stderr, "sched_get_priority_max() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    printf("max priority for RR=%d\n", max);

    struct sched_param sp = {.sched_priority = 1};
    if (sched_getparam(0, &sp) == -1) // 0 - current process (or pid of any process)
    {
        fprintf(stderr, "sched_getparam() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    printf("Initial priority=%d\n", sp.sched_priority);

    sp.sched_priority = max - 1;
    if (sched_setscheduler(0, SCHED_RR, &sp) == -1) // !!!error if no root!!!
    {
        fprintf(stderr, "sched_setscheduler() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }

    if (sched_getparam(0, &sp) == -1) // 0 - current process (or pid of any process)
    {
        fprintf(stderr, "sched_getparam() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    printf("New priority=%d\n", sp.sched_priority);

    sp.sched_priority = max; // set priority = min + 1
    if (sched_setparam(0, &sp) == -1)
    {
        fprintf(stderr, "sched_setparam() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }

    if (sched_getparam(0, &sp) == -1) // 0 - current process (or pid of any process)
    {
        fprintf(stderr, "sched_getparam() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    printf("New priority=%d\n", sp.sched_priority);

    return 0;
};
