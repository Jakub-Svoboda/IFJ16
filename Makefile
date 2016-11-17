CC=gcc
CFLAGS=-Wall -Wextra -pedantic -O3 -std=c99 -I.
DEPS=scanner.h precedence.h syntax.h 			htab.h	firstrun.h 	ilist.h	interpret.h garbage.h builtin.h ial.h
FILES=main.c precedence.c syntax.c scanner.c 		htab.c 	firstrun.c	ilist.c	interpret.c garbage.c builtin.c	ial.c
OBJ=main.o precedence.o scanner.o syntax.o 		htab.o 	firstrun.o 	ilist.o	interpret.o garbage.o builtin.o	ial.o

all: project

project: $(OBJ) $(DEPS)
	$(CC) $(OBJ)       -g -o project -I.

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)


clean:
	rm *o project

test: project
	./project ./tests/test0.java

test1: project
	./project ./tests/test.java

test2: project
	./project ./tests/test2.java

test3: project
	./project ./tests/test3.java

test4: project
	./project ./tests/test4.java

cleantest: project
	./project ./tests/test0.java
	rm *o project
zip:    
		
		zip xschmi08.zip builtin.c builtin.h firstrun.c firstrun.h garbage.c garbage.h htab.c htab.h ial.c ial.h ilist.c ilist.h interpret.c interpret.h main.c precedence.c precedence.h scanner.c scanner.h syntax.c syntax.h Makefile