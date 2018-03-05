#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

pthread_t tid[5];
sem_t mutex;
int moving_sum[5] = {0,0,0,0,0};

typedef struct {
	int number[5];
} num;
num nums[5];
void* factorial(void *arg)
{
	pthread_t id = pthread_self();
	int c, n, i=0, fact = 1;
	n=nums[i].number;
	printf("%d",n);
	for (c=1;c<=n;c++)
		fact = fact*c;
	printf("factorial of %d = %d\n", n, fact);
	
	while (moving_sum[i-1] <= 0)
	{
		sem_wait(&mutex);
		if (moving_sum[i-1] > 0)
		{
			fact += moving_sum[i-1];
			moving_sum[i] += fact;
		}
		sem_post(&mutex);
		i++;
	}
	
	return NULL;
}

int main(void)
{
	int i;
	int err;
	int x;
	struct nums;
	sem_init(&mutex, 0, 1);
	for (i=0; i<5; i++)
	{
		printf("Enter number %d: ", i+1);
		scanf("%d",&nums.number[i]);
		printf("%d",nums.number[i]);
		err = pthread_create(&(tid[i]),NULL,&factorial,NULL);
		if (err != 0)
			printf("\nCan't create thread: [%s]", strerror(err));
		else
			printf("\n Thread created\n");
	}

	return 0;
	

}




