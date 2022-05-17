
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

// int execl(const char *pathname, const char *arg, ... , NULL); // "l" - use list of args
// return -1 if error
//
// int execlp(const char *file, const char *arg, ..., NULL); // "p" - only work with path
// return -1 if error
//
// int execv(const char *pathname, char *const argv[]); // "v" - use vector of args
//
// int execvp(const char *file, char *const argv[]); // "v", "p"
//
// int execve(const char *path, char *const argv[], char *const envp[]); // "e" - new environment
//
// int execle(const char *path, const char *arg0, ..., NULL, char *const envp[]);

void ExeclAbsPath2agr()
{
    if (execl("/mnt/d/Anton/study/Linux/Robert_Love_code/Section5_Process/exec/bin", "arg0", "arg1", NULL) == -1) // any number of arguments
    {
        fprintf(stderr, "execl() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

void ExeclRelativePath3agr()
{
    if (execl("bin", "arg0", "arg1", "arg2", NULL) == -1) // any number of arguments
    {
        fprintf(stderr, "execl() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

void Execlp4Arg()
{
    if (execlp("/mnt/d/Anton/study/Linux/Robert_Love_code/Section5_Process/exec/bin", "arg0", "arg1", "arg2", "arg3", NULL) == -1)
    {
        fprintf(stderr, "execlp() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

void ExecvRelativePath2Arg()
{
    char *args[] = {"arg0", "arg1", NULL};
    if (execv("bin", args) == -1)
    {
        fprintf(stderr, "execv() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

void ExecvAbsPath2Arg()
{
    char *args[] = {"arg0", "arg1", NULL};
    if (execv("/mnt/d/Anton/study/Linux/Robert_Love_code/Section5_Process/exec/bin", args) == -1)
    {
        fprintf(stderr, "execv() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

void Execvp2Arg()
{
    char *args[] = {"arg0", "arg1", NULL};
    if (execvp("/mnt/d/Anton/study/Linux/Robert_Love_code/Section5_Process/exec/bin", args) == -1)
    {
        fprintf(stderr, "execv() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

void ExecveRelativePath2Arg()
{
    char *args[] = {"arg0", "arg1", NULL};
    char *env[] = {"HOME=/usr/home", "LOGNAME=home", (char *)0}; // environment
    if (execve("bin", args, env) == -1)
    {
        fprintf(stderr, "execv() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

void ExecveAbsPath2Arg()
{
    char *args[] = {"arg0", "arg1", NULL};
    char *env[] = {"HOME=/usr/home", "LOGNAME=home", (char *)0}; // environment
    if (execve("/mnt/d/Anton/study/Linux/Robert_Love_code/Section5_Process/exec/bin", args, env) == -1)
    {
        fprintf(stderr, "execv() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

void ExecleRelativePath2Arg()
{
    char *env[] = {"HOME=/usr/home", "LOGNAME=home", (char *)0}; // environment
    if (execle("bin", "arg0", "arg1", NULL, env) == -1)
    {
        fprintf(stderr, "execv() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

void ExecleAbsPath2Arg()
{
    char *env[] = {"HOME=/usr/home", "LOGNAME=home", (char *)0}; // environment
    if (execle("/mnt/d/Anton/study/Linux/Robert_Love_code/Section5_Process/exec/bin", "arg0", "arg1", NULL, env) == -1)
    {
        fprintf(stderr, "execv() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

int main(int argc, char *argv[])
{
    // printf("My binary!\n");
    // for (int i = 0; i < argc; i++)
    //     printf("arg[%d]=%s\n", i, argv[i]);

    // ExeclAbsPath2agr();
    // ExeclRelativePath3agr();
    // Execlp4Arg();
    // ExecvRelativePath2Arg();
    // ExecvAbsPath2Arg();
    // Execvp2Arg();
    // ExecveAbsPath2Arg();
    // ExecveRelativePath2Arg();
    // ExecleRelativePath2Arg();
    ExecleAbsPath2Arg();

    return 0;
};
