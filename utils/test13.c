/* test13.c --- 
 * 
 * 
 * Author: Ezgi Okutan
 * Created: Sat Oct  9 14:06:46 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <inttypes.h> 
#include <stdbool.h>
#include "queue.h" 

#define MAXREG 10 

typedef struct car {
	struct car *next;
	char plate[MAXREG];
	double price;
	int year;
} car_t;


static void printcar(void *p) {
	car_t* cp = (car_t *)p;
	printf("Plate : %s, Price: %f, Year: %d\n", cp->plate, cp->price, cp->year);
}

static void printcars(car_t *cp) {
	printf("Plate : %s, Price: %f, Year: %d\n", cp->plate, cp->price, cp->year);
}

car_t *front=NULL; 

car_t *make_car(char *plate,double price,int year)  { 
	car_t *pp=(car_t*)malloc(sizeof(car_t));
	if(!pp) {
		printf("[Error: malloc failed allocating car]\n");
		return NULL;
	}
	pp->next = NULL;
	strcpy(pp->plate,plate);
	pp->price=price;
	pp->year=year;
	return pp;
}

bool searchfn (void *elementp, const void *keyp){
    car_t* cp = (car_t *)elementp;
    int key =*(int*)keyp;
		
		//printf("Year : %d\n", cp->year);
		// printf("Key: %d\n", key);
		printf("k: %d, y: %d, eq: %d\n", key, cp->year, cp->year==key);
    return(cp->year == key);
}

int main(void){
    car_t *queuep = qopen(); 
    car_t *p1 = make_car("1",3000,2005);
    car_t *p2 = make_car("2",2000,2000);
    car_t *p3 = make_car("3",4000,2006);
    car_t *p4 = make_car("4",3500,2008);
    car_t *p5 = make_car("5",1200,2001);

    qput(queuep,(void *)p1);
    qput(queuep,(void *)p2);
    qput(queuep,(void *)p3);
    qput(queuep,(void *)p4);
    qput(queuep,(void *)p5);
		
    qapply(queuep, printcar);
		printf("\n\n");

    const int key= 2000;
    void* result = qsearch(queuep,searchfn,(void*) &key);

	car_t* cp = (car_t *)result;
	printf("\n\n");	

	if (!cp){
		printf("couldn't find! exiting...");
		exit(EXIT_FAILURE);
	}

	if(cp->year == key){
		printf("Success! Returned right car\n");
		printcar(cp);
	}
	
	//free(p1); 
	//free(p2); 
    //free(p3); 
    //free(p4); 
    //free(p5);
		//free(pp);
		//free(queuep);
	qclose(queuep);
	exit(EXIT_SUCCESS); 
}

