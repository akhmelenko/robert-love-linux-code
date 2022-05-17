
#define _GNU_SOURCE // gettid();
#include <unistd.h> // gettid();

#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define THREAD_NUMBER 10
#define ACCOUNT_NUMBER 2

struct Account
{
    int balance;
    pthread_mutex_t mut;
};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct Account acc[ACCOUNT_NUMBER];

void InitAccount(struct Account *acc)
{
    acc->balance = 0;
    pthread_mutex_init(&acc->mut, NULL);
}

int Balance(struct Account *acc)
{
    pthread_mutex_lock(&acc->mut);
    const int balance = acc->balance;
    pthread_mutex_unlock(&acc->mut);
    return balance;
}

void Deposit(struct Account *acc, int amount)
{
    pthread_mutex_lock(&acc->mut);
    acc->balance += amount;
    pthread_mutex_unlock(&acc->mut);
}

int Withdraw(struct Account *acc, int amount)
{
    pthread_mutex_lock(&acc->mut);
    if (acc->balance < amount)
    {
        pthread_mutex_unlock(&acc->mut);
        return -1;
    }
    acc->balance -= amount;
    pthread_mutex_unlock(&acc->mut);
    return 0;
}

void *Thread(void *arg)
{
    srand(time(NULL));
    while (1)
    {
        // generete account, operation and amount
        int accNo = rand() % ACCOUNT_NUMBER;
        int amount = rand() % 10;
        int opNo = rand() % 2;
        switch (opNo)
        {
        case 0:
            Deposit(&acc[accNo], amount);
            printf("thread#%ld account#=%d deposit amount=%d balance=%d\n", pthread_self(), accNo, amount, Balance(&acc[accNo]));
            break;
        case 1:
        {
            int withRes = Withdraw(&acc[accNo], amount);
            printf("thread#%ld account#=%d withdrawal amount=%d result=%d  balance=%d\n", pthread_self(), accNo, amount, withRes, Balance(&acc[accNo]));
            if (withRes == -1)
            {
                printf("!!! Not enough account balance for withdraw %d (balance=%d)\n", amount, Balance(&acc[accNo]));
                pthread_exit(NULL);
            }
        }
        break;
        default:
            printf("Wrong operation generated\n");
            break;
        }
    }
}

pthread_mutex_t mut;

int main(int argc, char *argv[])
{
    // init accounts
    for (int i = 0; i < ACCOUNT_NUMBER; i++)
        InitAccount(&acc[i]);
    // start threads
    pthread_t tid[THREAD_NUMBER];
    for (int i = 0; i < THREAD_NUMBER; i++)
    {
        int ret = pthread_create(&tid[i], NULL, Thread, NULL);
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
    return 0;
};
