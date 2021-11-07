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
#include <dirent.h>
#include <errno.h>
#include <queue.h>

#define MAXREG 100 
// typedef struct helement { 
//    char word[MAXREG]; 
//    queue_t *qt; 
// } helement_t; 

typedef struct qelement { 
    int documentid;  
    int count; 
    struct qelement *next; 
 } qelement_t; 

// static void printelement(void *p){
//     qelement_t* qp = (qelement_t *)p; 
//     printf("%s", qp->word); 
// }

// helement_t *make_hel3(char *word){
//    helement_t *pp = (helement_t*)malloc(sizeof(helement_t)); 
//    if(!pp){
//       printf("Error: malloc failed allocating element\n");
//       return NULL;  
//    }
//    strcpy(pp->word, word); 
//    return pp; 
// }

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


int main(void){

   //char str[MAXREG];
   //char* token; 
   const char * printArray[4] = {"coding", "will", "design", "unix"};
   static FILE *fp;

   char* filename= "../indexer/depth0Indexnm";
   //hashtable_t *table = hopen(TABLESIZE); 
    fp = fopen(filename, "r");

    if(fp == NULL){
         printf("file is empty \n");
        return NULL; 
    }

   
   int id;
   int count;
   char s[100]; 
   int minCount = 100;


   while(fscanf(fp, "%s", s) == 1)  {
      for(int i=0; i<4; i++) {
      if(strcmp(s, printArray[i])==0){
      //printf("array[i]:%s\n", printArray[i]);
      //printf("s: %s\n", s);

      while(fscanf(fp, "%d %d", &id, &count) == 2){ 
        //qelement_t *qel = make_qel3(id, count); 
         //qput(qt, qel);
         if (count < minCount) {
            minCount = count;
         }
         printf("%s:%d ", s, count);
         //printf("id: %d, count: %d\n", qel->documentid, qel->count);
      }
      //hput(table, hel, s, strlen(s)); 
      }
      }
   }

   printf("- %d\n", minCount);
   fclose(fp); 
   return 0; 
   

    // char *input = argv[1];
    // for(int i = 0; i < strlen(input); i++) {
    //     if( isalpha(input[i]) ) {
    //     return 1;
    //     }
    // }
   



    // while(token != NULL){

    //     if(isalpha(token) == 0){
    //         printf("[invalid query"); 
    //     }
    //     else{
    //         lowerstr = tolower(atoi(token)); 
    //         printf(">%s", lowerstr); 
    //     }
    //     token = strtok(NULL, " ");
    // }
}
