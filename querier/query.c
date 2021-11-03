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

// typedef struct qelement { 
// 	char* word; 
// 	struct qelement *next; 
// } qelement_t; 

// static void printelement(void *p){
//     qelement_t* qp = (qelement_t *)p; 
//     printf("%s", qp->word); 
// }


int main(void){

   char str[MAXREG];
   char* token; 
   char* printArray[MAXREG];
      //queue_t* qt; 

   printf("> ");

   fgets(str,80,stdin); 
   token = strtok(str, " "); 
   printArray[0] = token; 

   int j = 1; 
   while(token != NULL){
      // printf("word\n");
      printf("token: %s\n", token);
      for(int i=0; token[i] != '\0'; i++) {
         if (!isalpha(token[i])) {
            printf("[invalid query]: %c\n", token[i]);
            return 1;
         }
         //printf("else case\n");
         token[i] = tolower(token[i]);
         //printf("token[i] %c\n", token[i]);
         printf("here\n"); 
      }
      //printf("token: %s\n", token);
      printArray[j] = token; 
      token = strtok(NULL, " ");
      j++;
    }
    
    for(int k=0; k<sizeof(printArray); k++){
      printf("THIS"); 
      printf("%s ", printArray[k]);
      printf("\n");
    }

   return 0;
  
}