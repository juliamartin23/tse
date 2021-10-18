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
#include <stdbool.h>

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

int main(void){
	//printf("hello\n");
	webpage_t *p1;
	queue_t *qt = qopen();
	webpage_t* page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
	
 	if(webpage_fetch(page)) {
		char *html = webpage_getHTML(page);
		printf("Found html: %s\n", html);
		int pos = 0;
		char *result;
		int depth = 1;
		webpage_t* nextpage;
		
		while ((pos = webpage_getNextURL(page, pos, &result)) > 0) {
			//char *nexthtml = webpage_getHTML(result);
			nextpage = webpage_new(result, depth, NULL);
			//char *nexthtml = webpage_getHTML(nextpage);
			bool intern = IsInternalURL(result);
			printf("Found url: %s", result);
		 	if (intern) {
				printf(" -internal\n");
				qput(qt, (void *)nextpage);
			}
			else {
				printf(" -external\n");	 
				webpage_delete(nextpage);
			}
			free(result);
		}
		
		qapply(qt, printurl);

		do {
		 	p1 = (webpage_t*)qget(qt);
			if (p1!=NULL)
				webpage_delete((void*)p1);
		} while(p1!=NULL);
		
				
		//		webpage_delete(nextpage);
		//	 	qapply(qt, webpage_delete);

 		qclose(qt);
		webpage_delete(page);
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_FAILURE);
	
}
