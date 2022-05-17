
#include <stdio.h>
#include <unistd.h> // stat() fstat() lstat()
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h> // open()

// ls -1 // - get file names with index descriptors

void Stat()
{
    struct stat buf;
    if (stat("main.c", &buf) == -1)
    {
        fprintf(stderr, "stat() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("stat device id=%ld\n", buf.st_dev);
    printf("stat index desctiptor=%ld\n", buf.st_ino);
    printf("stat mode and access rights=%d\n", buf.st_mode);
    printf("stat hard link number=%ld\n", buf.st_nlink);
    printf("stat user id=%d\n", buf.st_uid);
    printf("stat group id=%d\n", buf.st_gid);
    printf("stat special files device id=%ld\n", buf.st_rdev);
    printf("stat size=%ld\n", buf.st_size);
    printf("stat io block size=%ld\n", buf.st_blksize);
    printf("stat blocks number=%ld\n", buf.st_blocks);
    printf("stat last access time=%ld\n", buf.st_atim.tv_sec);
    printf("stat last modification time=%ld\n", buf.st_mtim.tv_sec);
    printf("stat last meta modificaton time=%ld\n", buf.st_ctim.tv_sec);
}

void Fstat()
{
    int fd = open("main.c", O_RDONLY);
    struct stat buf;
    if (fstat(fd, &buf) == -1)
    {
        fprintf(stderr, "stat() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    printf("stat device id=%ld\n", buf.st_dev);
    printf("stat index desctiptor=%ld\n", buf.st_ino);
    printf("stat mode and access rights=%d\n", buf.st_mode);
    printf("stat hard link number=%ld\n", buf.st_nlink);
    printf("stat user id=%d\n", buf.st_uid);
    printf("stat group id=%d\n", buf.st_gid);
    printf("stat special files device id=%ld\n", buf.st_rdev);
    printf("stat size=%ld\n", buf.st_size);
    printf("stat io block size=%ld\n", buf.st_blksize);
    printf("stat blocks number=%ld\n", buf.st_blocks);
    printf("stat last access time=%ld\n", buf.st_atim.tv_sec);
    printf("stat last modification time=%ld\n", buf.st_mtim.tv_sec);
    printf("stat last meta modificaton time=%ld\n", buf.st_ctim.tv_sec);
    close(fd);
}

void PrintFileType()
{
    struct stat buf;
    if (stat("main.c", &buf) == -1)
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

int PhysicalDevice()
{
    struct stat buf;
    if (stat("main.c", &buf) == -1)
    {
        fprintf(stderr, "stat() error=%d (%s)\n", errno, strerror(errno));
        return -1;
    }
    return buf.st_dev;
}

int main(int argc, char *argv[])
{
    Stat();
    Fstat();
    PrintFileType();
    int devNo = PhysicalDevice();
    printf("dev#=%d\n", devNo);
    return 0;
};
