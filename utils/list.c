/* list.c --- 
 * 
 * 
 * Author: Julia L. Martin
 * Created: Fri Oct  1 14:52:14 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */

#include "list.h"
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>

static car_t *front = NULL;

int32_t lput(car_t *cp){
	
	cp->next = front;
	front = cp;
	return 0;

}

car_t *lget(){
	car_t *p= NULL;

	if(front != NULL){
		p=front;
		front=front->next;
	}
	return p;
}

void lapply(void (*fn)(car_t *cp)){

car_t *cp;

	for(cp=front;cp!=NULL;cp=cp->next){
		fn(cp);
	}
}

car_t *lremove(char *plate){
	if(plate == NULL || front == NULL) {
		return NULL;
	}

	car_t *p = front;
	car_t *p2 = NULL;
	//check first car
	if(strcmp(front->plate, plate) == 0){
		p2=front;
		front=front->next;
        return p2;
	}

	while(p!=NULL){
		p=p->next;
		p2=p->next;
		if(strcmp(p2->plate, plate) == 0){
			p->next=p2->next;
			return p2;
		}
		
	}
	return NULL;
}
