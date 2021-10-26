
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

#include <pageio.h>
#include <webpage.h>
#include <queue.h>
#include <hash.h>

int32_t pagesave(webpage_t *pagep, int id, char *dirname){
	FILE *fp;
	char filename[100]; 

	sprintf(filename, "../%s/%d", dirname, id); 
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

    char url[100]; 
    int depth; 
    int len; 
		// char*html;


    FILE *fp; 
    char filename[100]; 

    sprintf(filename, "../%s/%d", dirnm, id); 
    printf("filename: %s\n", filename);
    fp = fopen(filename, "r");
    printf("file open\n");

    if(fp == NULL){
        printf("file is empty \n");
        return NULL; 
    }

   fscanf(fp, "%s\n%d\n%d\n", url, &depth, &len); 
	 char *html= malloc(sizeof(char)*(len+1));
	 int i=0;
   char c;

	 while((c=fgetc(fp)) != EOF){
		 html[i]=c;
		 i=i+1;
	 }
	 html[i-1]= '\0';
	 
	 printf("scanned\n");
   webpage_t *page = webpage_new(url, depth, html); 
   printf("created page\n");

	 fclose(fp);
	 
   return page; 

}

