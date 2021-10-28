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

static int sum = 0; 

typedef struct helement { 
	char word[MAXREG]; 
	queue_t *qt; 
} helement_t; 

typedef struct qelement { 
	int documentid;  
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

qelement_t *make_qel(int documentid, int count){
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

bool NormalizeWord(char *word){

	for (int i = 0; i < strlen(word); i++) {
		if (!isalpha(word[i])) return false;
		word[i] = tolower(word[i]);
	}
	return true;
}

bool searchpage (void *elementp, const void *keyp){
	helement_t* element = elementp; 
	//printf("word: %s\n", element->word); 
	if(strcmp(element->word, keyp) == 0) {
		return true;
	} 
	return false; 
}

bool searchfn (void *elementp, const void *keyp){
    qelement_t* qp = (qelement_t *)elementp;
    int* key = (int *)keyp;
    return((qp->documentid) == *(int*) key);
}

// static void printel(queue_t *p) {
// 	queue_t* qp = (queue_t *) p; 
// 	printf("qel: %d\n %d\n", qp->documentid, qp->count); 
// }


void freeq(void *elementp){
 	helement_t* hel = (helement_t *)elementp;
 	qclose(hel->qt); 
}
// void freeh(void *elementp){
// 	helement_t* hel = (helement_t *)elementp; 
// 	queue_t *q = (queue_t *) hel->qt; 

// 	qapply(q, freeq); 
// }



void sumq(void *elementp){
	qelement_t* qel = (qelement_t *)elementp; 
	sum = sum + qel->count; 
	//printf("this sum: %d \n", sum); 
	//qclose(qel); 
	//free(qel);

}

void sumwords(void *elementp) {
	helement_t* hel = (helement_t *)elementp; 
	queue_t *q = (queue_t *) hel->qt; 

	qapply(q, sumq); 
}
							
int main(void){

	int id=1;
	char* dirnm= "pages-depth3";
 	webpage_t *page;
	char *word;

	hashtable_t *htable = hopen(TABLESIZE); 
	

	//while((page=pageload(id, dirnm)) != NULL) { 
		page=pageload(id,dirnm); 
		int pos=0;
		while((pos=webpage_getNextWord(page, pos, &word))>0) {
			//printf("id: %d\n", id);
			queue_t *qt;

			if (strlen(word) > 2 && NormalizeWord(word)) {
				//printf("%s\n", word);

				if(!hsearch(htable,searchpage, word, strlen(word))){
					//make new hash element with "word"
					//open a queue at this "word"
					//create queue element w/current document id, count = 1 
					//qput this queue element into the queue 
					//make queue element next = NULL  
					//put new hash element into hash table 
					//printf("word not in hash\n");
					helement_t *hel = make_hel(word);
					qt = qopen(); 
					qelement_t *qel = make_qel(id, 1); 
					qput(qt, qel);  
					//qapply(qt, printel);
					hel->qt = qt; 
					hput(htable, hel, word, strlen(word)); 
					
				}
				else {
					//find last element of queue 
					//add an element after last element w/the new id and new count 
					// just make queue element b/c hash element already exists 

					helement_t *hel = hsearch(htable, searchpage, word, strlen(word)); 
					queue_t *qt = hel->qt; 
					
					qelement_t *qel; 
					//if current doc id already exists in the queue, just increment the count 
					if((qel = qsearch(qt, searchfn, &id)) != NULL ) { 
						qel->count = qel->count+1; 
						//printf("word in hash, id in queue, just increment\n");
					
					}
					// if there is no queue element with curent doc id, make a new one 
					else{
						qelement_t *qel = make_qel(id, 1); 
						qput(qt, qel); 
						//printf("in hash, but no doc id in queue\n");
					}
				}
			}
			free(word);
			//webpage_delete(page); 
		
		}
		//printf("incrementing id\n");
		//id++; 
        webpage_delete(page);
		//free(word);
	
	happly(htable, sumwords); 

	happly(htable, freeq);

	//hclose(htable); 

	printf("SUM: %d\n", sum); 
	hclose(htable); 

	return 0;
}
