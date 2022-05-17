#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

// #include <stdio.h>
//
// "int ferror(FILE *stream)"
// return:
// If no error, returns the resulting 0.
// On error, not 0 is returned and errno is set to indicate the error.
//
// "int feof(FILE *stream)"
// return:
// If no EOF, returns the resulting 0.
// On EOF, not 0 is returned and errno is set to indicate the error.
//
// "void clearerr(FILE *stream)"
// Clear info of error and EOF indicators.
//
// "int fileno(FILE * stream)"
// return: FD

int main()
{
    FILE *f = fopen("test.txt", "r");
    int fd = fileno(f);
    printf("fd=%d\n", fd);
    while (1)
    {
        char buf[50];
        char *ret = fgets(buf, 50, f);
        printf("ret=%s line=%s\n", ret, buf);
        if (ferror(f))
            printf("error f = %s\n", strerror(errno));
        if (feof(f))
        {
            printf("EOF\n");
            clearerr(f);
            printf("EOF after clearerr() =%d\n", feof(f));
            return 0;
        }
    }
    fclose(f);
    return 0;
};
