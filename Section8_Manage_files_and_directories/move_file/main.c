
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // open()

int main(int argc, char *argv[])
{
    if (rename(argv[1], argv[2]) == -1)
    {
        fprintf(stderr, "rename() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}