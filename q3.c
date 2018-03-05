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
	int number;
} num;
num nums[5];
void* factorial(int i)
{
	pthread_t id = pthread_self();
	int c, n, fact = 1;
	n=nums[i].number;
	//printf("%d",n);
	for (c=1;c<=n;c++)
		fact = fact*c;
	printf("factorial of %d = %d\n", n, fact);
	if (i==0)
	{
		moving_sum[i] = fact;
	}
	else
	{
			sem_wait(&mutex);
			if (moving_sum[i-1] > 0)
			{
				fact += moving_sum[i-1];
				moving_sum[i] += fact;
			}
			sem_post(&mutex);
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
		scanf("%d",&x);
		nums[i].number = x;
		//printf("%d",nums[i].number);
		err = pthread_create(&(tid[i]),NULL,&factorial,i);
		if (err != 0)
			printf("\nCan't create thread: [%s]", strerror(err));
		else
			printf("\n Thread created\n");
		sleep(1);
	}
	for (i=0; i<5; i++)
	{
		if(pthread_join(tid[i],NULL))
		{
			printf("Could not join thread %d\n", i);
			return -1;
		}
	}	
	sem_destroy(&mutex);

	for (i=0; i<5; i++)
	{
		printf("%d ", moving_sum[i]);
	}	
	printf("\n");
	return 0;
	

}




