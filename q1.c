#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
int main(void)
{
	FILE *f1, *f2;
	f1 = fopen("child1.txt", "w");
	
	char *child1 = "child 1";
	fprintf(f1, child1);
	f2 = fopen("child2.txt", "w");
	char *child2 = "child 2";
	fprintf(f2, child2);	
	fclose(f1);
	fclose(f2);	
	
	pid_t child1p, child2p;
	int status;
	child1p = fork();
	
	if (child1p == 0)
	{
		sleep(1);
		f1 = fopen("child1.txt", "r");
		char c1;
		while((c1=fgetc(f1)) !=EOF)
		{
			printf("%c",c1);
		}
		printf("\n");
		exit(0);
	}
	else
	{
		child2p = fork();
		if(child2p == 0)
		{
			sleep(1);
			f2 = fopen("child2.txt", "r");
			char c2;
			while((c2=fgetc(f2)) !=EOF)
			{
				printf("%c",c2);
			}
			printf("\n");
			exit(0);
		}
		else
		{
			waitpid(child1p, &status, 0);
			waitpid(child2p, &status, 0);
			printf("I am the parent\n");
		}
	}
}
