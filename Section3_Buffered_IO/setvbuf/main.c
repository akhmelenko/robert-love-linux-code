#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

// #include <stdio.h>
//
// "int setvbuf(FILE *stream, char *buf, int mode, size_t size)"
// buf - buffer for data buffering (inf buf == NULL, glibc - allocate buff automaticaly)
// mode:
// _IONBUF: without buffering
// _IOLBUF: with line buffering (after "\n")
// _IOFBUF: with block buffering
// return:
// If no error, returns the resulting 0.
// On error, not 0 is returned.

char buf[2];

int main()
{
    // set block for buffering to 2 bytes
    int res = setvbuf(stdout, buf, _IOFBF, 2);
    printf("Long line\n");

    return 0;
};
