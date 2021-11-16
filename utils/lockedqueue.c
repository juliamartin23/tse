
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "lockedqueue.h"
#include "queue.h"
#include <unistd.h>  


//typedef void queue_t; 
#define MAXREG 10


typedef struct rlqueue {
    pthread_mutex_t mutex; 
    queue_t *qt; 
} rlqueue_t; 

		
lqueue_t* lqopen(void) {
    rlqueue_t* lqp = (rlqueue_t*)malloc(sizeof(rlqueue_t));  
    pthread_mutex_init(&(lqp->mutex),NULL); 
    queue_t *qp = qopen(); 
    lqp->qt = qp; 
    return lqp; 
}      

void lqclose(lqueue_t *qp) {
    rlqueue_t *rqp = (rlqueue_t *)qp;
    pthread_mutex_lock(&(rqp->mutex)); 
    qclose(rqp->qt); 
    //should maybe comment this out
    pthread_mutex_unlock(&(rqp->mutex)); 
    pthread_mutex_destroy(&(rqp->mutex)); 
    free(rqp); 
}   

int32_t lqput(lqueue_t *qp, void *elementp) {
    rlqueue_t *rqp = (rlqueue_t *)qp;
    pthread_mutex_lock(&(rqp->mutex)); 
    int value = qput(rqp->qt, elementp); 
    //sleep(3); 
    pthread_mutex_unlock(&(rqp->mutex)); 
    return value; 
}


void* lqget(lqueue_t *qp) {
    rlqueue_t *rqp = (rlqueue_t *)qp;
    pthread_mutex_lock(&(rqp->mutex)); 
    void* element = qget(rqp->qt); 
    //sleep(waitTime); 
    pthread_mutex_unlock(&(rqp->mutex)); 
    return element; 
}


void lqapply(lqueue_t *qp, void (*fn)(void* elementp)) {
    rlqueue_t *rqp = (rlqueue_t *)qp;
    pthread_mutex_lock(&(rqp->mutex)); 
    qapply(rqp->qt, fn); 
    pthread_mutex_unlock(&(rqp->mutex)); 
}

void* lqsearch(lqueue_t *qp, bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp) {
    rlqueue_t *rqp = (rlqueue_t *)qp;
    pthread_mutex_lock(&(rqp->mutex)); 
    void* element = qsearch(rqp->qt, searchfn, skeyp); 
    pthread_mutex_unlock(&(rqp->mutex)); 
    return element; 
}







