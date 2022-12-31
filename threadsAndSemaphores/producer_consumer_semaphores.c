#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <time.h>

#define n 5

char list_of_products[5][11] = {"baclava","muffin","cheesecake","croissant","lemonpie"}; 

void* produce(void* arg);
void* consume(void* arg);
void do_sleep();

sem_t mutex;
sem_t full;
sem_t empty;

pthread_t producer;
pthread_t producer1;
pthread_t consumer;
pthread_t consumer1;


typedef struct product { int spot; char name[10]; } product;

product storage[n];


int main()
{
	
	srand(time(NULL));
	
	//Semaphores initialization
	sem_init(&mutex,0,1);
	sem_init(&full,0,0);
	sem_init(&empty,0,n);
	
	//Initialize our buffer for the products
	product nothing;
	strcpy(nothing.name,"empty");
	for( int i=0; i < n; i++){
		nothing.spot = i;
		storage[i] = nothing;
	}
	
	//Threads initialization
	pthread_create(&producer,NULL,produce,NULL);
	pthread_create(&producer1,NULL,produce,NULL);
	pthread_create(&consumer,NULL,consume,NULL);
	pthread_create(&consumer1,NULL,consume,NULL);
	//
	pthread_join(producer,NULL);
	pthread_join(producer1,NULL);
	pthread_join(consumer,NULL);
	pthread_join(consumer1,NULL);
	
	return 0;
}

void do_sleep(int s)
{
	int t = (rand() % n) + 1;
	sleep(t);
}


product create_product(int spot){
	product new_product;
	new_product.spot = spot;
	int seed = time(NULL);
	strcpy(new_product.name,list_of_products[(int)((double)rand_r(&seed)/RAND_MAX*(5))]); //RAND_MAX = 2147483647
	return new_product;
}



void* produce(void* arg)
{
	while(1){
		
		do_sleep(4);
		printf("Try to produce . . . \n");
		sem_wait(&empty);
		sem_wait(&mutex);
		for( int i = 0 ; i < n ; i++){
			if ( strcmp(storage[i].name, "empty")==0 ) {
				product new_product = create_product(i);
				storage[i] = new_product;
				printf("Produce: storage [");
				for(int k = 0; k < n; k++){
					printf(" %s ", storage[k].name);
				}
				printf("]\n");
				break;
			}
		}
		sem_post(&mutex);
		sem_post(&full);
	
	}
}

void* consume(void* arg)
{
	while(1){
		
		do_sleep(8);
		printf("Try to consume . . . \n");
		sem_wait(&full);
		sem_wait(&mutex);
		for(int i = n-1; i>=0;i--){
			if(strcmp(storage[i].name,"empty")!=0){
				product nothing;
				nothing.spot = i;
				strcpy(nothing.name,"empty");
				storage[i] = nothing;
				printf("Consume: storage [");
				for(int k = 0; k < n; k++){
					printf(" %s ", storage[k].name);
				}
				printf("]\n");
				break;
			}
		}
		
		sem_post(&mutex);
		sem_post(&empty);
	}
}


