
#define _POSIX_C_SOURCE 200809
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

//// File access
// mode | Meaning       | Explanation                  | Action if file already exists   | Action if file  does not exist
// "r"	| read	        | Open a file for reading      | read from start                 | failure to open
// "w"	| write	        | Create a file for writing	   | destroy contents	             | create new
// "a"	| append        | Append to a file	           | write to end	                 | create new
// "r+"	| read extended	| Open a file for read/write   | read from start	             | error
// "w+"	| write extended| Create a file for read/write | destroy contents	             | create new
// "a+"	|append extended| Open a file for read/write   | write to end#pragma region      | create new

int main()
{
    int fd = open("test.txt", O_RDWR);
    if (fd == -1)
    {
        printf("Error open file test.txt to create errno=%s\n", strerror(errno));
        return -1;
    }

    int ret;

    // create file PTR from FD
    FILE *pFile = fdopen(fd, "a+");
    if (!pFile)
    {
        fprintf(stderr, "fdopen() error=%s\n", strerror(errno));
        return -1;
    }

    // write with FD
    char *buf = "First line from written with FD1\n";
    ret = write(fd, buf, strlen(buf));
    if (ret == -1)
    {
        fprintf(stderr, "FD write error=%s\n", strerror(errno));
        return -1;
    }

    // write with file PTR
    char *buf2 = "Second line written with file PTR\n";
    ret = fprintf(pFile, "%s", buf2);
    if (ret == -1)
    {
        fprintf(stderr, "file PTR write error=%s\n", strerror(errno));
        return -1;
    }
    printf("ret2=%d\n", ret);

    pclose(pFile); // !!! CLOSE FILE ONLY WITH "pclose()" - WITH STREAM CLOSE (not with "close()") !!! (if not write buffer of pFile will be lost)

    return 0;
};
