
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

static FILE *fp;

typedef struct helement { 
	char word[MAXREG]; 
	queue_t *qt; 
} helement_t; 

typedef struct qelement { 
	int documentid;  
	int count; 
	struct qelement *next; 
} qelement_t; 

helement_t *make_hel2(char *word){
	helement_t *pp = (helement_t*)malloc(sizeof(helement_t)); 
	if(!pp){
		printf("Error: malloc failed allocating element\n");
		return NULL;  
	}
	strcpy(pp->word, word); 
	return pp; 
}

qelement_t *make_qel2(int documentid, int count){
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

void accessQueue(void *elementp) {
    qelement_t *qel= (qelement_t*)elementp;
	char filename[100]; 
    sprintf(filename, "../indexer/indexnm"); 
	//fp = fopen(filename, "a"); 

    fprintf(fp, " %d %d", qel->documentid, qel->count);
}

void accessIndex(void *helement){
	char filename[100]; 
    sprintf(filename, "../indexer/indexnm"); 
	//fp = fopen(filename, "a"); 
    helement_t *hel= (helement_t *)helement;
    fprintf(fp, "%s", hel->word);
	// exit(EXIT_FAILURE);
    queue_t* q = hel->qt;
	qapply(q, accessQueue);
    fprintf(fp, "\n");
}

int32_t indexsave(hashtable_t *htp, char *dirname){
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

hashtable_t* indexload(char* filename) {

	hashtable_t *table = hopen(TABLESIZE); 
    fp = fopen(filename, "r");

    if(fp == NULL){
      //  printf("file is empty \n");
        return NULL; 
    }
	
	char* word; 
	int id;
	int count;
	char s[100]; 

	while(!feof(fp)){
		fgets(s, 100, fp); 
		char* token = strtok(s, " "); //word 
		word = token; 

		helement_t *hel = make_hel2(token); 
		token = strtok(NULL, " "); //first num which is id 

		while(token != NULL){
			id = atoi(token);  
			token = strtok(NULL, " "); //count 
			count = atoi(token); 
			qelement_t *qel = make_qel2(id, count); 
			hel->qt = qel; 
			hput(table, hel, word, strlen(word)); 
			token = strtok(NULL, " "); // increment to next id
		}
	}
	fclose(fp); 
	return table; 
}


	
	 

