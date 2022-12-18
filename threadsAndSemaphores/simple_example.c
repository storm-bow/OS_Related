#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

void *routine1(void *arg);
void *routine2(void *arg);
void *routine3(void *arg);

int a2,b1,c1,c2,x,y,z = 0;
sem_t s1,s2,s3,s4,s5,s6,s7;

int main()
{
	pthread_t t1,t2,t3;
	
	
	sem_init(&s1,0,0);
	sem_init(&s2,0,0);
	sem_init(&s3,0,0);
	sem_init(&s4,0,0);
	sem_init(&s5,0,0);
	sem_init(&s6,0,0);
	sem_init(&s7,0,0);
	
	
	if(pthread_create(&t1,NULL,&routine1,NULL)!=0 || pthread_create(&t2,NULL,&routine2,NULL)!=0 || pthread_create(&t3,NULL,&routine3,NULL)!=0)
	{
		perror("Error creating the threads : ");
		exit(1);
	}
	
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_join(t3,NULL);
	
	return 0;
}

void *routine1(void *arg)
{
	int a1 = 10;
	a2 = 11; 
	sem_post(&s3);
	sem_wait(&s1);
	y = a1 + c1; 
	sem_post(&s6);
	sem_wait(&s7);
	printf("%d",x); 

}

void *routine2(void *arg)
{
	int b2 = 21;
	b1 = 20; 
	sem_post(&s4);
	sem_wait(&s2);
	int w = b2 + c2; 
	sem_wait(&s5);
	sem_wait(&s6);
	x = z - y + w;  
	sem_post(&s7);
}

void *routine3(void *arg)
{
	c1 = 30;  
	sem_post(&s1);
	c2 = 31;  
	sem_post(&s2);
	sem_wait(&s3);
	sem_wait(&s4);
	z = a2 + b1;  
	sem_post(&s5);
	
	
}


