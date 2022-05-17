
#define _GNU_SOURCE
#include <unistd.h> // fork() sleep()
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sched.h> // sched_yeld()

// CPU_ZERO(cpu_set_t *set) - Clears set, so that it contains no CPUs.
// CPU_SET(int cpu, cpu_set_t *set) - Add CPU cpu to set.
// CPU_CLR(int cpu, cpu_set_t *set) - Remove CPU cpu from set.
// CPU_ISSET(int cpu, cpu_set_t *set) - Test to see if CPU cpu is a member of set.
// CPU_COUNT(cpu_set_t *set) - Return the number of CPUs in set.

void SchedGetaffinity()
{
    // get cpu number
    long number_of_cpu = sysconf(_SC_NPROCESSORS_ONLN);
    printf("number of cpu=%ld\n", number_of_cpu);

    // attach process to CPU
    cpu_set_t set;
    // clear set
    CPU_ZERO(&set);
    // get affinity
    if (sched_getaffinity(0,                 // pid
                          sizeof(cpu_set_t), // size of cpu_set_t
                          &set) == -1)       // cpu set PTR
    {
        fprintf(stderr, "sched_getaffinity() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    // print process + cpu info
    for (int i = 0; i < number_of_cpu; i++)
    {
        printf("CPU#%d is %s\n", i, CPU_ISSET(i, &set) ? "set" : "unset");
    }
}

void SchedSetaffinity()
{
    // get cpu number
    long number_of_cpu = sysconf(_SC_NPROCESSORS_ONLN);
    printf("number of cpu=%ld\n", number_of_cpu);

    // attach process to CPU
    cpu_set_t set;
    // clear set
    CPU_ZERO(&set);
    // set settings only for first CPU
    CPU_SET(0, &set);
    CPU_CLR(1, &set);
    CPU_CLR(2, &set);
    CPU_CLR(3, &set);
    // get affinity
    if (sched_setaffinity(0,                 // pid
                          sizeof(cpu_set_t), // size of cpu_set_t
                          &set) == -1)       // cpu set PTR
    {
        fprintf(stderr, "sched_getaffinity() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    // print process + cpu info
    for (int i = 0; i < number_of_cpu; i++)
    {
        printf("CPU#%d is %s\n", i, CPU_ISSET(i, &set) ? "set" : "unset");
    }
}

int main(int argc, char *argv[])
{
    // SchedGetaffinity();
    SchedSetaffinity();
    return 0;
};
