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

typedef struct qelement { 
	char* word; 
	struct qelement *next; 
} qelement_t; 

// static void printelement(void *p){
//     qelement_t* qp = (qelement_t *)p; 
//     printf("%s", qp->word); 
// }


int main(void){

   char str[MAXREG];
   char* token; 
   queue_t* qt; 

   printf("> ");
   //scanf("%s", str);

   qt = qopen(); 

   //while(fgets(str, 80, stdin) != NULL)
   fgets(str,80,stdin); 
   token = strtok(str, " "); 
   qput(qt, token); 
   //printf("put\n"); 
   while(token != NULL){
      // printf("word\n");
       qput(qt,token); 
       token = strtok(NULL, " ");
    }
    printf("DONE\n"); 
   //qapply(qt,printelement); 
   
   free(token); 
   qclose(qt); 

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