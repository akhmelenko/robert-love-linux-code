#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

// #include <unistd.h>
// ssize_t write(int fd, const void *buf, size_t count);


void Write(){
    int fd = open("test.txt", O_WRONLY | O_TRUNC);
    if (fd == -1){
        fprintf(stderr, "Error open file!\n");
        return;
    }
    char *buf = "My data to write to the file!";
    size_t len = strlen(buf);
    size_t nw;
    printf("len=%ld\n",len);
    while (len > 0 && (nw = write(fd, buf, len)) != 0){
        if (nw == -1){            
            if (errno == EINTR) // system call was interrupted
                continue; // repeat write
            else{
                fprintf(stderr, "serious write() error\n");
                break;
            }
        }
            len -= nw;
            buf += nw;
    }
    printf("len=%ld, data written\n", len);

    close(fd);
}


int main()
{    
    Write();
    return 0;
};
