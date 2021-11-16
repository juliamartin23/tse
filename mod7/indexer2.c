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
#include <dirent.h>
#include <errno.h>

#include <webpage.h>
#include <pageio.h>
#include <hash.h>
#include <queue.h>
#include <indexio.h>
#include <lockedqueue.h>
#include <lockedhash.h>
#include <pthread.h>
#include <unistd.h>

#define TABLESIZE 10
#define MAXREG 100

static int sum = 0;

typedef struct helement
{
    char word[MAXREG];
    queue_t *qt;
} helement_t;

typedef struct qelement
{
    int documentid;
    int count;
    struct qelement *next;
} qelement_t;

typedef struct russiandoll
{
    int docid;
    char *dirnm;
    lhashtable_t *lht;
} russiandoll_t;


helement_t *make_hel(char *word)
{
    helement_t *pp = (helement_t *)malloc(sizeof(helement_t));
    if (!pp)
    {
        printf("Error: malloc failed allocating element\n");
        return NULL;
    }
    strcpy(pp->word, word);
    return pp;
}

qelement_t *make_qel(int documentid, int count)
{
    qelement_t *pp = (qelement_t *)malloc(sizeof(qelement_t));
    if (!pp)
    {
        printf("Error: malloc failed allocating element\n");
        return NULL;
    }
    pp->next = NULL;
    pp->count = count;
    pp->documentid = documentid;
    return pp;
}

bool NormalizeWord(char *word)
{

    for (int i = 0; i < strlen(word); i++)
    {
        if (!isalpha(word[i]))
            return false;
        word[i] = tolower(word[i]);
    }
    return true;
}

bool searchpage(void *elementp, const void *keyp)
{
    helement_t *element = elementp;
    //printf("word: %s\n", element->word);
    if (strcmp(element->word, keyp) == 0)
    {
        return true;
    }
    return false;
}

bool searchfn(void *elementp, const void *keyp)
{
    qelement_t *qp = (qelement_t *)elementp;
    int *key = (int *)keyp;
    return ((qp->documentid) == *(int *)key);
}

// static void printqel(void *p) {
// 	qelement_t* qel = (qelement_t *) p;
// 	printf("id: %d, count: %d ", qel->documentid, qel->count);
// }

// static void printel(void *p) {
// 	helement_t* hel = (helement_t *) p;
// 	printf("word: %s ", hel->word);
// 	queue_t *rqt = hel->qt;
// 	qapply(rqt, printqel);
// 	printf("\n");
// }

void freeq(void *elementp)
{
    helement_t *hel = (helement_t *)elementp;
    qclose(hel->qt);
}

void sumq(void *elementp)
{
    qelement_t *qel = (qelement_t *)elementp;
    sum = sum + qel->count;
    //printf("this sum: %d \n", sum);
    //qclose(qel);
    //free(qel);
}

void sumwords(void *elementp)
{
    helement_t *hel = (helement_t *)elementp;
    queue_t *q = (queue_t *)hel->qt;

    qapply(q, sumq);
}

void* indexerfn(void *ptr)
{
    russiandoll_t *ptr2= (russiandoll_t*) ptr;
   
    int loop= ptr2->docid;
    char *dirnm= ptr2->dirnm;
    //char *indexnm= ptr2->indexnm; 
    lhashtable_t *lhtable= ptr2->lht;

    webpage_t *page;
    char *word;
    //page=pageload(loop, dirnm);
    pthread_mutex_lock(&lhtable->mutex);
    page = pageload(loop, dirnm);
    pthread_mutex_unlock(&lhtable->mutex);
    while (page != NULL)
    {
        printf("url: %s\n", webpage_getURL(page));
        int pos = 0; 
        while ((pos = webpage_getNextWord(page, pos, &word)) > 0)
        {
            queue_t *qt;

            if (strlen(word) > 2 && NormalizeWord(word))
            {
                //printf("%s\n", word);

                if (!lhsearch(lhtable, searchpage, word, strlen(word)))
                {
                    helement_t *hel = make_hel(word);
                    qt = qopen();
                    qelement_t *qel = make_qel(loop, 1);
                    qput(qt, qel);
                    //qapply(qt, printel);
                    hel->qt = qt;
                    lhput(lhtable, hel, word, strlen(word));
                }
                else
                {
                    helement_t *hel = lhsearch(lhtable, searchpage, word, strlen(word));
                    queue_t *qt = hel->qt;

                    qelement_t *qel;
                    if ((qel = qsearch(qt, searchfn, &loop)) != NULL)
                    {
                        qel->count = qel->count + 1;
                    }
                    // if there is no queue element with curent doc id, make a new one
                    else
                    {
                        qelement_t *qel = make_qel(loop, 1);
                        qput(qt, qel);
                        //printf("in hash, but no doc id in queue\n");
                    }
                }
            }
            free(word);
        }

        pthread_mutex_lock(&lhtable->mutex);
        webpage_delete(page);
        loop++;
        page = pageload(loop, dirnm);
        pthread_mutex_unlock(&lhtable->mutex);
    }
    
    return NULL;
}

int main(int argc, char *argv[])
{

    //int id = atoi(argv[1]);
    int docid =1;
    char *dirnm = argv[1]; //argv[1];
    int threadno = atoi(argv[3]);
    char *indexnm= argv[2];
    russiandoll_t *fnptr=malloc(sizeof(russiandoll_t)+1);
    
    lhashtable_t *lhtable = lhopen(TABLESIZE);
    fnptr->lht= lhtable;

    fnptr->dirnm= malloc(strlen(dirnm)+1);
    strcpy(fnptr->dirnm, dirnm);
    fnptr->docid = docid;

    pthread_t threadarr[threadno];

    for (int i = 0; i < threadno; i++)
    {
        int error = pthread_create(&threadarr[i], NULL, indexerfn, fnptr);
        if (error)
        {
            printf("%d: creation unsuccessful", error);
            exit(EXIT_FAILURE);
        }
    }

    for(int i=0; i<threadno; i++){
		int error = pthread_join(threadarr[i],NULL); 
		if(error){
			printf("%d: creation unsuccessful", error); 
			exit(EXIT_FAILURE);   
		}  	      	
	}		

    lhapply(lhtable, sumwords);
    printf("SUM: %d\n", sum);
    
    indexsave(lhtable, indexnm);
    lhapply(lhtable, freeq);
    lhclose(lhtable);

    free(fnptr->dirnm);
    free(fnptr);

    //FILE *fp;
    //char filename[100];
    //sprintf(filename, "%s/.crawler",dirnm);
    //fp = fopen(filename,"r");
    // if (fp== NULL) {
    // 	printf("wrong dir\n");
    // 	exit(EXIT_FAILURE);
    // }
    //fclose(fp);

    // webpage_t *page;
    // char *word;
    // hashtable_t *htable = hopen(TABLESIZE);
    // //page=pageload(loop, dirnm);

    // while ((page = pageload(loop, dirnm)) != NULL)
    // {
    //     int pos = 0;
    //     while ((pos = webpage_getNextWord(page, pos, &word)) > 0)
    //     {
    //         //printf("id: %d\n", id);
    //         queue_t *qt;

    //         if (strlen(word) > 2 && NormalizeWord(word))
    //         {
    //             //printf("%s\n", word);

    //             if (!hsearch(htable, searchpage, word, strlen(word)))
    //             {
    //                 helement_t *hel = make_hel(word);
    //                 qt = qopen();
    //                 qelement_t *qel = make_qel(loop, 1);
    //                 qput(qt, qel);
    //                 //qapply(qt, printel);
    //                 hel->qt = qt;
    //                 hput(htable, hel, word, strlen(word));
    //             }
    //             else
    //             {
    //                 helement_t *hel = hsearch(htable, searchpage, word, strlen(word));
    //                 queue_t *qt = hel->qt;

    //                 qelement_t *qel;
    //                 if ((qel = qsearch(qt, searchfn, &loop)) != NULL)
    //                 {
    //                     qel->count = qel->count + 1;
    //                 }
    //                 // if there is no queue element with curent doc id, make a new one
    //                 else
    //                 {
    //                     qelement_t *qel = make_qel(loop, 1);
    //                     qput(qt, qel);
    //                     //printf("in hash, but no doc id in queue\n");
    //                 }
    //             }
    //         }
    //         free(word);
    //     }
    //     loop++;
    //     webpage_delete(page);
    // }
    // happly(htable, sumwords);
    // printf("SUM: %d\n", sum);

    // indexsave(htable, indexnm);
    // happly(htable, freeq);
    // hclose(htable);

    //Section 2
    //hashtable_t *htable2 = indexload(indexnm);

    //printf("SUM: %d\n", sum);

    // indexsave(htable2, "indexnm2");
    // happly(htable2, freeq);
    // hclose(htable2);

    return 0;
}
