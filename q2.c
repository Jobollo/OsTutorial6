#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
int main(void)
{
	pid_t childp;
	childp = fork();
	int status;
	if (childp==0)
	{
		sleep(1);
		printf("Child process\n");
		exit(0);
	}
	else
	{
		waitpid(childp, &status, 0);
		printf("Parent Process\n");
		
	}
}
