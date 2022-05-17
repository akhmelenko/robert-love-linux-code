
#include <stdio.h>
#include <unistd.h> // stat() fstat() lstat()
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> // open()
#include <grp.h>   // getgrnam() struct group
#include <sys/xattr.h>

// #include <sys/xattr.h>
// ssize_t listxattr(const char *path, char *list, size_t size);
// ssize_t llistxattr(const char *path, char *list, size_t size);
// ssize_t flistxattr(int fd, char *list, size_t size);
// Extended attributes are name:value pairs associated with inodes
//    (files, directories, symbolic links, etc.).

// #include <sys/xattr.h>
// int setxattr(const char *path, const char *name,
//                 const void *value, size_t size, int flags);
// int lsetxattr(const char *path, const char *name,
//                 const void *value, size_t size, int flags);
// int fsetxattr(int fd, const char *name,
//                 const void *value, size_t size, int flags);
// Extended attributes are name:value pairs associated with inodes
//    (files, directories, symbolic links, etc.)

// #include <sys/xattr.h>
// ssize_t getxattr(const char *path, const char *name,
//                 void *value, size_t size);
// ssize_t lgetxattr(const char *path, const char *name,
//                 void *value, size_t size);
// ssize_t fgetxattr(int fd, const char *name,
//                 void *value, size_t size);
// Extended attributes are name:value pairs associated with inodes
//    (files, directories, symbolic links, etc.)

// #include <sys/xattr.h>
// int removexattr(const char *path, const char *name);
// int lremovexattr(const char *path, const char *name);
// int fremovexattr(int fd, const char *name);
// Extended attributes are name:value pairs associated with inodes
//    (files, directories, symbolic links, etc.)

int main(int argc, char *argv[])
{
    int fd = open("test.txt", O_RDWR);

    // get current size of xattr list
    ssize_t retSize = flistxattr(fd, NULL, 0);
    if (retSize == -1)
    {
        fprintf(stderr, "flistxattr() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("xattr size=%ld\n", retSize);
    // get list of xattr
    char *xattrlist = (char *)malloc(retSize);
    retSize = flistxattr(fd, xattrlist, retSize);
    if (retSize == -1)
    {
        fprintf(stderr, "flistxattr() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    // print xattr list
    for (int i = 0; i < retSize; i++)
        printf("%c", xattrlist[i]);

    // set new xattr
    char *key = "myxattrkey";
    int val = 73;
    if (fsetxattr(fd, key, (const void *)&val, sizeof(val), XATTR_CREATE) == -1)
    {
        fprintf(stderr, "fsetxattr() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // get new xattr size
    retSize = fgetxattr(fd, key, NULL, 0);
    if (retSize == -1)
    {
        fprintf(stderr, "fgetxattr() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("xattr size=%ld\n", retSize);
    // get new xattr
    int *gotVal = (int *)malloc(retSize);
    if (fgetxattr(fd, key, (void *)gotVal, retSize) == -1)
    {
        fprintf(stderr, "fgetxattr() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("got xattr value=%d\n", *gotVal);

    // remove xattr
    if (fremovexattr(fd, key) == -1)
    {
        fprintf(stderr, "fremovexattr() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    close(fd);
    return 0;
};
