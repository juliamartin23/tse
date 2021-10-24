CFLAGS= -Wall -pedantic -std=c11 -I ../utils -L ../lib -g
LIBS=-lutils -lcurl
#I wrote I instead of l 

crawler: crawler.c
	gcc $(CFLAGS) crawler.c $(LIBS) -o $@

crawler1: crawler1.c
	gcc $(CFLAGS) crawler1.c $(LIBS) -o $@

testpageio: testpageio.c
	gcc $(CFLAGS) testpageio.c $(LIBS) -o $@


clean:
	rm -f *.o crawler
