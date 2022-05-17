
#include <unistd.h> // nice()
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sched.h> // sched_getscheduler() sched_setscheduler()

// int sched_setscheduler(pid_t pid,
//                          int policy,
//                          const struct sched_param *param);
// return policy on success or -1 on error
//
// int sched_getscheduler(pid_t pid);
// return 0 on success or -1 on error

void Getscheduler()
{
    int policy = sched_getscheduler(0); // 0 - current process (or pid of any process)
    switch (policy)
    {
    case SCHED_OTHER:
        printf("Normal policy\n");
        break;
    case SCHED_FIFO:
        printf("FIFO policy\n");
        break;
    case SCHED_RR:
        printf("RoundRobin policy\n");
        break;
    case -1:
        fprintf(stderr, "sched_getscheduler() error=%d (%s)\n", errno, strerror(errno));
        break;
    default:
        printf("Unknown policy\n");
    }
}

void Setscheduler()
{
    struct sched_param sp = {.sched_priority = 1}; // set priority = 1
    if (sched_setscheduler(0, SCHED_RR, &sp) == -1)
    {
        fprintf(stderr, "sched_setscheduler() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

int main(int argc, char *argv[])
{
    Setscheduler();
    Getscheduler();
    return 0;
};
