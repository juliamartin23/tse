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

int main(void){

   char str[MAXREG];
   char* token; 
   char* printArray[MAXREG];
   char delimit[]=" \t\r\n\v\f"; 

   printf("> ");

   fgets(str,80,stdin); 
   token = strtok(str, delimit); 
   
    
   if(strcmp(token, "\n") == 0){
      printf("> "); 
   }

   int j = 0; 
   while(token != NULL){
      // printf("word\n");
      printf("token: %s\n", token);
      for(int i=0; i< strlen(token) && strcmp(&token[i],"\n") && strcmp(&token[i], "\t"); i++) {
         if (!isalpha(token[i])) {
            printf("[invalid query]");
            return 1;
         }
         token[i] = tolower(token[i]);
         
      }
      //printf("token: %s\n", token);
      printArray[j] = token; 
      token = strtok(NULL, " ");
      j++;
    }
    
    for(int k=0; k<j; k++){
      printf("%s ", printArray[k]);
    }

   return 0;
  
}