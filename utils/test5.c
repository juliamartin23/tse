/* test5.c --- test lapply() from an empty list
 * 
 * 
 * Author: Julia Martin 
 * Created: Fri Oct  1 11:51:24 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

car_t *front=NULL;

static void printcar(car_t *cp) {
	printf("Plate : %s, Price: %f, Year: %d\n", cp->plate, cp->price, cp->year);
	
}

int main(void) {
 	if(front!=NULL){ 
	  lapply(printcar);
	}else{
		printf("Empty list, can't use lapply!");
	}
	exit(EXIT_SUCCESS);
}
