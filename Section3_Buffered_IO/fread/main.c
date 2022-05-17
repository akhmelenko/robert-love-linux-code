
// #define _POSIX_C_SOURCE 200809
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

// #include <stdio.h>
// size_t fread(void *buf, size_t size, size_t nr, FILE *stream) - read from binary file
// return number of read blocks
// ruturn < nr: error or EOF

int main()
{
    FILE *f = fopen("test.bin", "r");
    if (!f)
    {
        printf("Error open file test.txt to create errno=%s\n", strerror(errno));
        return -1;
    }

    char buf[10];
    int blockLen = 5;
    int blocksNumber = 2;

    int ret;
    while (ret = fread(buf, blockLen, blocksNumber, f))
    { // read 1 block of 4 bytes
        if (ret < blocksNumber)
        {
            printf("ret=%d\n", ret);
            if (feof(f))
            {
                printf("EOF\n");
            }
            else
            {
                fprintf(stderr, "fread() error=%s\n", strerror(errno));
                return -1;
            }
        }
        buf[ret * blocksNumber] = '\0';
        fprintf(stdout, "ret=%d buf=%s\n", ret, buf);
    }

    pclose(f); // !!! CLOSE FILE ONLY WITH "pclose()" - WITH STREAM CLOSE (not with "close()") !!! (if not write buffer of pFile will be lost)

    return 0;
};
