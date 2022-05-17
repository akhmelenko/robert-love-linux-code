
#include <unistd.h> // nice()
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sched.h> // sched_getscheduler() sched_setscheduler()

// int sched_setparam(pid_t pid, const struct sched_param *param);
// int sched_getparam(pid_t pid, struct sched_param *param);

void GetParam()
{
    struct sched_param sp;
    if (sched_getparam(0, &sp) == -1) // 0 - current process (or pid of any process)
    {
        fprintf(stderr, "sched_getparam() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("Initial priority=%d\n", sp.sched_priority);
}

void SetParam()
{
    struct sched_param sp = {.sched_priority = -1}; // set priority = 1
    if (sched_setparam(0, &sp) == -1)
    {
        fprintf(stderr, "sched_setscheduler() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

int main(int argc, char *argv[])
{
    GetParam();
    SetParam();
    GetParam();
    return 0;
};
