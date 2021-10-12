//testing hopen in hash.c

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <inttypes.h> 
#include <stdbool.h>
#include "hash.h"
#include "queue.h"

#define TABLESIZE 10 
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
	if(!(pp = (car_t*)malloc(sizeof(car_t)))) {
		printf("[Error: malloc failed allocating car]\n");
		return NULL;
	}
	pp->next = NULL;
	strcpy(pp->plate,plate);
	pp->price=price;
	pp->year=year;
	return pp;
}

bool searchplate (void *elementp, const void *keyp){
	car_t* car = elementp; 
	printf("plate: %s\n", car->plate); 
	if(strcmp(car->plate, keyp) == 0) {
		return true;
	} 
	return false; 
}


int main(void){
	hashtable_t* table = hopen(TABLESIZE); 

	car_t *p1 = make_car("vehicle", 1500, 2002); 
	car_t *p2 = make_car("car", 2000, 2000);
	car_t *p3 = make_car("truck", 3000, 2008);
	hput(table, (void *)p1, "vehicle", strlen("vehicle")); 
	hput(table, (void *)p2, "car", strlen("car")); 
	hput(table, (void *)p3, "truck", strlen("truck")); 


	printf("Printing cars now!! \n"); 
	happly(table, printcar); 

	printf("removing car #3 now\n"); 
	hremove(table,searchplate, "truck", strlen("truck")); 
	printf("new list of cars\n"); 
	happly(table, printcar); 
	hclose(table); 

    exit(EXIT_SUCCESS); 
}
