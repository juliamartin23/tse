
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

int main(){
    //webpage_t* page = webpage_new("https://thayer.github.io/engs50/", 0, NULL);
    int id = 1;
    char* dirnm = "pages";

    webpage_t *page = pageload(id, dirnm);
    pagesave(page, id, dirnm);
    webpage_t *reloadpage = pageload(id, dirnm);

    printWebpage((void *)page);
    printWebpage((void *)reloadpage);
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