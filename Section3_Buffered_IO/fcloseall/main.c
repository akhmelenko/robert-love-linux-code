
#define _GNU_SOURCE
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

//
// #define _GNU_SOURCE
// #include <stdio.h>
// int fcloseall(void);
//

int main()
{
    int ret;

    FILE *pFile1 = fopen("test1.txt", "w");
    if (!pFile1)
    {
        fprintf(stderr, "fopen() error=%s\n", strerror(errno));
        return -1;
    }

    FILE *pFile2 = fopen("test2.txt", "w");
    if (!pFile2)
    {
        fprintf(stderr, "fopen() error=%s\n", strerror(errno));
        return -1;
    }

    // write 1
    char *buf = "First file from written with PTR\n";
    ret = fputs(buf, pFile1);
    if (ret == -1)
    {
        fprintf(stderr, "FD write error=%s\n", strerror(errno));
        return -1;
    }

    // write 2
    char *buf2 = "Second file written with file PTR\n";
    ret = fputs(buf2, pFile2);
    if (ret == -1)
    {
        fprintf(stderr, "file PTR write error=%s\n", strerror(errno));
        return -1;
    }
    printf("ret2=%d\n", ret);

    fcloseall(); // !!! CLOSE ALL FILE STREAMS

    return 0;
};
