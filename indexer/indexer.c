/* indexer.c --- 
 * 
 * 
 * Author: Ezgi Okutan
 * Created: Mon Oct 25 21:26:12 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <webpage.h>
#include <pageio.h>
#include <hash.h>
#include <queue.h> 

#define TABLESIZE 10 
#define MAXREG 100 

typedef struct helement { 
	char word[MAXREG]; 
	queue_t *qt; 
} helement_t; 

typedef struct qelement { 
	char documentid[MAXREG]; 
	int count; 
	struct qelement *next; 
} qelement_t; 


helement_t *make_hel(char *word){
	helement_t *pp = (helement_t*)malloc(sizeof(helement_t)); 
	if(!pp){
		printf("Error: malloc failed allocating element\n");
		return NULL;  
	}
	strcpy(pp->word, word); 
	return pp; 
}

qelement_t *make_qel(char *documentid, int count){
	qelement_t *pp = (qelement_t*)malloc(sizeof(qelement_t)); 
	if(!pp){
		printf("Error: malloc failed allocating element\n");
		return NULL;  
	}
	pp->next = NULL; 
	pp->count = count; 
	strcpy(pp->documentid, documentid); 
	return pp; 
}

bool NormalizeWord(char *word){

	for (int i = 0; i < strlen(word); i++) {
		if (!isalpha(word[i])) return false;
		word[i] = tolower(word[i]);
	}
	return true;
}

bool searchpage (void *elementp, const void *keyp){
	helement_t* element = elementp; 
	printf("word: %s\n", element->word); 
	if(strcmp(element->word, keyp) == 0) {
		return true;
	} 
	return false; 
}

bool searchfn (void *elementp, const void *keyp){
    qelement_t* qp = (qelement_t *)elementp;
    int* key = (int *)keyp;
    return((qp->documentid) == key);
}
							
int main(void){

	int id=1;
	char* dirnm= "pages-depth3";
 	webpage_t *page; 
	int pos=0;
	char *word;
	int sum = 0; 

	hashtable_t *htable = hopen(TABLESIZE); 
	

	while((page=pageload(id, dirnm)) != NULL) { 
		while((pos=webpage_getNextWord(page, pos, &word))>0) {
			if (strlen(word) > 2 && NormalizeWord(word)) {
				printf("%s\n", word);

				if(!hsearch(htable,searchpage, word, strlen(word))){
					//make new hash element with "word"
					//open a queue at this "word"
					//create queue element w/current document id, count = 1 
					//qput this queue element into the queue 
					//make queue element next = NULL  
					//put new hash element into hash table 
					helement_t *hel = make_hel(word);
					queue_t *qt = qopen(); 
					qelement_t *qel = make_qel(id, 1); 
					qput(qt, qel);  
					hel->qt = qt; 
					hput(htable, hel, word, strlen(word)); 
					sum++; 
				}
				else {
					//find last element of queue 
					//add an element after last element w/the new id and new count 
					// just make queue element b/c hash element already exists 

					helement_t *hel = hsearch(htable, searchpage, word, strlen(word)); 
					queue_t *qt = hel->qt; 
					
					qelement_t *qel; 
					//if current doc id already exists in the queue, just increment the count 
					if((qel = qsearch(qt, searchfn, id)) != NULL ) { 
						qel->count = qel->count+1; 
					}
					// if there is no queue element with curent doc id, make a new one 
					else{
						qelement_t *qel = make_qel(id, 1); 
						qput(qt, qel); 
					}
				}

			}
			free(word);
			webpage_delete(page); 
		}
		//webpage_delete(page); 
		
		id++; 
	} 

	//happly(table, qclose(hel->qt)); 
	
	//printf("Sum of table is: %d\n", sum); 
	//go through all the queue elements and call qclose(); 
	//for(i=0;i < len(table); i++)){
		
	//}
	hclose(htable); 

	
 
	return 0;
}
