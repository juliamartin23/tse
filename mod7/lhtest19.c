#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <queue.h>
#include <lockedqueue.h>
#include <lockedhash.h> 
#include <pthread.h>   
#include <unistd.h>  

#define TABLESIZE 10  
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


void* puttingfn(void* arg){
	lhashtable_t *table = (lhashtable_t*) arg; 
    char word[10]; 
	while(num!=0){
		car_t *car = make_car("plate", 3000, index++); 
        sprintf(word,"%d",index); 
		lhput(table, car, word,strlen(word)); 
		num--; 
	}
	return NULL; 
}

void* searchingfn(void* arg){ 
    printf("searching"); 
	lhashtable_t *table = (lhashtable_t*) arg; 
	car_t* car; 
    // char word[10];
    // int index = 2003; 
    // sprintf(word,"%d",index); 

	if((car = lhsearch(table, searchfn, "plate", strlen("plate")))!=NULL){
		printf("car with year %s found\n", "plate"); 
		printf("Car-> "); 
		printcar(car); 
	} 
	return NULL; 
}

car_t *front=NULL;

int main(void){
	int numt = 2; 
  	pthread_t threadarray[numt]; 
    lhashtable_t* ltable = lhopen(TABLESIZE);
    //car_t *queuep= qopen();
    
    car_t *p1 = make_car("1",3000,2005);
	car_t *p2 = make_car("2",3002,2006);
	car_t *p3 = make_car("3",3003,2007);

    lhput(ltable,(void *)p1, "1", strlen("1"));
	lhput(ltable,(void *)p2, "2", strlen("2"));
	lhput(ltable,(void *)p3, "3", strlen("3"));

    lhapply(ltable, printcar);
    printf("....\n");

	//createthread(threadarray,gettingfn, numt,ltable); 

	printf("final \n");
	lhapply(ltable,printcar);
	lhclose(ltable);

	printf("Testing lhput: \n");
	lhashtable_t* ltable2 = lhopen(TABLESIZE); 
	createthread(threadarray,puttingfn, numt,ltable2); 
	printf("final \n");
	lhapply(ltable2,printcar);
	index = 2001; 
    printf("Testing search fn: \n");
	createthread(threadarray,searchingfn, numt,ltable2);
	lhclose(ltable2);

	//free(cp1);
    exit(EXIT_SUCCESS); 

}

