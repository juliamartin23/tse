
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
      }
      webpage_t *page = pageload(id, dirnm);
      char *url = webpage_getURL(page);
      printf("rank: %d doc: %d url: %s\n", mincount, id, url);
      webpage_delete(page);
   }
   //return 0;
}

bool tokenized (char* str, char** printArray){
   char *delimit = " \t\n";
   char *token;
   token = strtok(str, delimit);

   int j = 0;
   while (token)
   {
      for (int i = 0; i < strlen(token); i++)
      {

         if (!isalpha(token[i]))
         {
            printf("[invalid query]\n");
            return (false);
         }
         token[i] = tolower(token[i]);
      }
      if (strcmp(token, "\t") != 0 && strcmp(token, " ") != 0)
      {
         printArray[j] = token;
      }

      token = strtok(NULL, delimit);
      j++;
   }

   if(token){
      if (!strcmp(token, "\n"))
    {
       printf("> ");
    }
      return(true);
   }
}

// typedef struct helement {
// 	char word[MAXREG];
// 	queue_t *qt;
// } helement_t;

// typedef struct qelement {
// 	int documentid;
// 	int count;
// 	struct qelement *next;
// } qelement_t;

// helement_t *make_hel3(char *word){
// 	helement_t *pp = (helement_t*)malloc(sizeof(helement_t));
// 	if(!pp){
// 		printf("Error: malloc failed allocating element\n");
// 		return NULL;
// 	}
// 	strcpy(pp->word, word);
// 	return pp;
// }

// qelement_t *make_qel3(int documentid, int count){
// 	qelement_t *pp = (qelement_t*)malloc(sizeof(qelement_t));
// 	if(!pp){
// 		printf("Error: malloc failed allocating element\n");
// 		return NULL;
// 	}
// 	pp->next = NULL;
// 	pp->count = count;
// 	pp->documentid = documentid;
// 	return pp;
// }
// static void printqel(void *p) {
// 	qelement_t* qel = (qelement_t *) p;
// 	printf("id: %d, count: %d ", qel->documentid, qel->count);
// }
// bool searchpage2 (void *elementp, const void *keyp){
// 	helement_t* element = elementp;
// 	//printf("word: %s\n", element->word);
// 	if(strcmp(element->word, keyp) == 0) {
// 		return true;
// 	}
// 	return false;
// }
// bool searchfn2 (void *elementp, const void *keyp){
//     qelement_t* qp = (qelement_t *)elementp;
//     int* key = (int *)keyp;
//     return((qp->documentid) == *(int*) key);
// }

int main(void)
{
   char str[MAXREG];
   char *printArray[MAXREG];

   printf("> ");

   while((fgets(str, 500, stdin))!=NULL){

   //fgets(str, 100, stdin);
   
   if (!tokenized(str,printArray)){
      printf("Invalid query:\n");
      printf("> ");
      continue;
   }



   int j=0;
   for (int k = 0; k < j; k++)
   {
      if (strcmp(printArray[k], "\t"))
      {
         printf("%s ", printArray[k]);
      }
   }
   printf("\b\n");
   printf("> ");


    char* filename= "indexnm2";
   hashtable_t *htable = indexload(filename);
   queue_t *qt;
   queue_t *qIdsWithWord;
   queue_t *outputQueue = qopen();

   for(int i=0; i<=sizeof(printArray); i++) {
      const char *word = printArray[i];
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

            while( (qel=qget(qt))!=NULL){
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
   printf("final qapply\n");
   qapply(outputQueue,printqel); 
   rankfn(outputQueue);
   
   happly(htable, freeq); 
	hclose(htable);
   qclose(outputQueue);

   }

  
   //for (int k = 0; k < j; k++)
   //{
   //   if (strlen(printArray[k]>3)
  //    {
   //      strcpy(printArray2[k], printArray[k]);
    //  }
   //}
}

