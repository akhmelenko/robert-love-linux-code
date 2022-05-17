
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // open()

void Copy(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Wrong number of parameters to copy!\n");
        exit(EXIT_FAILURE);
    }

    // open source
    FILE *srcF = fopen(argv[1], "r");
    if (!srcF)
    {
        fprintf(stderr, "fopen() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // open destination
    FILE *destF = fopen(argv[2], "w+");
    if (!destF)
    {
        fprintf(stderr, "fopen() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // get size of source
    if (fseek(srcF, 0L, SEEK_END) == -1)
    {
        fprintf(stderr, "fseek() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    size_t sz = ftell(srcF);
    if (sz == -1)
    {
        fprintf(stderr, "ftell() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    // !!! put seek pointer back
    if (fseek(srcF, 0L, SEEK_SET) == -1)
    {
        fprintf(stderr, "fseek() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // allocate buffer
    void *buf = malloc(sz);
    if (buf == NULL)
    {
        fprintf(stderr, "malloc() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
    // read data
    int ret = fread(buf, sz, 1, srcF);
    if (ret < 1)
    {
        if (feof(srcF))
            printf("EOF\n");
        else
        {
            fprintf(stderr, "fread() error=%d (%s) ret=%d\n", errno, strerror(errno), ret);
            exit(EXIT_FAILURE);
        }
    }

    // write data
    ret = fwrite(buf, sz, 1, destF);
    if (ret < 1)
    {
        fprintf(stderr, "fwrite() error=%d (%s)\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    free(buf);
    fclose(srcF);
    fclose(destF);

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    Copy(argc, argv);
}