#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{
    int fd = open("test.txt", O_CREAT | O_WRONLY | O_TRUNC);
    if (fd == -1)
        printf("Error open file test.txt to create errno=%s\n", strerror(errno));
    else
        close(fd);

    int fd2 = open("test2.txt", O_WRONLY | O_TRUNC);
    if (fd2 == -1)
        printf("Error open file test2.txt with truncating errno=%s\n", strerror(errno));
    else
        close(fd2);

    // open with permitions
    int fd3 = open("test3.txt", O_CREAT | O_WRONLY, S_IRWXU | S_IRWXG);
    if (fd == -1)
        printf("Error open file test3.txt with permitions errno=%s\n", strerror(errno));
    else
        close(fd3);

    return 0;
};
