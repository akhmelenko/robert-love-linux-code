
#include <stdio.h>
#include <unistd.h> // stat() fstat() lstat()
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> // open()
#include <grp.h>   // getgrnam() struct group

// #include <unistd.h>

// int chown(const char *pathname,
//           uid_t owner, // -1 - no change, 0 - change to root
//           gid_t group); // -1 - no change, 0 - change to root
// int fchown(int fd,
//              uid_t owner, // -1 - no change, 0 - change to root
//              gid_t group); // -1 - no change, 0 - change to root
// int lchown(const char *pathname,
//              uid_t owner, // -1 - no change, 0 - change to root
//              gid_t group); // -1 - no change, 0 - change to root

void Chown()
{
    struct group *gr = getgrnam("officers");
    if (gr == NULL)
    {
        fprintf(stderr, "getgrnam() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (chown("test.txt", -1, gr->gr_gid) == -1)
    {
        fprintf(stderr, "chown() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void Fchmod()
{
    // int fd = open("test", O_DIRECTORY);
    int fd = open("test.txt", O_RDWR);
    if (fd == -1)
    {
        fprintf(stderr, "open() error=%d (%s)\n", errno, strerror(errno));
        return;
    }

    struct group *gr = getgrnam("officers");
    if (gr == NULL)
    {
        fprintf(stderr, "getgrnam() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (fchown(fd, -1, gr->gr_gid) == -1)
    {
        fprintf(stderr, "fchown() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    close(fd);
}

void MakeRootOwner()
{
    if (chown("test.txt", 0, 0) == -1)
    {
        fprintf(stderr, "chown() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    struct stat buf;
    if (stat("main.c", &buf) == -1)
    {
        fprintf(stderr, "stat() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("before: stat gid=%d uid=%d\n", buf.st_gid, buf.st_uid);

    // Chown();
    // Fchmod();
    MakeRootOwner();

    if (stat("main.c", &buf) == -1)
    {
        fprintf(stderr, "stat() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("after: stat gid=%d uid=%d\n", buf.st_gid, buf.st_uid);
    return 0;
};
