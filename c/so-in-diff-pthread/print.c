#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int num=0;

int so_print(void)
{
	srand((unsigned int)time(NULL));
	unsigned int time=rand();
	
	for(num=0; num<100; num++)
	{
		time = (time + 5 + num) * num % 9 + 2; 
		printf("pid:%d, num:%d, sleep for %d\n", pthread_self(), num, time);
		sleep(time);
	}
}
