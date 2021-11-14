#include <string.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <lockedqueue.h> 
#include <queue.h>
#include <lockedhash.h>
#include <hash.h> 



typedef struct lhashtable {
    pthread_mutex_t mutex; 
    hashtable_t *table;  
}lhashtable_t;	


lhashtable_t *lhopen(uint32_t hsize){
    lhashtable_t* lhtable = (lhashtable_t*)malloc(sizeof(lhashtable_t)); 
    pthread_mutex_init(&(lhtable->mutex),NULL); 
    pthread_mutex_lock(&(lhtable->mutex)); 
    hashtable_t *htable = hopen(); 
    lhtable->table = htable; 
    pthread_mutex_unlock(&(lhtable->mutex)); 
    return lhtable; 
}

void lhclose(lhashtable_t *htp){
    pthread_mutex_lock(&(htp->mutex)); 
    hclose(htp->table); 
    pthread_mutex_unlock(&(htp->mutex)); 
    pthread_mutex_destroy(&(htp->mutex)); 
    free(htp); 
}

int32_t lhput(lhashtable_t *htp, void *ep, const char *key, int keylen){
    pthread_mutex_lock(&(htp->mutex)); 
    int value = hput(htp->table, ep, key, keylen); 
    pthread_mutex_unlock(&(htp->mutex)); 
    return value; 
}

void lhapply(lhashtable_t *htp, void (*fn)(void* ep)){
    pthread_mutex_lock(&(htp->mutex)); 
    happly(htp->table, fn); 
    pthread_mutex_unlock(&(htp->mutex)); 
}


void *lhsearch(lhashtable_t *htp, bool (*searchfn)(void* elementp, const void* searchkeyp), const char *key, int32_t keylen){
    pthread_mutex_lock(&(htp->mutex)); 
    void* element = qsearch(htp->table, searchfn, key, keylen); 
    pthread_mutex_unlock(&(htp->mutex)); 
    return element;
}



