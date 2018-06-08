#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>

void sync_thread(void)
{
    struct timeval sync_start;
    struct timeval sync_end;
    unsigned long diff_sec;

    gettimeofday(&sync_start,NULL);

    while (1) {
        sync();
        usleep(200000);
	printf("sync_thread: sync continue\n");

        gettimeofday(&sync_end,NULL);
        diff_sec = sync_end.tv_sec - sync_start.tv_sec;
        if (diff_sec > 20 ) { //start up 20s
            printf("sync_thread: sync done\n");
            break;
        }
    }

    pthread_exit(0);
}

int main(int argc, char **argv)
{
    pthread_t sync_tid;

    int ret;
    ret = pthread_create(&sync_tid, NULL, (void  *)sync_thread, NULL);
    if (ret != 0) {
        printf("Create pthread error!\n");
        return -1;
    }

    pthread_join(sync_tid, NULL);
    return 0;
}
