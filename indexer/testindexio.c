#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

#include <pageio.h>
#include <webpage.h>
#include <queue.h>
#include <hash.h>
#include <indexio.h>
#include <indexer.c>


#define TABLESIZE 10 
#define MAXREG 100 

static int sum = 0; 

typedef struct helement_t; 

typedef struct qelement_t; 
							
int main(int argc, char *argv[]){

	int loop = 1; 
	int id= atoi(argv[1]);
	char* dirnm= "pages-depth3";
 	webpage_t *page;
	char *word;

	hashtable_t *htable = hopen(TABLESIZE); 
	

	while(loop <= id) { 
		page=pageload(loop,dirnm); 
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
					qelement_t *qel = make_qel(loop, 1); 
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
					if((qel = qsearch(qt, searchfn, &loop)) != NULL ) { 
						qel->count = qel->count+1; 
						//printf("word in hash, id in queue, just increment\n");
					
					}
					// if there is no queue element with curent doc id, make a new one 
					else{
						qelement_t *qel = make_qel(loop, 1); 
						qput(qt, qel); 
						//printf("in hash, but no doc id in queue\n");
					}
				}
			}
			free(word);
			//webpage_delete(page); 
		
		}
		//printf("incrementing id\n");
		loop++; 
        webpage_delete(page);
		//free(word);
	}
	happly(htable, sumwords);

    indexsave(htable, "indexer"); 

	happly(htable, freeq);

	//hclose(htable); 

	printf("SUM: %d\n", sum); 
	hclose(htable); 

	return 0;
}

