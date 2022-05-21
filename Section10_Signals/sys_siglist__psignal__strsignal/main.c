
#define _GNU_SOURCE
#include <signal.h> // psignal()
#include <string.h> // strsignal()
#include <stdio.h>

int main()
{
    // sig#0=(null)
    // sig#1=Hangup
    // sig#2=Interrupt
    // sig#3=Quit
    // sig#4=Illegal instruction
    // sig#5=Trace/breakpoint trap
    // sig#6=Aborted
    // sig#7=Bus error
    // sig#8=Floating point exception
    // sig#9=Killed
    // sig#10=User defined signal 1
    // sig#11=Segmentation fault
    // sig#12=User defined signal 2
    // sig#13=Broken pipe
    // sig#14=Alarm clock
    // sig#15=Terminated
    // sig#16=Stack fault
    // sig#17=Child exited
    // sig#18=Continued
    // sig#19=Stopped (signal)

    // sys_siglist[]
    printf("\n--sys_siglist[]--\n");
    extern const char *const sys_siglist[];
    for (int i = 0; i < 20; i++)
    {
        printf("sig#%d=%s\n", i, sys_siglist[i]);
    }

    // psignal()
    printf("\n--psignal()--\n");
    for (int i = 0; i < 20; i++)
    {
        char buf[20];
        sprintf(buf, "sig#%d", i);
        psignal(i, buf);
    }

    // psignal()
    printf("\n--strsignal()--\n");
    for (int i = 0; i < 20; i++)
    {
        printf("sig#%d=%s\n", i, strsignal(i));
    }

    return 0;
}