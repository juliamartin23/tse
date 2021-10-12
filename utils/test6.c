/* test6.c --- lapply() from a non-empty list 
 * 
 * 
 * Author: Julia Martin 
 * Created: 
 * Version: 
 * 
 * Description: this file tests lapply() from list.c on a non-empty list 
 * 
 */

#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

car_t *front=NULL;

car_t *make_car(char *plate,double price,int year)  { 
	car_t *pp;
	pp = (car_t*)malloc(sizeof(car_t));
	pp->next = NULL;
	strcpy(pp->plate,plate);
	pp->price=price;
	pp->year=year;
	return pp;
}

static void printcar(car_t *cp) {
	printf("Plate : %s, Price: %f, Year: %d\n", cp->plate, cp->price, cp->year);
}

int main(void) {
	car_t *p1 = make_car("1",3000,2005);
	car_t *p2 = make_car("2",3002,2006);
	car_t *p3 = make_car("3",3004,2007);
	car_t *p4 = make_car("4",3006,2008);
	car_t *p5 = make_car("5",3008,2009);
	printf("adding five cars to list\n");
	lput(p1);
	lput(p2);
	lput(p3);
	lput(p4);
	lput(p5);
  printf("applied the print car function so that the list of cars should print out");
	lapply(printcar);
	free(p1);
	free(p2);
	free(p3);
	free(p4);
	free(p5);
	exit(EXIT_SUCCESS); 




}
