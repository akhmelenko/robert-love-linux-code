
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <pthread.h> // pthread_t, pthread_create()
#include <stdio.h>   // print()
#include <stdlib.h>  // exit()

// #include <stdio.h>
//
// "void flockfile(FILE *stream)" - thread blocking if file already locked (increment counter)
//
// "void funlockfile(FILE *stream)" - file locked by thread unlocking (decrement counter)
//
// "int ftrylockfile(FILE *stream)" - thread try to lock without blocking (increment counter if success)
// return 0 if success, else return non 0

struct PContext
{
    int threadNo;
    FILE *f;
};

void *PrintFileBlock(void *pContext)
{
    struct PContext *context = (struct PContext *)pContext;
    flockfile(context->f);
    printf("read from thread#=%d fd=%d\n", context->threadNo, fileno(context->f));
    while (!feof(context->f))
    {
        char buf[50];
        char *ret = fgets(buf, 50, context->f);
        printf("ret=%s buf=%s\n", ret, buf);
        usleep(1000000);
    }
    funlockfile(context->f);
    clearerr(context->f);
    fseek(context->f, 0, SEEK_SET);
    return (void *)&(context->threadNo);
}

void *PrintFileNoneBlock(void *pContext)
{
    struct PContext *context = (struct PContext *)pContext;
    // try lock file non block
    while (ftrylockfile(context->f))
    {
        printf("Try lock file unsuccess thread #%d\n", context->threadNo);
        usleep(500000);
    }
    printf("read from thread#=%d fd=%d\n", context->threadNo, fileno(context->f));
    while (!feof(context->f))
    {
        char buf[50];
        char *ret = fgets(buf, 50, context->f);
        printf("ret=%s buf=%s\n", ret, buf);
        usleep(1000000);
    }
    funlockfile(context->f);
    clearerr(context->f);
    fseek(context->f, 0, SEEK_SET);
    return (void *)&(context->threadNo);
}

void TestBlock()
{
    FILE *f = fopen("test.txt", "r");
    pthread_t thread0, thread1;
    struct PContext pContext0, pContext1;
    pContext0.f = pContext1.f = f;
    pContext0.threadNo = 0;
    pContext1.threadNo = 1;
    int status0 = pthread_create(&thread0, NULL, PrintFileBlock, (void *)&pContext0);
    if (status0)
        printf("pthread_create() 0 error=%s\n", strerror(errno));
    int status1 = pthread_create(&thread1, NULL, PrintFileBlock, (void *)&pContext1);
    if (status1)
        printf("pthread_create() 1 error=%s\n", strerror(errno));
    int *ret;
    pthread_join(thread0, (void **)&ret);
    printf("thread 0 ret=%d\n", *ret);
    pthread_join(thread1, (void **)&ret);
    printf("thread 1 ret=%d\n", *ret);
    fclose(f);
}

void TestNoneBlock()
{
    FILE *f = fopen("test.txt", "r");
    pthread_t thread0, thread1;
    struct PContext pContext0, pContext1;
    pContext0.f = pContext1.f = f;
    pContext0.threadNo = 0;
    pContext1.threadNo = 1;
    int status0 = pthread_create(&thread0, NULL, PrintFileNoneBlock, (void *)&pContext0);
    if (status0)
        printf("pthread_create() 0 error=%s\n", strerror(errno));
    int status1 = pthread_create(&thread1, NULL, PrintFileNoneBlock, (void *)&pContext1);
    if (status1)
        printf("pthread_create() 1 error=%s\n", strerror(errno));
    int *ret;
    pthread_join(thread0, (void **)&ret);
    printf("thread 0 ret=%d\n", *ret);
    pthread_join(thread1, (void **)&ret);
    printf("thread 1 ret=%d\n", *ret);
    fclose(f);
}

int main()
{
    // TestBlock();
    TestNoneBlock();
    return 0;
};
