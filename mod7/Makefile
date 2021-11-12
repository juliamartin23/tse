CFLAGS= -Wall -pedantic -std=c11 -I ../utils -L ../lib -g
LIBS=-lutils -lcurl
#I wrote I instead of l 
PROG= lqtest18

all: $(PROG)

$(PROG): lqtest18.c
	gcc $(CFLAGS) $^ $(LIBS) -o $@


clean:
	rm -f *.o $(PROG)