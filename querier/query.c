
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
static void printqel(void *p) {
   if (p != NULL) {
      qelement_t* qel = (qelement_t *) p; 
      int count = qel->count;
      int id= qel->documentid;
      char *dirnm = "../pages-depth3";
      webpage_t *page = pageload(id, dirnm);
      char *url = webpage_getURL(page);
      printf("rank: %d doc: %d url: %s\n", count, id, url);
      webpage_delete(page);
   }
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

   while((qel= qget(qt))!=NULL){ 
      id= qel->documentid;
      count= qel->count;
      mincount=count;
      while((qel2= qremove(qt,searchfn2, &id))!=NULL){
         if (qel2->count < mincount){
            mincount=qel2->count;
         }
      }
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



void unionize(queue_t* outputQueue, queue_t* addingQueue){
   if(addingQueue == NULL){
      return; 
   }

   queue_t* backupQueue = qopen(); 
   qelement_t* currelement; 
   while((currelement = qget(addingQueue))!=NULL){ 
      qelement_t* found = qsearch(outputQueue, searchfn2, &currelement->documentid); 

      if(found != NULL){
         found->count += currelement->count; 
         //qelement_t* newelement = make_qel3(currelement->documentid, currelement->count); 
         //qput(outputQueue, newelement); 
      }
      else{
         //free(found);

         qelement_t* newelement = make_qel3(currelement->documentid, currelement->count); 
         qput(outputQueue, newelement); 
      }
      qput(backupQueue, currelement); 
   }
   if (currelement != NULL){
      qconcat(addingQueue, backupQueue); 
   }
   else{
      //qclose(addingQueue);
      qclose(backupQueue);
      free(currelement);
   }

}

void intersection(queue_t* outputQueue, queue_t* minimizeQueue){
   queue_t* backupQueue = qopen(); 
   qelement_t* currelement; 

   while((currelement = qget(minimizeQueue))!=NULL){
      qelement_t* found = qsearch(outputQueue, searchfn2, &currelement->documentid); 

      if(found != NULL){
         int mincount; 
         if(found->count < currelement->count){
            mincount = found->count; 
         }
         else{
            mincount = currelement->count; 
         } 
         found->count = mincount; 
      }
      else{
         qelement_t* newelement = make_qel3(currelement->documentid, currelement->count); 
         qput(outputQueue, newelement); 
      }
      qput(backupQueue, currelement); 
   }
   qconcat(minimizeQueue, backupQueue); 

}

int ranking(queue_t *outputQueue, char** printArray, hashtable_t* htable, int count){
   if(outputQueue == NULL || printArray == NULL || htable == NULL || count < 1){
      return 1; 
   }

   queue_t *andq = NULL;
   
   for(int i=0; i<count; i++) {
      char *word = printArray[i];

      if(strcmp(word, "or")==0){
         unionize(outputQueue,andq); 
         if(andq != NULL) qclose(andq); 
         andq = NULL; 
      }
      else if(strcmp(word, "and") != 0){ 
         //get from index the associated queue for word 
         helement_t *hel = hsearch(htable, searchpage2, word, strlen(word));
         if(hel== NULL){
            int j = i+1;  
            while(j < count){
               if(strcmp(printArray[j], "or")==0){
                  break; 
               }
               j++; 
            }
            i = j-1; 
         }

         else if(andq == NULL){
            andq = qopen(); 
            unionize(andq, hel->qt); 
         }
         else{
            intersection(andq, hel->qt); 
         }
      }
   }
   if(andq != NULL){
      unionize(outputQueue,andq); 
      qclose(andq); 
      andq = NULL; 
   }
   return 0; 
      
}

int main(int argc, char *argv[]){
   char str[MAXREG];
   char *printArray[MAXREG];
   int count = 0; 

   char* dirnm = argv[1];



   printf("> ");
   char* filename= argv[2];
   FILE *fp;
   char file[100];
	sprintf(file, "../%s/.querier",dirnm);
	fp = fopen(file,"r");
	if (fp== NULL) {
		printf("wrong dir\n");
		return 1;
	}
	fclose(fp);


   
   while((fgets(str, 500, stdin))!=NULL){
      //fgets(str, 100, stdin);
      count = tokenized(str,printArray); 

      if(count==0 || (strcmp(printArray[0],"and") == 0) || (strcmp(printArray[0],"or") == 0)){
         printf("Invalid query\n");
         printf("> ");
         continue;
         //return 0;
      }

      // printf("\n");

      if((strcmp(printArray[count-1],"and") == 0) || (strcmp(printArray[count-1],"or") == 0))
      {
         printf("Invalid query\n");
         printf("> "); 
         continue; 
         //return 0;
      }
      
      for(int i=0; i < count-1; i++){
         if(((strcmp(printArray[i],"and") == 0) && (strcmp(printArray[i+1], "and") == 0)) || 
            ((strcmp(printArray[i],"or") == 0) && (strcmp(printArray[i+1], "or") == 0))){
            printf("Invalid query:\n");
            //printf("> "); 
            printf("> "); 
            continue;
         }

         if(((strcmp(printArray[i],"and") == 0) && (strcmp(printArray[i+1], "or") == 0)) || 
            ((strcmp(printArray[i],"or") == 0) && (strcmp(printArray[i+1], "and") == 0))){
            printf("Invalid query:\n");
            //printf("> "); 
            printf("> "); 
            continue;
         }
       }

      for(int i=0; i < count; i++){
         printf("%s ", printArray[i]); 
      }
      printf("\n"); 
      queue_t *outputQueue = qopen();
      hashtable_t *htable = indexload(filename);


      if(ranking(outputQueue, printArray, htable, count)!=0){
         printf("error occurred\n"); 
         printf(">"); 
         qclose(outputQueue); 
         continue;
         //return 0; 
      } 
      //printf("Final Count: \n"); 
      qapply(outputQueue,printqel); 
      //rankfn(outputQueue);
   

      qclose(outputQueue);
      printf("> ");

   happly(htable, freeq); 
   hclose(htable);
 
   }


   return 0;

   
}

