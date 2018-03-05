#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
pthread_t tid[5];
sem_t mutex;
int total_sum[5] = {0,0,0,0,0};
int j = -1;
void* factorial(int n)
{

	int c, fact = 1;
	for (c=1;c<=n;c++)
		fact = fact*c;
	printf("factorial of %d = %d\n", n, fact);
	if (j==0)
	{
		total_sum[j] = fact;
		printf("Total Sum: %d\n",total_sum[j]);
	}
	else
	{
		sem_wait(&mutex);
		fact += total_sum[j-1];
		total_sum[j] += fact;
		sem_post(&mutex);
		printf("Total Sum: %d\n",total_sum[j]);
	}
	return NULL;
}
int main(void)
{
	int n,x,status,err,i;
	struct info;
	pid_t child1;
	FILE *f1;
	char c1;
	f1 = fopen("numbers.txt", "w");
	sem_init(&mutex, 0, 1);
	for(i=0;i<5;i++)
	{
		printf("Enter a number\n");
		scanf("%d", &n);
		fprintf(f1,"%d", n);
	}
	fclose(f1);
	child1=fork();
	if (child1==0)
	{
		sleep(1);
		f1 = fopen("numbers.txt", "r");
		while((c1=fgetc(f1)) !=EOF)
		{
			x= c1 -'0';
			err = pthread_create(&(tid[i]),NULL,factorial,x);
			i++;
			if (err != 0)
				printf("\nCan't create thread: [%s]", strerror(err));
			else
				printf("\n Thread created\n");
			j++;
			sleep(1);
		}
		printf("\n");
		for (int k=0; k<5; k++)
		{
			pthread_join(tid[k],NULL);

		}
		sem_destroy(&mutex);
		f1 = fopen("sum.txt", "w");
		for(int i = 0;i<5;i++)
		{
			fprintf(f1, "%d ", total_sum[i]);
		}
		fclose(f1);	
		exit(0);
	}
	else
	{
		waitpid(child1, &status, 0);
		f1 = fopen("sum.txt", "r");
		while((c1=fgetc(f1)) !=EOF)
		{
			printf("%c",c1);
		}
		printf("\n");
		exit(0);

	}
}
