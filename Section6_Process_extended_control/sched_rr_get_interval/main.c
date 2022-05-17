
#include <unistd.h> // fork() sleep()
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sched.h> // sched_rr_get_interval()

// int sched_rr_get_interval(pid_t pid, struct timespec *tp);

int main(int argc, char *argv[])
{
    struct timespec tp;
    struct sched_param sp = {.sched_priority = 1};
    if (sched_setscheduler(0, SCHED_RR, &sp) == -1)
    {
        fprintf(stderr, "sched_setscheduler() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    if (sched_rr_get_interval(0, &tp) == -1)
    {
        fprintf(stderr, "sched_rr_get_interval() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    printf("RR interval=%ld ns\n", tp.tv_nsec);

    return 0;
};
