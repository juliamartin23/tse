/* test1.c --- 
 * 
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

#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void printcar(car_t *cp) {
	printf("Plate : %s, Price: %f, Year: %d\n", cp->plate, cp->price, cp->year);
}

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

int main(void){

	car_t *p1 = make_car("1",3000,2005);
    car_t *p2 = make_car("2",3002,2006);
	lput(p1);
    lput(p2);
    printf("Print pre-existing list of cars\n");
	lapply(printcar);
	printf(".....\n");
    //add another car
    car_t *p3 = make_car("3",3004,2007);
	lput(p3);
    printf("Print updated list of cars\n");
    lapply(printcar);
	//NULL out pointers
	// free memory
	free(p1);
	free(p2);
	free(p3);
	exit(EXIT_SUCCESS);
}