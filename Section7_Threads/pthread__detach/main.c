
#define _GNU_SOURCE // gettid();
#include <unistd.h> // gettid();

#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

void *ThreadHandler(void *arg)
{
    // detaching from main thread !!! works only with "pthread_exit(NULL)" in main thread!!!
    int ret = pthread_detach(pthread_self()); // detaching from main thread !!! works only with "pthread_exit(NULL)" in main thread!!!
    if (ret != 0)
    {
        fprintf(stderr, "pthread_detach() error=%d (%s)\n", ret, strerror(ret));
        return NULL;
    }
    int cnt = 0;
    while (cnt < 10)
    {
        sleep(1);
        cnt++;
        printf("thread#:%ld cnt=%d (corepid)=%d\n", pthread_self(), cnt, gettid());
    }
    return NULL;
}

void TryJoinDetached()
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, ThreadHandler, NULL);
    if (ret != 0)
    {
        fprintf(stderr, "pthread_create() error=%d (%s)\n", ret, strerror(ret));
        return;
    }
    sleep(1);
    ret = pthread_join(tid, NULL);
    if (ret != 0)
    {
        fprintf(stderr, "pthread_join() error=%d (%s)\n", ret, strerror(ret));
        return;
    }
    printf("main stop\n");
}

void Detached()
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, ThreadHandler, NULL);
    if (ret != 0)
    {
        fprintf(stderr, "pthread_create() error=%d (%s)\n", ret, strerror(ret));
        return;
    }
    sleep(3);
    printf("main stop\n");
    pthread_exit(NULL); // !!! use to not kill detached thread
}

int main(int argc, char *argv[])
{
    Detached();
    // TryJoinDetached();
    return 0;
};
