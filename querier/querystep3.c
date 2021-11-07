
/* query.c --- 
 * 
 * 
 * Author: Julia L. Martin
 * Created: Tue Nov  2 10:52:58 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>

#include <indexio.h>
#include <queue.h>
#include <hash.h>
#include <webpage.h>
#include <pageio.h>

#define MAXREG 100 


typedef struct helement { 
	char word[MAXREG]; 
	queue_t *qt; 
} helement_t; 

typedef struct qelement { 
	int documentid;  
	int count; 
	struct qelement *next; 
} qelement_t; 

helement_t *make_hel3(char *word){
	helement_t *pp = (helement_t*)malloc(sizeof(helement_t)); 
	if(!pp){
		printf("Error: malloc failed allocating element\n");
		return NULL;  
	}
	strcpy(pp->word, word); 
	return pp; 
}

qelement_t *make_qel3(int documentid, int count){
	qelement_t *pp = (qelement_t*)malloc(sizeof(qelement_t)); 
	if(!pp){
		printf("Error: malloc failed allocating element\n");
		return NULL;  
	}
	pp->next = NULL; 
	pp->count = count; 
	pp->documentid = documentid; 
	return pp; 
}
// static void printqel(void *p) {
// 	qelement_t* qel = (qelement_t *) p; 
// 	printf("id: %d, count: %d\n", qel->documentid, qel->count); 
// }

bool searchpage2 (void *elementp, const void *keyp){
	helement_t* element = elementp; 
	//printf("word: %s\n", element->word); 
	if(strcmp(element->word, keyp) == 0) {
		return true;
	} 
	return false; 
}
bool searchfn2 (void *elementp, const void *keyp){
    qelement_t* qp = (qelement_t *)elementp;
    int* key = (int *)keyp;
    return((qp->documentid) == *(int*) key);
}

void freeq(void *elementp){
 	helement_t* hel = (helement_t *)elementp;
 	qclose(hel->qt);

}



void rankfn(queue_t *qt){
   
   qelement_t * qel;
   qelement_t * qel2;
   int id;
   int count;
   int mincount=100;
   char *dirnm = "../pages-depth3";

   while((qel= qget(qt))!=NULL){
      id= qel->documentid;
      count= qel->count;
      mincount=count;
      while((qel2= qremove(qt,searchfn2, &id))!=NULL){
         if (qel2->count < mincount){
            mincount=qel2->count;
         }
         free(qel2);
      }
      webpage_t *page = pageload(id, dirnm);
      char *url = webpage_getURL(page);
      printf("rank: %d doc: %d url: %s\n", mincount, id, url);
      webpage_delete(page);
      free(qel);
   }
   //return 0;
}



int main(void) {

   
   const char * printArray2[2] = {"coding","course"};
   //somewhere in here we have to compare ranks.... 
   //im not exactly sure where that would be but yeah
   //char* filename= "depth0Indexnm";
   char* filename= "indexnm2";
   hashtable_t *htable = indexload(filename);
   queue_t *qt;
   queue_t *qIdsWithWord;
   queue_t *outputQueue = qopen();

   for(int i=0; i<2; i++) {
      const char *word = printArray2[i];
      if(hsearch(htable,searchpage2, word, strlen(word))){
            helement_t *hel = hsearch(htable, searchpage2, word, strlen(word)); 
				//queue associated with the word
            qt = hel->qt; 
				
            //if we are in the first iteration copy all things from the word queue to our new queue
            if (i==0) {
               //is this allowed? yes 
               qIdsWithWord = qt;
               //qapply(qIdsWithWord,printqel);
            }
            else {
            
            //for second, third words etc check through all ids and only keep 
            //how to get the ids from the new queue I created?
            //maybe i can qget them if i qput them back in?
            qelement_t *qel;
            qelement_t *qelFound;
            // printf("i=%d\n", i);
            // printf("before first while\n");
            // qapply(qIdsWithWord,printqel);
            // printf("qt before first while\n");
            // qapply(qt,printqel); 
            while( (qel=qget(qt))!=NULL){
               if((qelFound = qsearch(qIdsWithWord, searchfn2, &qel->documentid)) != NULL ) { 
                  //remove the current listing
                  qremove(qIdsWithWord, searchfn2, &qel->documentid);
                  //re-add so you know its in the first and iteratively until this word
                  qput(outputQueue, qelFound);
                  qput(outputQueue, qel);
                  //qapply(qIdsWithWord,printqel);
               }
               else {
                  free(qel);
                  free(qelFound);
                  //take it out of the queue so our final queue won't contain this or print it out
                  //qremove(qIdsWithWord, searchfn2, &qel->documentid);
               }
            }
            qIdsWithWord = outputQueue;
         }
      }
   }
   //printf("final qapply\n");
   //qapply(outputQueue,printqel); 
   //qclose(qIdsWithWord); 
   rankfn(outputQueue);
   
   happly(htable, freeq); 
	hclose(htable);
   qclose(outputQueue);
   return 0;
   
}               
