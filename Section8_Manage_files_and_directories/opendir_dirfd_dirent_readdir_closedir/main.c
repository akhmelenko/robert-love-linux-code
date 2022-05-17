
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>

//  #include <sys/types.h>
//  #include <dirent.h>

//  DIR *opendir(const char *name); // return NULL or error
// The opendir() function opens a directory stream corresponding to
//    the directory name, and returns a pointer to the directory
//    stream.  The stream is positioned at the first entry in the
//    directory.

//  DIR *fdopendir(int fd); // return NULL or error
// The fdopendir() function is like opendir(), but returns a
//    directory stream for the directory referred to by the open file
//    descriptor fd.
// int closedir(DIR *dirp); // on error, -1 is returned (errno saved)

// struct dirent *readdir(DIR *dirp);
// The readdir() function returns a pointer to a dirent structure
//        representing the next directory entry in the directory stream
//        pointed to by dirp.  It returns NULL on reaching the end of the
//        directory stream or if an error occurred.
//
// struct dirent
// {
//     ino_t d_ino;             /* Inode number */
//     off_t d_off;             /* Not an offset; see below */
//     unsigned short d_reclen; /* Length of this record */
//     unsigned char d_type;    /* Type of file; not supported by all filesystem types */
//     char d_name[256];        /* Null-terminated filename */
// };

int main()
{
    // open directory stream
    DIR *dir = opendir(".");
    if (dir == NULL)
    {
        fprintf(stderr, "opendir() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // printf directory entires
    struct dirent *entry;
    printf("directory entries:\n");
    while ((entry = readdir(dir)) != NULL)
    {
        printf("struct dirent: d_ino=%ld\n", entry->d_ino);
        printf("struct dirent: d_off=%ld\n", entry->d_off);
        printf("struct dirent: d_reclen=%d\n", entry->d_reclen);
        printf("struct dirent: d_type=%d\n", entry->d_type);
        printf("struct dirent: d_name=%s\n\n", entry->d_name);
    }

    // check for error
    if (errno && !entry)
    {
        fprintf(stderr, "readdir() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // close directory stream
    if (closedir(dir) == -1)
    {
        fprintf(stderr, "closedir() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}