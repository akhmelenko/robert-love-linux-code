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
// "int fseek(FILE *stream, long offset, int whence)"
//
// orign: 
// SEEK_SET: The file offset is set to offset bytes.
// SEEK_CUR: The file offset is set to its current location plus offset bytes.
// SEEK_END: The file offset is set to the size of the file plus offset bytes.
//
// return:
// Upon successful completion, returns the resulting 0. On
// error, -1 is returned and errno is set to indicate the error.

int Example_fseek(){

    FILE *f = fopen("test.txt", "w+");
    if (!f){
        fprintf(stderr,"fopen() error %s", strerror(errno));
        return -1;
    }

    int ret = fwrite("12345", 5, 1, f);
    if (ret == -1){
        fprintf(stderr, "fopen() error %s", strerror(errno));
        return -1;
    }    

    // !!!!!!! set position
    if (fseek(f, 5, SEEK_CUR)){
        fprintf(stderr ,"fseek() error = %s", strerror(errno));
        return -1;
    }

    ret = fwrite("54321", 5, 1, f);
    if (ret == -1){
        fprintf(stderr, "fopen() error %s", strerror(errno));
        return -1;
    }
    
    if (fclose(f) == -1){
        fprintf(stderr, "fclose() error %s", strerror(errno));
        return -1;
    }
}

int Example_fsetpos(){

    FILE *f = fopen("test1.txt", "w+");
    if (!f){
        fprintf(stderr,"fopen() error %s", strerror(errno));
        return -1;
    }

    int ret = fwrite("123456", 6, 1, f);
    if (ret == -1){
        fprintf(stderr, "fopen() error %s", strerror(errno));
        return -1;
    }    

    // !!!!!!! set position
    fpos_t fPos;
    fPos.__pos = 12;
    if (fsetpos(f, &fPos)){
        fprintf(stderr ,"fsetpos() error = %s", strerror(errno));
        return -1;
    }

    ret = fwrite("654321", 6, 1, f);
    if (ret == -1){
        fprintf(stderr, "fopen() error %s", strerror(errno));
        return -1;
    }
    
    // !!!!!!!!!! put position to the start of file
    rewind(f); // == fseek(f, 0, SEEK_SET);

    ret = fwrite("00", 2, 1, f);
    if (ret == -1){
        fprintf(stderr, "fopen() error %s", strerror(errno));
        return -1;
    }

    // !!! get current file position
    int pos = ftell(f);
    if (pos == -1){
        fprintf(stderr, "ftell() error %s", strerror(errno));
        return -1;
    }
    printf("Current position with ftell = %d\n", pos);

    // !!! get current file position (to fpos_t)
    ret = fgetpos(f, &fPos);
    if (ret == -1){
        fprintf(stderr, "ftell() error %s", strerror(errno));
        return -1;
    }
    printf("Current position with fgetpos = %ld\n", fPos.__pos);

    if (fclose(f) == -1){
        fprintf(stderr, "fclose() error %s", strerror(errno));
        return -1;
    }
}

int main()
{    
    Example_fseek();
    Example_fsetpos();

    return 0;
};
