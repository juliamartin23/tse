#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <queue.h>
#include <lockedqueue.h>
#include <pthread.h>   
#include <unistd.h>  

#define MAXREG 10

int num = 4; 
int index = 2000; 

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

typedef struct putstruct {
	lqueue_t *queue; 
	int iterator; 
} put_t; 

car_t *make_car(char *plate,double price,int year)  { 
	car_t *pp;
	pp = (car_t*)malloc(sizeof(car_t));
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

static void printcar(void *p) {
	car_t* cp = (car_t *)p;
	printf("Plate : %s, Price: %f, Year: %d\n", cp->plate, cp->price, cp->year);
}

void createthread (pthread_t threadarr[],void*(fn)(void*arg), int number, void* item){

	for(int i=0; i<number; i++){
		int error = pthread_create(&threadarr[i],NULL,fn, item); 
		if(error){
			printf("%d: creation unsuccessful", error); 
			exit(EXIT_FAILURE);   
		}  	      	
	}

	for(int i=0; i<number; i++){
		int error = pthread_join(threadarr[i],NULL); 
		if(error){
			printf("%d: creation unsuccessful", error); 
			exit(EXIT_FAILURE);   
		}  	      	
	}				
}


void* gettingfn(void* arg){
	lqueue_t *lqt = (lqueue_t*) arg; 
	sleep(3); 
	car_t* car; 
	while((car= lqget(lqt))!=NULL){
		printf("Removing plate : %s, Price: %f, Year: %d\n", car->plate, car->price, car->year);
		free(car); 
	}
	return NULL; 
}

void* puttingfn(void* arg){
	lqueue_t *lqt = (lqueue_t*) arg; 
	while(num!=0){
		car_t *car = make_car("plate", 3000, index++); 
		lqput(lqt, car); 
		num--; 
	}
	return NULL; 
}

void* searchingfn(void* arg){
	lqueue_t *lqt = (lqueue_t*) arg; 
	car_t* car; 
	if((car = lqsearch(lqt, searchfn, &index))!=NULL){
		printf("car with year %d found\n", index); 
		printf("Car-> "); 
		printcar(car); 
	} 
	return NULL; 
}

car_t *front=NULL;

int main(void){
	int numt = 2; 
  	pthread_t threadarray[numt]; 
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

	createthread(threadarray,gettingfn, numt,lqp); 

	printf("final \n");
	lqapply(lqp,printcar);
	lqclose(lqp);

	printf("Testing lqput: \n");
	lqueue_t *lqp2 = lqopen(); 
	createthread(threadarray,puttingfn, numt,lqp2); 
	printf("final \n");
	lqapply(lqp2,printcar);
	index = 2001; 
	createthread(threadarray,searchingfn, numt,lqp2);
	lqclose(lqp2);

	//free(cp1);
    exit(EXIT_SUCCESS); 

}

