
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
// size_t fwrite(void *buf,
//              size_t size,
//              size_t nr,
//              FILE *stream);

#pragma pack(push, 1)
struct Person
{
    char name[32];
    int age;
    int height;
} final, start = {"Anton", 39, 192};
#pragma pop()

int main()
{
    FILE *fout = fopen("test.bin", "w");
    if (!fout)
    {
        printf("Error open file test.txt to create errno=%s\n", strerror(errno));
        return -1;
    }

    int ret = fwrite(&start, sizeof(struct Person), 1, fout);
    if (ret < 1)
        fprintf(stderr, "fwrite() error=%s ret=%d\n", strerror(errno), ret);

    pclose(fout); // !!! CLOSE FILE ONLY WITH "pclose()" - WITH STREAM CLOSE (not with "close()") !!! (if not write buffer of pFile will be lost)

    FILE *fin = fopen("test.bin", "r");
    if (!fin)
    {
        printf("Error open file test.txt to create errno=%s\n", strerror(errno));
        return -1;
    }

    ret = fread(&final, sizeof(struct Person), 1, fin);
    if (ret < 1)
        fprintf(stderr, "fread() error=%s ret=%d\n", strerror(errno), ret);

    printf("name=%s age=%d height=%d\n", final.name, final.age, final.height);

    pclose(fin); // !!! CLOSE FILE ONLY WITH "pclose()" - WITH STREAM CLOSE (not with "close()") !!! (if not write buffer of pFile will be lost)

    return 0;
};
