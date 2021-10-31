CFLAGS= -Wall -pedantic -std=c11 -I ../utils -L ../lib -g
LIBS=-lutils -lcurl
#I wrote I instead of l 
PROG= indexer testindexio

all: $(PROG)

$(PROG): indexer.c testindexio.c
	gcc $(CFLAGS) $^ $(LIBS) -o $@


clean:
	rm -f *.o $(PROG)
