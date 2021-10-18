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

//static void printurl(void *cp) {
	//webpage_t* p = (webpage_t *)cp;
	//printf("Url : %s\n", webpage_getURL(p));
//}

static void printelement(void *cp) {
	char* p = (char *)cp;
	printf("Url : %s\n", p);                                                                
}

bool searchfn (void *elementp, const void *keyp){
	char* urlptr = (char*)elementp;
	//printf("plate: %s\n", webpage1->url); 
	if(strcmp(urlptr, keyp) == 0) {
		return true;
	} 
	return false; 
}

int main(void){
	webpage_t *p1;
	queue_t *qt = qopen();
	hashtable_t *table = hopen(TABLESIZE); 
	webpage_t *page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);

 	if(webpage_fetch(page)) {
		char *html = webpage_getHTML(page);
		printf("Found html: %s\n", html);
		int pos = 0;
		char *resulturl;
		int depth = 1;
		webpage_t* nextpage;
		
 		while ((pos = webpage_getNextURL(page, pos, &resulturl)) > 0) {
			//char *nexthtml = webpage_getHTML(result);
			//1			webpage_t* nextpage = malloc(sizeof(webpage_t));
			nextpage= webpage_new(resulturl, depth, NULL);
			//char *nexthtml = webpage_getHTML(nextpage);
			bool intern = IsInternalURL(resulturl);
			printf("Found url: %s", resulturl);
		 	if (intern) {
				printf(" -internal\n");
				qput(qt, (void *)nextpage);
				if (!hsearch(table,searchfn,(void *)resulturl, strlen(resulturl))) {
					hput(table,(void *)resulturl, (void *)resulturl, strlen(resulturl));
				}
				else {
					free(resulturl);
				}
			}
			else {
				printf(" -external\n");	 
				webpage_delete(nextpage);
			}
			//	free(resulturl);
		}

		happly(table, printelement);
		free(resulturl);

		do {
		 	p1 = (webpage_t*)qget(qt);
			if (p1!=NULL)
				webpage_delete((void*)p1);
		} while(p1!=NULL);
 
		webpage_delete(page);    

		qclose(qt);
		hclose(table);
		
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
}
