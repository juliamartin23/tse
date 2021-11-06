
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
   char *token;
   char *printArray[MAXREG];
   char *delimit = " \t\n";

   printf("> ");
   fgets(str, 100, stdin);
   token = strtok(str, delimit);

   if (!strcmp(token, "\n"))
   {
      printf("> ");
   }

   int j = 0;
   while (token)
   {
      for (int i = 0; i < strlen(token); i++)
      {

         if (!isalpha(token[i]))
         {
            printf("[invalid query]\n");
            return 1;
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

   for (int k = 0; k < j; k++)
   {
      if (strcmp(printArray[k], "\t"))
      {
         printf("%s ", printArray[k]);
      }
   }
   printf("\b\n");
}

// const char * printArray2[5] = {"examples", "coding", "course", "guide", "join"};

// //somewhere in here we have to compare ranks....
// //im not exactly sure where that would be but yeah
// hashtable_t *htable = indexload(depth0Indexnm);

// queue_t *qIdsWithWord;
// for(int i=0; i<5; i++) {
//    char *word = printArray2[i];
//    if(hsearch(htable,searchpage2, word, strlen(word))){
//          helement_t *hel = hsearch(htable, searchpage2, word, strlen(word));
// 			//queue associated with the word
//          queue_t *qt = hel->qt;

//          //if we are in the first iteration copy all things from the word queue to our new queue
//          if (i==0) {
//             //is this allowed?
//             qIdsWithWord = qt;
//          }
//          //for second, third words etc check through all ids and only keep
//          //how to get the ids from the new queue I created?
//          //maybe i can qget them if i qput them back in?
//          qelement_t *qel = qget(qt);
//          qelement_t *qelFound;
//          if((qelFound = qsearch(qt, searchfn2, &qel->documentid)) != NULL ) {
//             //remove the current listing
//             qremove(qIdsWithWord, searchfn2, &qel->documentid);
//             //re-add so you know its in the first and iteratively until this word
//             qput(qIdsWithWord, qel);
//          }
//          else {
//             //take it out of the queue so our final queue won't contain this or print it out
//             qremove(qIdsWithWord, searchfn2, &qel->documentid);
//          }

// }

// // char  * wordArray[5];
// // int  countArray[5];
// // static FILE *fp;

// // char* filename= "../indexer/depth0Indexnm";
// // //hashtable_t *table = hopen(TABLESIZE);
// //  fp = fopen(filename, "r");

// //  if(fp == NULL){
// //       printf("file is empty \n");
// //      return NULL;
// //  }

// // queue_t qt = qopen();
// // int id;
// // int count;
// // char s[100];
// // int minCount = 100;

// // int inDoc = 0;
// // while(fscanf(fp, "%s", s) == 1)  {
// //    for(int i=0; i<5; i++) {
// //    if(strcmp(s, printArray2[i])==0){

// //    //printf("array[i]:%s\n", printArray[i]);
// //    //printf("s: %s\n", s);
// //    inDoc = inDoc+1;
// // 	while(fscanf(fp, "%d %d", &id, &count) == 2){
// // 		qelement_t *qel = make_qel3(id, count);
// // 		qput(qt, qel);
// //       if (count < minCount) {
// //          minCount = count;
// //       }
// //       //printf("i: %d\n", i);
// //       printf("%s:%d ", s, count);
// //       //strcpy(wordArray[i],s);
// //       countArray[i] = count;

// // 		//printf("id: %d, count: %d\n", qel->documentid, qel->count);
// // 	}
// // 	//hput(table, hel, s, strlen(s));
// //    }
// //    }

// // }

// // for(int k=0; k<5; k++){
// //    //printf("%s :", wordArray[k]);
// //    printf("%d ", countArray[k]);
// // }

// // printf("- %d\n", minCount);
// // if (inDoc != 5) {
// //    printf("[invalid query]\n");
// // }
// // fclose(fp);

// // return 0;
