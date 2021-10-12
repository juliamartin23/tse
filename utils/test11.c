/* test11.c --- testing qget implicity test qput
 * --> should pass as there are things to get
 * --> tests qput both for putting something on an empty list
 * and for putting somethingona list with other things
 * 
 * Author: Ezgi Okutan
 * Created: Thu Sep 30 21:34:08 2021 (-0400)
 * Version: 
 * 
 * Description: This is a test file that tests if we can put to an empty list
 * 
 */

/* test1.c --- 
 * 
 * 
 * Author: Ezgi Okutan
 * Created: Thu Sep 30 21:20:51 2021 (-0400)
 * Version: 
 * 
 * Description: This is a test file. It tests if we can put to an empty list 
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

	printf("Adding three cars...\n");
	qput(queuep,(void *)p1);
	qput(queuep,(void *)p2);
	qput(queuep,(void *)p3);

	printf("\nUsing qapply to print the list:\n");
	qapply(queuep, printcar);
	printf("....\n");

	printf("\nUsing qget to get first element of the list:\n");
	void *carp1= qget(queuep);
	car_t* cp1 = (car_t *)carp1;
	printf("Front car we get is:\n");
	printcar(cp1);
	printf("\n\n");
	printf("Rest of the list:\n");
	qapply(queuep, printcar);   
	
	if(cp1!=NULL){
		qclose(queuep);
		free(cp1);
		//	free(p1);
		//	free(p2);
		//	free(p3);
		exit(EXIT_SUCCESS);
	}
	qclose(queuep);
	free(cp1);
	//	free(p1);
	//	free(p2);
	//free(p1);
	qclose(queuep);
	exit(EXIT_FAILURE);
}
