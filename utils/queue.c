/* queue.c --- Implements queue functions
 * 
 * 
 * Author: Hannah M. Gaven
 * Created: Fri Oct  1 14:52:14 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "queue.h"     

/* creates a structure of the "real element type"
which is the element within the real queue; each element has a next 
pointer (which points to the next element in the queue) and has a pointer 
to the information/structure stored in this element, which is void 
*/

typedef struct relementstruct{
    struct relementstruct *next; 
    void *element; 
} relement_t;

typedef struct rq {
    relement_t *front; 
    relement_t *back; 
}rq_t;

relement_t *make_element(relement_t *next_el, void *el){
    relement_t *rep; 
    rep = (relement_t *)malloc(sizeof(relement_t)); 
    rep->next = next_el; 
    rep->element = el; 
    return rep; 
}



queue_t* qopen(void){
	rq_t* p= (rq_t*)malloc(sizeof(rq_t));
	if (!p){
		printf("Error!");
		return NULL;
	}else{
		p->front = NULL; 
		p->back = NULL; 
		return (queue_t *)p; 
	}
}

void qclose(queue_t *qp){
    rq_t *rqp = (rq_t *)qp;
    relement_t *p = rqp->front;
    while(p != NULL) {
        relement_t *temp = p->next;
        //need to free void element pointer
        void *el = p->element; 
        free(el); 
		free(p);
        p=temp;
    }
	free(rqp);
}


int32_t qput(queue_t *qp, void *elementp){
    rq_t *rqp = (rq_t *)qp; 
		relement_t *end = rqp->back;
		relement_t *new;
		
		if(rqp->back ==NULL){
		  new = make_element(rqp->front,elementp);
			rqp->back= new;
			rqp->front=new;
		}else{
			relement_t *end = rqp->back; 
			new = make_element(end->next,elementp); 
			end->next= new;
			rqp->back= end->next;
		}
		if(end != new){
				return 1;
			}
		return 0;
}

void* qget(queue_t *qp){
    rq_t *rqp = (rq_t *)qp;
    relement_t *p = rqp->front;
    relement_t *pp = NULL;

	if(p!= NULL){
		pp= p;
		p=p->next;
		rqp->front=p;
	}
	else {
		pp=p;
		free(pp);		
		return NULL;
	}
	void* data = pp->element;
	free(pp);
	return data;
}

void qapply(queue_t *qp, void (*fn)(void* elementp)){
	rq_t *rqp = (rq_t *)qp;
	relement_t *elementp = rqp->front;
	
	while (elementp!=NULL){
		fn(elementp->element);
		elementp=elementp->next;
	}
}

void* qsearch(queue_t *qp, bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp){
	rq_t *rqp = (rq_t *)qp;
	relement_t *elementp = rqp->front;
	
	while(elementp != NULL) {
		if (searchfn((void *)elementp->element,skeyp)){
			printf("Element found!\n");
			return(void *)elementp->element; 
		}
		elementp=elementp->next;  
	}

	return NULL;   
}

void* qremove(queue_t *qp,bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp) {
	rq_t *rqp = (rq_t *)qp;											 
  void* p_element=NULL;
	
	//if (!(p = (relement_t*)malloc(sizeof(relement_t)))){
	//printf("Error!");
	//return NULL;  
	//}
	if (qp== NULL || rqp->front==NULL || searchfn== NULL || skeyp ==NULL){
		printf("List may be empty.. Cant remove\n");
		return NULL;
	}
	
	//first item on the queue
	if (searchfn(rqp->front->element, skeyp)){
		relement_t *p2=rqp->front;
		p_element= rqp->front->element;
		rqp->front=rqp->front->next;
		free(p2);
	}else{

		for(relement_t *p2=rqp->front; p2->next!=NULL;){
			if (searchfn(p2->next->element, skeyp)){
				printf("Element found!Removing it\n");
				p_element= p2->next->element;
				relement_t *temp= p2->next;
				p2->next=temp->next;
				free(temp);
			}else{
				p2= p2->next;
			}
		}
	}
	return p_element;                                                                             
}

void qconcat(queue_t *q1p, queue_t *q2p){
	rq_t *rqp1 = (rq_t *)q1p; 
	rq_t *rqp2 = (rq_t *)q2p; 
	relement_t *back1 = rqp1->back; 

	if (rqp1->front == NULL) {
		rqp1->front = rqp2->front; 
		rqp1->back = rqp2->back;
		rqp2->front = NULL; 
		rqp2->back = NULL;
		free(rqp2);
	}
	else if (rqp2->front != NULL) { 
		back1->next = rqp2->front;
		rqp1->back = rqp2->back; 
		rqp2->front = NULL; 
		rqp2->back = NULL; 
		free(rqp2); 
		//free(rqp2);
	}
	else if (rqp2->front == NULL) { 
		free(rqp2); 
	}
}





