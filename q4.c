#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define BUFF_SIZE   5           /* total number of slots */
#define NUMBERS      10          /* number of items produced/consumed */

typedef struct
{
    int buf[BUFF_SIZE];   /* shared var */
    int in;               /* buf[in%BUFF_SIZE] is the first empty slot */
    int out;              /* buf[out%BUFF_SIZE] is the first full slot */
    sem_t full;           /* keep track of the number of full spots */
    sem_t empty;          /* keep track of the number of empty spots */
    pthread_mutex_t mutex;          /* enforce mutual exclusion to shared data */
} sbuf_t;

sbuf_t shared;
int input[NUMBERS];

void *Producer()
{
    int i, item, r;
    srand(time(NULL));


    for (i=0; i < NUMBERS; i++)
    {

        /* Produce item */
        item = input[i];

        /* Prepare to write item to buf */

        /* If there are no empty slots, wait */
        sem_wait(&shared.empty);
        /* If another thread uses the buffer, wait */
        pthread_mutex_lock(&shared.mutex);
        shared.buf[shared.in] = item;
        shared.in = (shared.in+1)%BUFF_SIZE;
        printf("Produced %d\n", item);
        fflush(stdout);
        /* Release the buffer */
        pthread_mutex_unlock(&shared.mutex);
        /* Increment the number of full slots */
        sem_post(&shared.full);
	r = rand() % (10 + 1 - 1) + 1;
	sleep(r);
    }
    return NULL;
}

void *Consumer()
{
    int i, item, r;
    srand(time(NULL));
    for (i=NUMBERS; i > 0; i--) {
        sem_wait(&shared.full);
        pthread_mutex_lock(&shared.mutex);
        item=input[i];
        item=shared.buf[shared.out];
	shared.buf[shared.out] = 0;
        shared.out = (shared.out+1)%BUFF_SIZE;
        printf("Consumed  %d\n", item);
        fflush(stdout);
        /* Release the buffer */
        pthread_mutex_unlock(&shared.mutex);
        /* Increment the number of full slots */
        sem_post(&shared.empty);
	r = rand() % (10 + 1 - 1) + 1;
	sleep(r);
    }
    return NULL;
}
int main(void)
{
    pthread_t idP, idC;
    int n;

    sem_init(&shared.full, 0, 0);
    sem_init(&shared.empty, 0, BUFF_SIZE);
    pthread_mutex_init(&shared.mutex, NULL);
    for(int i=0;i<NUMBERS;i++)
    {
	printf("Enter a number\n");
	scanf("%d", &n);
	input[i]=n;
    }
    /* Create a new producer */
    pthread_create(&idP, NULL, Producer, NULL);

    /*create a new Consumer*/
    pthread_create(&idC, NULL, Consumer, NULL);




    (void) pthread_join(idP, NULL);
    (void) pthread_join(idC, NULL);
    printf("Buffer: ");
    for(int i=0;i<BUFF_SIZE;i++)
    {
    	printf("%d ",shared.buf[i]);
    }
    printf("\n");
    return 0;
}
