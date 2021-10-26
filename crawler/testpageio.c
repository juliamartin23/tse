
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

#include <pageio.h>
#include <webpage.h>
#include <queue.h>
#include <hash.h>

static void printWebpage(void *cp) {
	webpage_t* p = (webpage_t *)cp;
    printf("Start of page");
	printf("Url : %s\n", webpage_getURL(p));
    printf("Depth : %d\n", webpage_getDepth(p));
    printf("Html : %s\n", webpage_getHTML(p));
}

int main(void){
    //webpage_t* page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
    int id = 1;
    char* dirnm = "pages-depth3";

    webpage_t *page = pageload(id, dirnm);
    pagesave(page, id, dirnm);
    webpage_t *reloadpage = pageload(id, dirnm);

		//    printWebpage((void *)page);
		// printWebpage((void *)reloadpage);
		if (strcmp(webpage_getURL(page), webpage_getURL(reloadpage))==0) printf("same url");
	  else printf("not same url");

		if (strcmp(webpage_getHTML(page), webpage_getHTML(reloadpage))==0) printf("same html");      
		else printf("not same html");
		
		if(webpage_getDepth(page) == webpage_getDepth(reloadpage)) printf("same depth");
		else printf("not same depth");

		if(webpage_getHTMLlen(page)== webpage_getHTMLlen(reloadpage)) printf("same length");
		else printf("not same length");  
		
		webpage_delete(page);
		webpage_delete(reloadpage);
		
		exit(EXIT_SUCCESS);

//     if (strcmp(page, reloadpage) == 0) {
//       printf("The two strings are same\n");
//       exit(EXIT_SUCCESS);
//     }
//    else {
//       printf("The two strings are not same\n" );
//       exit(EXIT_FAILURE);
//     }

}
