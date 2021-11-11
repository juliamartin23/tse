
#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <lockedqueue.h> 
#include <queue.h>

//typedef void queue_t; 


typedef struct lqueue {
    pthread_mutex_t mutex; 
    queue_t *qt; 
} lqueue_t; 
		
lqueue_t* lqopen(void) {
    lqueue_t* lqp = (lqueue_t*)malloc(sizeof(lqueue_t));  
    pthread_mutex_init(&(lqp->mutex),NULL); 
    pthread_mutex_lock(&(lqp->mutex)); 
    queue_t *qp = qopen(); 
    lqp->qt = qp; 
    pthread_mutex_unlock(&(lqp->mutex)); 
    return lqp; 
}      

void lqclose(lqueue_t *qp) {
    pthread_mutex_lock(&(qp->mutex)); 
    qclose(qp->qt); 
    pthread_mutex_unlock(&(qp->mutex)); 
    pthread_mutex_destroy(&(qp->mutex)); 
    free(qp); 
}   

int32_t lqput(lqueue_t *qp, void *elementp) {
    pthread_mutex_lock(&(qp->mutex)); 
    int value = qput(qp->qt, elementp); 
    pthread_mutex_unlock(&(qp->mutex)); 
    return value; 
}


void* lqget(lqueue_t *qp) {
    pthread_mutex_lock(&(qp->mutex)); 
    void* element = qget(qp->qt); 
    pthread_mutex_unlock(&(qp->mutex)); 
    return element; 
}


void lqapply(lqueue_t *qp, void (*fn)(void* elementp)) {
    pthread_mutex_lock(&(qp->mutex)); 
    qapply(qp->qt, fn); 
    pthread_mutex_unlock(&(qp->mutex)); 
}

void* lqsearch(lqueue_t *qp, bool (*searchfn)(void* elementp,const void* keyp),const void* skeyp) {
    pthread_mutex_lock(&(qp->mutex)); 
    void* element = qsearch(qp->qt, searchfn); 
    pthread_mutex_unlock(&(qp->mutex)); 
    return element; 
}







