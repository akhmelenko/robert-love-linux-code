
#include <stdio.h>
#include <unistd.h> // stat() fstat() lstat()
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> // open()

// int chmod(const char *pathname, mode_t mode);
// int fchmod(int fd, mode_t mode);

//    S_ISUID  (04000): set-user-ID (set process effective user ID on execve(2))
//    S_ISGID  (02000): set-group-ID (set process effective group ID on execve(2);
//           mandatory locking, as described in fcntl(2); take a new
//           file's group from parent directory, as described in
//           chown(2) and mkdir(2))
//    S_ISVTX  (01000)
//           sticky bit (restricted deletion flag, as described in
//           unlink(2))
//    S_IRUSR  (00400)
//           read by owner
//    S_IWUSR  (00200)
//           write by owner
//    S_IXUSR  (00100)
//           execute/search by owner ("search" applies for directories,
//           and means that entries within the directory can be
//           accessed)
//    S_IRGRP  (00040)
//           read by group
//    S_IWGRP  (00020)
//           write by group
//    S_IXGRP  (00010)
//           execute/search by group
//    S_IROTH  (00004)
//           read by others
//    S_IWOTH  (00002)
//           write by others
//    S_IXOTH  (00001)
//           execute/search by others

void Chmod()
{
    if (chmod("Makefile", S_IRUSR) == -1)
    {
        fprintf(stderr, "chmod() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
}

void Fchmod()
{
    // int fd = open("test", O_DIRECTORY);
    int fd = open("Makefile", O_RDWR);
    if (fd == -1)
    {
        fprintf(stderr, "open() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    if (fchmod(fd, S_IRUSR) == -1)
    {
        fprintf(stderr, "chmod() error=%d (%s)\n", errno, strerror(errno));
        return;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    struct stat buf;
    if (stat("main.c", &buf) == -1)
    {
        fprintf(stderr, "stat() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("before: stat mode and access rights=%x\n", buf.st_mode);

    Chmod();
    // Fchmod();

    if (stat("main.c", &buf) == -1)
    {
        fprintf(stderr, "stat() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("after: stat mode and access rights=%x\n", buf.st_mode);
    return 0;
};
