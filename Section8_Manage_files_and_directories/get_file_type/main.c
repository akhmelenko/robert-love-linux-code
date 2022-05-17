
#include <stdio.h>
#include <unistd.h> // stat() fstat() lstat()
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void PrintFileType(char *path)
{
    struct stat buf;
    if (stat(path, &buf) == -1)
    {
        fprintf(stderr, "stat() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("stat mode and access rights=%x\n", buf.st_mode);
    switch (buf.st_mode & S_IFMT)
    {
    case S_IFBLK:
        printf("blocking node of device\n");
        break;
    case S_IFCHR:
        printf("text id of device\n");
        break;
    case S_IFDIR:
        printf("directoty\n");
        break;
    case S_IFIFO:
        printf("FIFO\n");
        break;
    case S_IFLNK:
        printf("symbolic link\n");
        break;
    case S_IFREG:
        printf("regular file\n");
        break;
    case S_IFSOCK:
        printf("socket\n");
        break;
    default:
        printf("unknown\n");
        break;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Wrong number of arguments!\n");
        exit(EXIT_FAILURE);
    }
    PrintFileType(argv[1]);
    return 0;
};
