#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <queue.h>
#include <lockedqueue.h>
#include <pthread.h>   
#include <unistd.h>  

#define MAXREG 10

typedef struct car {
	struct car *next;
	char plate[MAXREG];
	double price;
	int year;
} car_t;

typedef struct argstruct {
	lqueue_t *queue;
	car_t *car;
} arg_t;


static void printcar(void *p) {
	car_t* cp = (car_t *)p;
	printf("Plate : %s, Price: %f, Year: %d\n", cp->plate, cp->price, cp->year);
}

car_t *front=NULL;

car_t *make_car(char *plate,double price,int year)  { 
	car_t *pp;
	pp = (car_t*)malloc(sizeof(car_t));
	//if(!(pp = (car_t*)malloc(sizeof(car_t)))) {
		//printf("[Error: malloc failed allocating car]\n");
		//return NULL;
	//}
	pp->next = NULL;
	strcpy(pp->plate,plate);
	pp->price=price;
	pp->year=year;
	return pp;
}

bool searchfn (void *elementp, const void *keyp){
    car_t* cp = (car_t *)elementp;
    int* key = (int *)keyp;
    return(cp->year == *key);
}

int main(void){
  	pthread_t tid1,tid2;
    lqueue_t *lqp = lqopen(); 
    //car_t *queuep= qopen();
    
    car_t *p1 = make_car("1",3000,2005);
	car_t *p2 = make_car("2",3002,2006);
	car_t *p3 = make_car("3",3003,2007);

    lqput(lqp,(void *)p1);
	lqput(lqp,(void *)p2);
	lqput(lqp,(void *)p3);

    lqapply(lqp, printcar);
    printf("....\n");

	// void *carp1= lqget(lqp);
	// car_t* cp1 = (car_t *)carp1;

	// if(cp1==NULL){
	// 	printf("got car\n");
	// 	lqclose(lqp);
	// 	printcar(cp1);
	// 	free(cp1);
	// 	exit(EXIT_FAILURE);
	// }

	// const int key= 2007;  
	// void *foundCar = lqsearch(lqp, searchfn, (void *)&key);
	// car_t* fCar = (car_t *)foundCar;

	// if(fCar==NULL){
	// 	printf("found car\n");
	// 	printcar(fCar);
	// 	lqclose(lqp);
	// 	exit(EXIT_FAILURE);
	// }
	// printf("got car\n");
	// printcar(cp1);
	
	// printf("....\n");

	// printf("found car\n");
	// printcar(fCar);

	// printf("....\n");
	// printf("final queue\n");
	// lqapply(lqp, printcar);

	arg_t *argument;
	car_t *carp5;

	//argument->queue= lqp

	if((pthread_create(&tid1,NULL,lqget,(void *)lqp))!=0) {  
	printf("Thread 1 created:");
	exit(EXIT_FAILURE);             	
}					
	printf("after first thread creation\n");
	lqapply(lqp,printcar);	

	if((pthread_create(&tid2,NULL,lqget,(void *)lqp))!=0) {  
	printf("Thread 2 created:");
	exit(EXIT_FAILURE);             	
}				

	printf("after second thread creation\n");
	lqapply(lqp,printcar);

if((pthread_join(tid1,NULL))!=0) {  
	printf("Thread 1 joined:"); 
	printf("before sleep\n");
	lqapply(lqp,printcar);	
	sleep(3); 
	exit(EXIT_FAILURE);             	
}		
	
	//sleep(3); 
	printf("after sleep\n");
	lqapply(lqp,printcar);

if(((pthread_join(tid2,NULL))!=0)) {   
	printf("Thread 2 joined:");
	exit(EXIT_FAILURE);             	
}		

	printf("final \n");
	lqapply(lqp,printcar);
	lqclose(lqp);
	//free(cp1);
    exit(EXIT_SUCCESS); 
}
