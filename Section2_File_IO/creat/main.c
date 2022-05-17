#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    // int fd = open("test.txt", O_CREAT | O_WRONLY | O_TRUNC);
    int fd = creat("test.txt", 0644); // same as prev line
    if (fd < 0)
        printf("Error open file test.txt to create\n");
    else
        close(fd);

    return 0;
};
