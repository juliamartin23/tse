/* test14.c --- 
 * 
 * 
 * Author: Ezgi Okutan
 * Created: Fri Oct  8 00:20:43 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

int main(void){
  	car_t *queuep= qopen();
	car_t *p1 = make_car("1",3000,2005);
	car_t *p2 = make_car("2",3002,2006);
	car_t *p3 = make_car("3",3003,2007);

  	qput(queuep,(void *)p1);
	qput(queuep,(void *)p2);
	qput(queuep,(void *)p3);
	qapply(queuep, printcar);
	printf("....\n");
	void *carp1= qget(queuep);

	if carp1=NULL {
		qclose(queuep);
		exit(EXIT_SUCCESS);

	}
	car_t* cp1 = (car_t *)carp1;
	//void *carp2= qget(queuep);
	//car_t* cp2 = (car_t *)carp2;
	//void *carp3= qget(queuep);
	//car_t* cp3 = (car_t *)carp3;
	if(cp1!=NULL){
		qclose(queuep);
		//free(p1);
		//free(p2);
		//free(p3);
		exit(EXIT_SUCCESS);
	}
	qclose(queuep);
	//free(p1);
	//free(p2);
	//free(p3);
	exit(EXIT_FAILURE);

}
