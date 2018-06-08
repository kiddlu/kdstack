#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_TEST_THREAD 500000
void * handle(void *data){
    sleep(100);
    pthread_exit(NULL);
}

int main(){
    pthread_t pid;
    int i=0;
    printf("Now %d threads will be create!\n",MAX_TEST_THREAD);
    for(;i<MAX_TEST_THREAD;i++){
        pthread_create(&pid,NULL,handle,NULL);
    }
    printf("Thread have been created!\n");
    sleep(1);
    exit(0);
}