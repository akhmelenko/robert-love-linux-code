#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

// #include <sys/types.h>
// #include <unistd.h>
// "off_t lseek(int fd, off_t pos, int origin)"
//
// orign: 
// SEEK_SET: The file offset is set to offset bytes.
// SEEK_CUR: The file offset is set to its current location plus offset bytes.
// SEEK_END: The file offset is set to the size of the file plus offset bytes.
//
// return:
// Upon successful completion, lseek() returns the resulting offset. On
// error, the value (off_t) -1 is returned and errno is set to indicate the error.


int main()
{    
    int fd = open("test.txt", O_RDWR);
    // read every even character:
    char ch;
    while(read(fd, &ch, 1) > 0){
        // fflush(stdout);
        write(1, &ch, 1); // write to stdout
        if(lseek(fd, 1, SEEK_CUR) == -1)
            fprintf(stderr, "lseek() error=%s", strerror(errno));
    }

    // get current position:
    int pos = lseek(fd, 0, SEEK_CUR);
    if (pos == -1)
        fprintf(stderr, "lseek() error!");
    printf("\ncurrent position=%d\n", pos);

    // write out of file (with HOLE FILLED BY '\0')
    pos = lseek(fd, 10, SEEK_END); // move pos + 10 characters from file end
    if (pos == -1)
        fprintf(stderr, "lseek() error!");

    char *buf2 = "out of file";
    if (write(fd, buf2, strlen(buf2)) == -1)
        fprintf(stderr, "write() error=%s\n", strerror(errno));    

    if (close(fd) == -1)
        fprintf(stderr, "close error=%s\n", strerror(errno));
    return 0;
};
