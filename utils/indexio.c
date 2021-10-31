
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

#include <pageio.h>
#include <webpage.h>
#include <queue.h>
#include <indexio.h>
#include <hash.h>

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
	//printf("%d %d", qel->documentid, qel->count);
    fprintf(fp, " %d %d", qel->documentid, qel->count);
}

void accessIndex(void *helement){
    helement_t *hel= (helement_t *)helement;
    fprintf(fp, "%s", hel->word);
	//printf("%s : ", hel->word);
    queue_t* q = hel->qt;
	qapply(q, accessQueue);
    fprintf(fp, "\n");
	//printf("\n");
}

int32_t indexsave(hashtable_t *htp, char *dirname){
	char filename[100]; 
	//printf("in index save function\n");
	sprintf(filename, "../indexer/%s", dirname); 
	fp = fopen(filename, "w"); 
	if(fp == NULL){ 
		printf("cant open file\n");
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
		//printf("word: %s\n", word);

		helement_t *hel = make_hel2(token); 
		token = strtok(NULL, " "); //first num which is id 

		while(token != NULL){
			id = atoi(token);  
			token = strtok(NULL, " "); //count 
			count = atoi(token); 
			queue_t *qt = qopen();
			qelement_t *qel = make_qel2(id, count); 
			qput(qt, qel);
			//printf("id: %d, count: %d\n", qel->documentid, qel->count);
			hel->qt = qt; 
			hput(table, hel, word, strlen(word)); 
			token = strtok(NULL, " "); // increment to next id
		}
	}
	fclose(fp); 
	return table; 
}


	
	 

