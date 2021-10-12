//testing qconcat

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

int main(void){
    car_t *queuep = qopen(); 
    //car_t *p1 = make_car("1",3000,2005);
    //car_t *p2 = make_car("2",2000,2000);
    //qput(queuep,(void *)p1);
    //qput(queuep,(void *)p2);
    //qapply(queuep, printcar);

    car_t *queuep2 = qopen();
    car_t *p3 = make_car("3",4000,2006);
    car_t *p4 = make_car("4",3500,2008);
    car_t *p5 = make_car("5",1200,2001);
    qput(queuep2,(void *)p3);
    qput(queuep2,(void *)p4);
    qput(queuep2,(void *)p5); 
    qapply(queuep2, printcar);
    
    qconcat(queuep2, queuep);

    if (queuep2 != NULL) {
        printf("printing queue1\n");
        qapply(queuep2, printcar);
        //qclose(queuep);
        qclose(queuep2);
        exit(EXIT_SUCCESS);
    }

    qclose(queuep);

    exit(EXIT_FAILURE);
}


