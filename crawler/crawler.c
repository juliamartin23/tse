/* crawler.c --- 
 * 
 * 
 * Author: Ezgi Okutan (and only Ezgi)
 * Created: Sun Oct 17 01:17:54 2021 (-0400)
 * Version: 
 * 
 * Description: 
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "../utils/webpage.h"
#include "../utils/queue.h"
#include "../utils/list.h"
#include "../utils/hash.h"    
#include <stdbool.h>
#include <string.h>

#define TABLESIZE 10 

typedef struct webpage {
  char *url;                               // url of the page
  char *html;                              // html code of the page
  size_t html_len;                         // length of html code
  int depth;                               // depth of crawl
} webpage_t;

typedef void queue_t;

static void printurl(void *cp) {
	webpage_t* p = (webpage_t *)cp;
	printf("Url : %s\n", webpage_getURL(p));
}


bool searchfn (void *elementp, const void *keyp){
	webpage_t* webpage1 = elementp;
	//printf("plate: %s\n", webpage1->url); 
	if(strcmp(webpage1->url, keyp) == 0) {
		return true;
	} 
	return false; 
}

int main(void){
	//printf("hello\n");
	queue_t *qt = qopen();
	hashtable_t* table = hopen(TABLESIZE); 
	webpage_t* page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
 	if(webpage_fetch(page)) {
		char *html = webpage_getHTML(page);
		printf("Found html: %s\n", html);
		int pos = 0;
		char *result;
		int depth = 1;
		webpage_t* nextpage = malloc(sizeof(webpage_t));  
 		while ((pos = webpage_getNextURL(page, pos, &result)) > 0) {
			//char *nexthtml = webpage_getHTML(result);
			//1			webpage_t* nextpage = malloc(sizeof(webpage_t));
			nextpage= webpage_new(result, depth, NULL);
			//char *nexthtml = webpage_getHTML(nextpage);
			bool intern = IsInternalURL(result);
			printf("Found url: %s", result);
		 	if (intern) {
				printf(" -internal\n");
			 	qput(qt, (void *)nextpage);
				if (!hsearch(table,searchfn,(void *)result, strlen(result))) {
					hput(table,(void *) nextpage, (void *)result, strlen(result));
				}
			}
			else {
				printf(" -external\n");	 
			}
			free(result);
		}

		
		happly(table, printurl);
	 	happly(table, webpage_delete);
		//	qapply(qt, printurl);
		hclose(table);

		//redundant		qapply(qt, webpage_delete);
	 	qclose(qt);
 		webpage_delete(page);
		//		free(page);
		free(nextpage);
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
	
}
