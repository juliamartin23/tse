
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

#include <pageio.h>

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

webpage_t *pageload(int id, char *dirnm) {
    char* url; 
    int depth; 
    int len; 
    char* html; 
    char filename[100]; 

    sprintf(filename, "../%s/%d.txt", dirname, id); 
    fp = fopen(filename, "r");

    if(fp == NULL){
        printf("file is empty \n");
        return NULL; 
    }

   fscanf(fp, "%s %d %d %s", url, depth, len, html); 
   webpage_t *page = webpage_new(url, depth, html); 

   return page; 

}

