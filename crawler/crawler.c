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
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

#include <webpage.h>
#include <queue.h>
#include <hash.h>


#define TABLESIZE 10 

//static void printurl(void *cp) {
//	webpage_t* p = (webpage_t *)cp;
//	printf("Url : %s\n", webpage_getURL(p));
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


int32_t pagesave(webpage_t *pagep, int id, char *dirname){
	FILE *fp;
	char filename[100]; 

	sprintf(filename, "../%s/%d.txt", dirname, id); 
	fp = fopen(filename, "w"); 
	if(fp == NULL){ 
		return 2; 
	}
	
	char* url = webpage_getURL(pagep); 
	int depth = webpage_getDepth(pagep); 
	char* html = webpage_getHTML(pagep); 
	int len = webpage_getHTMLlen(pagep); 

	fprintf(fp, "%s\n%d\n%d\n%s", url, depth, len, html); 

	fclose(fp); 

	return 0; 
}


int main(int argc, char *argv[]){
	webpage_t *p1;
	queue_t *qt = qopen();
	hashtable_t *table = hopen(TABLESIZE); 
	
	char* seedurl = argv[1]; 
	char* pagedir = argv[2]; 
	int maxdepth = atoi(argv[3]); 
	webpage_t *page = webpage_new(seedurl, 0, NULL);



	if(argc != 4){
		printf("usage: not enough arguments\n"); 
		exit(EXIT_FAILURE); 
	}

	if(maxdepth < 0) {
		printf("usage: maxdepth non-negative\n"); 
		exit(EXIT_FAILURE); 
	}


 		char *resulturl;
		int id = 1; 
		int pos = 0; 
		
		qput(qt, page); 

		
		while(qt != NULL){
			webpage_t *currpage = qget(qt); 

 			if(webpage_fetch(currpage)) {
				//char *html = webpage_getHTML(currpage);
				//printf("Found html: %s\n", html);
				pos=0;

				pagesave(currpage, id, pagedir); 
				id++;
			
				webpage_t* nextpage;
			//check if currpage is in hashtable 
		
				int currdepth = webpage_getDepth(currpage) + 1; 

				while ((pos = webpage_getNextURL(currpage, pos, &resulturl)) > 0 && currdepth <= maxdepth){			
					bool intern = IsInternalURL(resulturl);
					nextpage= webpage_new(resulturl, currdepth, NULL);
					printf("Found url: %s", resulturl);
					if (intern) {
						printf(" -internal\n");
						//qput(qt, (void *)nextpage);
						if (!hsearch(table,searchfn,(void *)resulturl, strlen(resulturl))) {
							qput(qt, (void *)nextpage);
							hput(table,(void *)resulturl, (void *)resulturl, strlen(resulturl));
							//printf("hput happened");	
							//printurl((void*)nextpage);
						}
						else {
							free(resulturl);
						}
					}
					else {
						printf(" -external\n");	 
						webpage_delete(nextpage);
					}
				}
				
			}
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
		
		if(qt==NULL){
			exit(EXIT_SUCCESS);
		}
	exit(EXIT_FAILURE);
}
