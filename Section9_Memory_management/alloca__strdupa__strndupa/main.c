
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <alloca.h>
#include <fcntl.h>  // open()
#include <unistd.h> // close()

// #include <alloca.h>
// void *alloca(size_t size);
// allocates size bytes of space in the stack
// frame of the caller.  This temporary space is automatically freed
// when the function that called alloca() returns to its caller
// Returns a pointer to the beginning of the allocated space

// #include <string.h>
// char *strdupa(const char *s);
// char *strndupa(const char *s, size_t n);
// use alloca(3) to allocate the buffer.
// They are only available when using the GNU GCC suite,
// and suffer from the same limitations described in alloca(3)
// Return: returns a pointer to the duplicated string, or NULL if insufficient memory was available

// alloca()
int open_sysconf(const char *filename, int flags)
{
    const char *etc = "/etc/";
    char *path = alloca(strlen(etc) + strlen(filename) + 1);
    strcpy(path, etc);
    strcat(path, filename);
    int ret = open(path, flags);
    if (ret == -1)
    {
        fprintf(stderr, "open() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return ret;
}

// my str dup (with alloca())
void duplication()
{
    char *song = "song";
    char *dup = alloca(strlen(song) + 1);
    strcpy(dup, song);
    printf("song=%s dup=%s\n", song, dup);
}

int main()
{
    int fd = open_sysconf("at.deny", O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "open_sysconf() error\n");
        exit(EXIT_FAILURE);
    }

    char buf[20];
    read(fd, buf, 20);
    printf("buf=%s\n", buf);

    close(fd);

    // duplication with alloca()
    duplication();

    // strdup() strndup()
    char *buffer = "Long string";
    char *fullString = strdupa(buffer);
    char *partString = strndupa(buffer, 4);
    printf("buffer=%s fullString=%s partStrig=%s\n", buffer, fullString, partString);

    return 0;
}