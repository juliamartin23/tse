
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
static void printqel(void *p) {
	qelement_t* qel = (qelement_t *) p; 
	printf("id: %d, count: %d\n", qel->documentid, qel->count); 
}

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



void rankfn(queue_t *qt, queue_t *temp){

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
      }
      webpage_t *page = pageload(id, dirnm);
      char *url = webpage_getURL(page);
      printf("rank: %d doc: %d url: %s\n", mincount, id, url);
      qput(temp,qel); 
      webpage_delete(page);
   }
   //return 0;
}

int tokenized (char* str, char** printArray){
   char *delimit = " \t\n";
   char *token;
   token = strtok(str, delimit);
   int counter = 0; 

   int j = 0;
   while (token){
      for (int i = 0; i < strlen(token); i++){
         if (!isalpha(token[i])){
            //printf("[invalid query]\n");
            return 0;
         }
         token[i] = tolower(token[i]);
      }
      if (strcmp(token, "\t") != 0 && strcmp(token, " ") != 0){
         printArray[j] = token;
         counter++; 
      }
      token = strtok(NULL, delimit);
      j++;
   }

   if(token){
      if (!strcmp(token, "\n")){
       printf("> ");
    }
   }
   return counter;
}


void helper(queue_t* queue, hashtable_t *htable, char* word){

   if(hsearch(htable,searchpage2, word, strlen(word))){
      helement_t *hel = hsearch(htable, searchpage2, word, strlen(word)); 
      qt = hel->qt; 
      
      if (i==0){
         qIdsWithWord = qt; 
      }
      else {
         qelement_t *qel;
         qelement_t *qelFound;
         
         while((qel=qget(qt))!=NULL){
            if((qelFound = qsearch(qIdsWithWord, searchfn2, &qel->documentid)) != NULL ) { 
               //remove the current listing
               qremove(qIdsWithWord, searchfn2, &qel->documentid);
               //re-add so you know its in the first and iteratively until this word 
               qput(outputQueue, qelFound);
               qput(outputQueue, qel);
            }
            else {
               //take it out of the queue so our final queue won't contain this or print it out
               //qremove(qIdsWithWord, searchfn2, &qel->documentid);
            }
         }
         qIdsWithWord = outputQueue;

      }
   }   
}


int ranking(queue_t *outputQueue, char** printArray, hashtable_t* index, int count){
   queue_t *andq = qopen();
   queue_t *orq = qopen(); 

   for(int i=0; i<count; i++) {
      char *word = printArray[i];

      if(strcmp(word, "or")==0){
         qelement_t *elementp = orq->front; 
         int id = 0; 
         //iterating through the or queue to find the element with the given document id 
         //update the count for that id so it is the sum of cat OR dog ranks 
         while(elementp!=NULL){
            qelement_t *curr = qsearch(andq, searchfn2, id); 
            elementp->count += curr->count; 
            id++; 
         }
      }
      else if(!strcmp(word, "and"){  
         rankfn(outputQueue, andq); //perform rank function to get min value and store it in and queue 
      }
   }
   //just goes through to print out final values of whatever query operations were performed 
   for(int i=0; i<count; i++){
      qelement_t *temp = org->front; 
      printf("doc %d %d\n", temp->id, temp->count); 
   }

   qclose(andq); 
   qclose(orq); 
      
}

int main(void)
{
   char str[MAXREG];
   char *printArray[MAXREG];
   int count = 0; 

   printf("> ");
   char* filename= "indexnm2";
   hashtable_t *htable = indexload(filename);
   
   while((fgets(str, 500, stdin))!=NULL){
      //fgets(str, 100, stdin);
      count = tokenized(str,printArray); 

      if(count==0){
         printf("Invalid query:\n");
         printf("> ");
         continue;
      }

      printf("\b\n");
      printf("> ");

      for(int i=0; i < count; i++){
         printf("%s ", printArray[i]); 
      }
      printf("\n"); 

      queue_t *outputQueue;

      if(ranking(outputQueue, printArray, htable, count)!=0){
         printf("error occurred"); 
         qclose(outputQueue); 
         continue; 
      } 

      printf("final qapply\n");
      qapply(outputQueue,printqel); 
      rankfn(outputQueue);
   
      happly(htable, freeq); 
      qclose(outputQueue);
   }
   hclose(htable);
   
}

