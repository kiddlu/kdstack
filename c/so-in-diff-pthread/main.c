#include "print.h"

#define MAX_TEST_THREAD 3

void *handle(void *data)
{
    so_print();
    pthread_exit(NULL);
}

int main()
{
    pthread_t pid[MAX_TEST_THREAD];
    int i = 0;
    printf("Now %d threads will be create!\n",MAX_TEST_THREAD);
    for (; i<MAX_TEST_THREAD; i++) {
        pthread_create(&pid, NULL, handle, NULL);
    }
//    printf("Thread have been created!\n");
    sleep(100);
    exit(0);
}