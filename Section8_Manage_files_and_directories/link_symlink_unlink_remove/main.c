
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // open()

// #include <unistd.h>

// int link(const char *oldpath, const char *newpath); // On error, -1 is returned, and errno is set to indicate the error
// - Creates a new link (also known as a hard link) to an
//  existing file.
//  If newpath exists, it will not be overwritten.

// int symlink(const char *target, const char *linkpath); // On error, -1 is returned, and errno is set to indicate the error.
// - Creates a symbolic link named linkpath which contains
//    the string target

// int unlink(const char *pathname); // On error, -1 is returned, and errno is set appropriately.
// Deletes a name from the file system.
// If that name was the last link to a file and no processes
// have the file open the file is deleted and the space it was using is made available for reuse.

// #include <stdio.h>
// int remove(const char *pathname); // On error, -1 is returned, and errno is set to indicate the error.
// Deletes a name from the filesystem.  It calls unlink(2)
//        for files, and rmdir(2) for directories

int main()
{
    printf("create hard link:\n");
    // create new hard link
    if (link("test.txt", "test_hardlink.txt") == -1)
    {
        fprintf(stderr, "link() error-%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    sleep(1);
    printf("create symbolic link:\n");
    // create symbolic link
    if (symlink("test.txt", "test_symlink.txt") == -1)
    {
        fprintf(stderr, "symlink() error-%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    sleep(4);

    printf("delete hard link:\n");
    // delete new hard link
    if (unlink("test_hardlink.txt") == -1)
    {
        fprintf(stderr, "unlink() error-%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    sleep(1);
    printf("delete symbolic link:\n");
    // delete new symbolic link
    if (unlink("test_symlink.txt") == -1)
    {
        fprintf(stderr, "unlink() error-%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    printf("crate new file\n");
    int fd = open("created.txt", O_CREAT);
    sleep(3);
    close(fd);
    printf("remove new file\n");
    if (remove("created.txt") == -1)
    {
        fprintf(stderr, "remove() error-%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}