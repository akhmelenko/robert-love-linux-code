
#define _GNU_SOURCE // gettid();
#include <unistd.h> // gettid();

#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define THREAD_NUMBER 4
pthread_t tid[THREAD_NUMBER];

void *ThreadHandler(void *arg)
{
    int tNum = *(int *)arg;
    int cnt = 0;
    while (cnt < 10)
    {
        sleep(1);
        cnt++;
        printf("thread#:%ld cnt=%d (corepid)=%d\n", pthread_self(), cnt, gettid());
        // disable cancel for thread #0 (enable default)
        if (cnt == 1 && tNum == 0)
        {
            printf("thread#:%ld disable cancel\n", pthread_self());
            int oldState;
            int ret = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &oldState); // PTHREAD_CANCEL_ENABLE
            if (ret != 0)
            {
                fprintf(stderr, "pthread_setcancelstate() error=%d (%s)\n", ret, strerror(ret));
                return NULL;
            }
            // make cancel DEFERRED (default type)
            ret = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldState); // PTHREAD_CANCEL_ASYNCHRONOUS
            if (ret != 0)
            {
                fprintf(stderr, "pthread_setcanceltype() error=%d (%s)\n", ret, strerror(ret));
                return NULL;
            }
        }
        // thread number THREAD_NUMBER - 1 kill others at 5 second
        if (tNum == THREAD_NUMBER - 1)
        {
            if (cnt == 5)
            {
                // stop all other threads
                printf("thread#:%ld cnt=%d stop all other threads\n", pthread_self(), cnt);
                for (int i = 0; i < THREAD_NUMBER; i++)
                {
                    if (pthread_equal(pthread_self(), tid[i]) == 0)
                    {
                        int ret = pthread_cancel(tid[i]);
                        if (ret != 0)
                        {
                            fprintf(stderr, "pthread_cancel() error=%d (%s)\n", ret, strerror(ret));
                            return NULL;
                        }
                    }
                }
            }
            else if (cnt == 7)
            {
                printf("thread#:%ld killer exit\n", pthread_self());
                pthread_exit(NULL);
                // exit(0);
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int num[THREAD_NUMBER];
    for (int i = 0; i < THREAD_NUMBER; i++)
    {
        num[i] = i;
        int ret = pthread_create(&tid[i], NULL, ThreadHandler, &num[i]);
        if (ret != 0)
        {
            fprintf(stderr, "pthread_create() error=%d (%s)\n", ret, strerror(ret));
            return -1;
        }
    }
    for (int i = 0; i < THREAD_NUMBER; i++)
    {
        int ret = pthread_join(tid[i], NULL);
        if (ret != 0)
        {
            fprintf(stderr, "pthread_join() error=%d (%s)\n", ret, strerror(ret));
            return -1;
        }
    }
    printf("main stop\n");
    return 0;
};
