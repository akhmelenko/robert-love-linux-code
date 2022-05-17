
#define _GNU_SOURCE
#include <unistd.h> // getcws() get_current_dir_name()
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>  // open()
#include <stdlib.h> // exit()

// #include <unistd.h>
// char *getcwd(char *buf, size_t size);
// char *getwd(char *buf);
// char *get_current_dir_name(void);
// These functions return a null-terminated string containing an
//      absolute pathname that is the current working directory of the
//      calling process

// #include <unistd.h>
// int chdir(const char *path);
// int fchdir(int fd);

// #include <sys/stat.h>
// int mkdir(const char *pathname, mode_t mode);
// The argument mode specifies the mode for the new directory (see
//    inode(7)).  It is modified by the process's umask in the usual
//    way: in the absence of a default ACL, the mode of the created
//    directory is (mode & ~umask & 0777)

// #include <unistd.h>
//    int rmdir(const char *pathname);
//       deletes a directory, which must be empty.

void GetCWDNoDynamicAlloc()
{
    char buf[200];
    if (!getcwd(buf, sizeof(buf)))
    {
        fprintf(stderr, "gercwd() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("getcwd() no dynamic alloc=%s\n", buf);
}

void GetCWDDynamicAlloc()
{
    char *buf = getcwd(NULL, 0); // !!! unique usage only for Linux
    if (!buf)
    {
        fprintf(stderr, "getcwd() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("getcwd() dynamic alloc=%s\n", buf);
    free(buf);
}

void GetCurrentDirName()
{
    char *buf = get_current_dir_name(); // !!! unique usage only for Linux
    if (!buf)
    {
        fprintf(stderr, "get_current_dir_name() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("get_current_dir_name() dynamic alloc=%s\n", buf);
    free(buf);
}

void ChangeDir(char *path)
{
    if (chdir(path) == -1)
    {
        fprintf(stderr, "chdir() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void FchangeDir(char *path)
{
    int fd = open(path, O_DIRECTORY);
    if (fchdir(fd) == -1)
    {
        fprintf(stderr, "chdir() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    close(fd);
}

void SaveCurrDir()
{
    int fd = open(".", O_RDONLY); // save curr dir
    // change dir
    if (chdir("/") == -1) // change dir to root
    {
        fprintf(stderr, "chdir() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    GetCurrentDirName();
    // some process...
    // go back
    if (fchdir(fd) == -1)
    {
        fprintf(stderr, "fchdir() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    GetCurrentDirName();
    close(fd);
}

void MakeDir(char *newDir)
{
    if (mkdir(newDir, 0) == -1)
    {
        fprintf(stderr, "mkdir() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void RemoveDir(char *newDir)
{
    if (rmdir(newDir) == -1) // can remove only empty directory
    {
        fprintf(stderr, "rmdir() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    // GetCWDNoDynamicAlloc();
    // GetCWDDynamicAlloc();
    // GetCurrentDirName();
    // ChangeDir(".."); // relative
    // ChangeDir("/mnt/d/Anton/study/Linux/Robert_Love_code/"); // absolute
    // FchangeDir(".."); // relative
    // FchangeDir("/mnt/d/Anton/study/Linux/Robert_Love_code/"); // absolute
    // GetCurrentDirName();

    printf("Save corrent dir to fd\n");
    SaveCurrDir();

    printf("Make dir\n");
    MakeDir("test_dir");
    sleep(2);
    printf("Remove dir\n");
    RemoveDir("test_dir");

    return 0;
};
