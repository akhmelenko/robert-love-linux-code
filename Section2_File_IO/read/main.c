#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void Read1(){
    char buf[50] = {[0 ... 49] = 33};
    int fd = open("test1.txt", O_RDONLY);
    if (fd < 0)
        perror("Error open file test1.txt to create\n");

    ssize_t nr = read(fd, buf, sizeof(buf)); //!!!


    printf("Read size=%ld\n", nr);
    for (int i = 0; i < sizeof(buf); i++)
        printf("%c", buf[i]);


    close(fd);
}

void Read2(){
    char buf[50] = {[0 ... 49] = 33};
    char *pBuf = buf;
    int fd = open("test2.txt", O_RDONLY);
    if (fd < 0)
        perror("Error open file test2.txt to create\n");
    
    int len = sizeof(buf);
    ssize_t nr;
    while (len != 0 && (nr = read(fd, pBuf, len)) != 0){ //!!! nr == 0 : EOF
        if (nr == -1){
            if (errno == EINTR)
                continue;
            perror("read() error\n");
            break;
        }        
        len -= nr;
        pBuf += nr;
        printf("nr=%ld len=%d\n", nr, len);
    }

    printf("Read len=%d bufShift=%ld\n", len, pBuf - buf);
    for (int i = 0; i < sizeof(buf); i++)
        printf("%c", buf[i]);


    close(fd);
}


int main()
{
    
    Read1(); // not correct: possible partly read (nr < file data)
    
    printf("\n");

    Read2(); // correct

    return 0;
};
