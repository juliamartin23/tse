CFLAGS= -Wall -pthread -pedantic -std=c11 -I ../utils -L ../lib -g
LIBS=-lutils -lcurl
#I wrote I instead of l 
PROG= indexer2

all: $(PROG)

$(PROG): indexer2.c
	gcc $(CFLAGS) $^ $(LIBS) -o $@


clean:
	rm -f *.o $(PROG)