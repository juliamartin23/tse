
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

#include <pageio.h>
#include <webpage.h>
#include <queue.h>
#include <indexio.h>

#define TABLESIZE 10 
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

void accessQueue(void *elementp) {
    qelement_t *qel= (qelement_t*)elementp;
    FILE *fp;
	char filename[100]; 
    sprintf(filename, "../indexer/indexnm"); 
	fp = fopen(filename, "a"); 

    fprintf(fp, "%d %d", qel->documentid, qel->count);
}

void accessIndex(void *helement){
    FILE *fp;
	char filename[100]; 
    sprintf(filename, "../indexer/indexnm"); 
	fp = fopen(filename, "a"); 
    helement_t *hel= (helement_t *)helement;
    fprintf(fp, "%s", hel->word);
    queue_t* q = hel->qt;
	qapply(q, accessQueue);
    fprintf(fp, "\n");

}

int32_t indexsave(hashtable_t *htp, char *dirname){
	FILE *fp;
	char filename[100]; 

	sprintf(filename, "../%s/indexnm", dirname); 
	fp = fopen(filename, "w"); 
	if(fp == NULL){ 
		return 2; 
	}
	
    happly(htp, accessIndex); 

	fclose(fp); 

	return 0; 
}

//webpage_t *indexload(int id, char *dirnm) {

//     char url[100]; 
//     int depth; 
//     int len; 
// 		// char*html;


//     FILE *fp; 
//     char filename[100]; 

//     sprintf(filename, "../%s/%d", dirnm, id); 
// 		//    printf("filename: %s\n", filename);
//     fp = fopen(filename, "r");
// 		// printf("file open\n");

//     if(fp == NULL){
//       //  printf("file is empty \n");
//         return NULL; 
//     }

//    fscanf(fp, "%s\n%d\n%d\n", url, &depth, &len); 
// 	 char *html= malloc(sizeof(char)*(len+1));
// 	 int i=0;
//    char c;

// 	 while((c=fgetc(fp)) != EOF){
// 		 html[i]=c;
// 		 i=i+1;
// 	 }
// 	 html[i-1]= '\0';
	 
// 	 //printf("scanned\n");
//    webpage_t *page = webpage_new(url, depth, html); 
//    //printf("created page\n");

// 	 fclose(fp);
	 
//    return page; 

// }

