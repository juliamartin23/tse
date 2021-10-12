CFLAGS=-Wall -pedantic -std=c11 -I. -g

all: test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11 test12 test13 test15 test16 test17

%.o:		%.c %.h
				gcc $(CFLAGS) -c $<

test1:	list.o test1.o
				gcc $(CFLAGS) list.o test1.o -o test1

test2:	list.o test2.o
				gcc $(CFLAGS) list.o test2.o -o test2

test3:	list.o test3.o
				gcc $(CFLAGS) list.o test3.o -o test3

test4:  list.o test4.o
				gcc $(CFLAGS) list.o test4.o -o test4

test5:	list.o test5.o
				gcc $(CFLAGS) list.o test5.o -o test5

test6:	list.o test6.o

				gcc $(CFLAGS) list.o test6.o -o test6

				gcc $(CFLAGS) list.o test7.o -o test7

test8:  list.o test8.o
				gcc $(CFLAGS) list.o test8.o -o test8

test9:  list.o test9.o
				gcc $(CFLAGS) list.o test9.o -o test9

test10: list.o test10.o
				gcc $(CFLAGS) list.o test10.o -o test10

test11: queue.o test11.o
				gcc $(CFLAGS) queue.o test11.o -o test11

test12: queue.o test12.o
				gcc $(CFLAGS) queue.o test12.o -o test12 

test13: queue.o test13.o
				gcc $(CFLAGS) queue.o test13.o -o test13

test15: queue.o test15.o
				gcc $(CFLAGS) queue.o test15.o -o test15

test16: queue.o test16.o
				gcc $(CFLAGS) queue.o test16.o -o test16

test17: hash.o queue.o test17.o
				gcc $(CFLAGS) hash.o queue.o test17.o -o test17 

clean:
				rm -f *.o test1 test2 test3 test4 test5 test6 test7 test8 test9 test10 test11 test12 test13 test15 test16 test17
