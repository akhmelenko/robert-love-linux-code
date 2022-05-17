
#include <stdio.h> // sleep()
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/resource.h> // setrlimit() getrlimit()
#include <signal.h>       // signal()

// int getrlimit(int resource, struct rlimit *rlim);
// int setrlimit(int resource, const struct rlimit *rlim);
//
// resource:
// RLIMIT_AS: The maximum size of the process's virtual memory (address space) in bytes. This limit affects calls to brk(2), mmap(2) and mremap(2), which fail with the error ENOMEM upon exceeding this limit. Also automatic stack expansion will fail (and generate a SIGSEGV that kills the process if no alternate stack has been made available via sigaltstack(2)). Since the value is a long, on machines with a 32-bit long either this limit is at most 2 GiB, or this resource is unlimited.
// RLIMIT_CORE: Maximum size of core file. When 0 no core dump files are created. When nonzero, larger dumps are truncated to this size.
// RLIMIT_CPU: CPU time limit in seconds. When the process reaches the soft limit, it is sent a SIGXCPU signal. The default action for this signal is to terminate the process. However, the signal can be caught, and the handler can return control to the main program. If the process continues to consume CPU time, it will be sent SIGXCPU once per second until the hard limit is reached, at which time it is sent SIGKILL. (This latter point describes Linux behavior. Implementations vary in how they treat processes which continue to consume CPU time after reaching the soft limit. Portable applications that need to catch this signal should perform an orderly termination upon first receipt of SIGXCPU.)
// RLIMIT_DATA: The maximum size of the process's data segment (initialized data, uninitialized data, and heap). This limit affects calls to brk(2) and sbrk(2), which fail with the error ENOMEM upon encountering the soft limit of this resource.
// RLIMIT_FSIZE: The maximum size of files that the process may create. Attempts to extend a file beyond this limit result in delivery of a SIGXFSZ signal. By default, this signal terminates a process, but a process can catch this signal instead, in which case the relevant system call (e.g., write(2), truncate(2)) fails with the error EFBIG.
// RLIMIT_LOCKS (Early Linux 2.4 only): A limit on the combined number of flock(2) locks and fcntl(2) leases that this process may establish.
// RLIMIT_MEMLOCK: The maximum number of bytes of memory that may be locked into RAM. In effect this limit is rounded down to the nearest multiple of the system page size. This limit affects mlock(2) and mlockall(2) and the mmap(2) MAP_LOCKED operation. Since Linux 2.6.9 it also affects the shmctl(2) SHM_LOCK operation, where it sets a maximum on the total bytes in shared memory segments (see shmget(2)) that may be locked by the real user ID of the calling process. The shmctl(2) SHM_LOCK locks are accounted for separately from the per-process memory locks established by mlock(2), mlockall(2), and mmap(2) MAP_LOCKED; a process can lock bytes up to this limit in each of these two categories. In Linux kernels before 2.6.9, this limit controlled the amount of memory that could be locked by a privileged process. Since Linux 2.6.9, no limits are placed on the amount of memory that a privileged process may lock, and this limit instead governs the amount of memory that an unprivileged process may lock.
// RLIMIT_MSGQUEUE (Since Linux 2.6.8): Specifies the limit on the number of bytes that can be allocated for POSIX message queues for the real user ID of the calling process. This limit is enforced for mq_open(3). Each message queue that the user creates counts (until it is removed) against this limit according to the formula:
//          bytes = attr.mq_maxmsg * sizeof(struct msg_msg *) +
//                  attr.mq_maxmsg * attr.mq_msgsize
//          where attr is the mq_attr structure specified as the fourth argument to mq_open(3).
//          The first addend in the formula, which includes sizeof(struct msg_msg *) (4 bytes on Linux/i386), ensures that the user cannot create an unlimited number of zero-length messages (such messages nevertheless each consume some system memory for bookkeeping overhead).
// RLIMIT_NICE (since Linux 2.6.12, but see BUGS below): Specifies a ceiling to which the process's nice value can be raised using setpriority(2) or nice(2). The actual ceiling for the nice value is calculated as 20 - rlim_cur. (This strangeness occurs because negative numbers cannot be specified as resource limit values, since they typically have special meanings. For example, RLIM_INFINITY typically is the same as -1.)
// RLIMIT_NOFILE: Specifies a value one greater than the maximum file descriptor number that can be opened by this process. Attempts (open(2), pipe(2), dup(2), etc.) to exceed this limit yield the error EMFILE. (Historically, this limit was named RLIMIT_OFILE on BSD.)
// RLIMIT_NPROC: The maximum number of processes (or, more precisely on Linux, threads) that can be created for the real user ID of the calling process. Upon encountering this limit, fork(2) fails with the error EAGAIN.
// RLIMIT_RSS: Specifies the limit (in pages) of the process's resident set (the number of virtual pages resident in RAM). This limit only has effect in Linux 2.4.x, x < 30, and there only affects calls to madvise(2) specifying MADV_WILLNEED.
// RLIMIT_RTPRIO (Since Linux 2.6.12, but see BUGS): Specifies a ceiling on the real-time priority that may be set for this process using sched_setscheduler(2) and sched_setparam(2).
// RLIMIT_RTTIME (Since Linux 2.6.25): Specifies a limit (in microseconds) on the amount of CPU time that a process scheduled under a real-time scheduling policy may consume without making a blocking system call. For the purpose of this limit, each time a process makes a blocking system call, the count of its consumed CPU time is reset to zero. The CPU time count is not reset if the process continues trying to use the CPU but is preempted, its time slice expires, or it calls sched_yield(2).
//          Upon reaching the soft limit, the process is sent a SIGXCPU signal. If the process catches or ignores this signal and continues consuming CPU time, then SIGXCPU will be generated once each second until the hard limit is reached, at which point the process is sent a SIGKILL signal.
//          The intended use of this limit is to stop a runaway real-time process from locking up the system.
// RLIMIT_SIGPENDING (Since Linux 2.6.8): Specifies the limit on the number of signals that may be queued for the real user ID of the calling process. Both standard and real-time signals are counted for the purpose of checking this limit. However, the limit is only enforced for sigqueue(3); it is always possible to use kill(2) to queue one instance of any of the signals that are not already queued to the process.
// RLIMIT_STACK: The maximum size of the process stack, in bytes. Upon reaching this limit, a SIGSEGV signal is generated. To handle this signal, a process must employ an alternate signal stack (sigaltstack(2)).
//          Since Linux 2.6.23, this limit also determines the amount of space used for the process's command-line arguments and environment variables; for details, see execve(2).

void GetLimits()
{
    struct rlimit rlim;

    // RLIMIT_AS
    if (getrlimit(RLIMIT_AS, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_AS(Address space) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_CORE
    if (getrlimit(RLIMIT_CORE, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_CORE(Core files size) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_CPU
    if (getrlimit(RLIMIT_CPU, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_CPU(Processor time) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_DATA
    if (getrlimit(RLIMIT_DATA, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_DATA(size of data or heap segment) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_FSIZE
    if (getrlimit(RLIMIT_FSIZE, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_FSIZE(size of file can create) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_LOCKS
    if (getrlimit(RLIMIT_LOCKS, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_LOCKS(blocks number) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_MEMLOCK
    if (getrlimit(RLIMIT_MEMLOCK, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_MEMLOCK(memory block size) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_MSGQUEUE
    if (getrlimit(RLIMIT_MSGQUEUE, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_MSGQUEUE(bytes for queue) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_NICE
    if (getrlimit(RLIMIT_NICE, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_NICE(max value of lowPrio) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_NOFILE
    if (getrlimit(RLIMIT_NOFILE, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_NOFILE(file descriptors number + 1) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_NPROC
    if (getrlimit(RLIMIT_NPROC, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_NPROC(one user process number) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_RSS
    if (getrlimit(RLIMIT_RSS, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_RSS(mem pages number) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_RTTIME
    if (getrlimit(RLIMIT_RTTIME, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_RTTIME(cpu time for RT process) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_RTPRIO
    if (getrlimit(RLIMIT_RTPRIO, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_RTPRIO(priority for RT process) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_SIGPENDING
    if (getrlimit(RLIMIT_SIGPENDING, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_SIGPENDING(max signals in process queue (except SIGKILL, SIGTERM)) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // RLIMIT_STACK
    if (getrlimit(RLIMIT_STACK, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_STACK(stack size) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);
}

void KillHeader()
{
    printf("SIGXCPU signal received\n");
}

void SetTimeLimit()
{
    printf("\n");
    struct rlimit rlim;
    // RLIMIT_CPU
    if (getrlimit(RLIMIT_CPU, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_CPU(Processor time) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    // catch all signals
    for (int i = 0; i < 32; i++)
    {
        signal(i, KillHeader);
    }

    rlim.rlim_cur = 5;
    rlim.rlim_max = 5;
    if (setrlimit(RLIMIT_CPU, &rlim) == -1)
    {
        fprintf(stderr, "setrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }

    if (getrlimit(RLIMIT_CPU, &rlim) == -1)
    {
        fprintf(stderr, "getrlim() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("RLIMIT_CPU(Processor time) current limit=%ld max limit=%ld\n", rlim.rlim_cur, rlim.rlim_max);

    while (1)
    {
        volatile static int cnt;
        cnt++;
        // sleep(1);
        // printf("while %d\n", cnt);
    }
}

int main(int argc, char *argv[])
{
    GetLimits();
    SetTimeLimit();

    return 0;
};
