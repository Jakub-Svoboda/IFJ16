CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -I.
DEPS=scanner.h precedence.h syntax.h htab.h
FILES=main.c precedence.c syntax.c scanner.c 
OBJ=main.o precedence.o scanner.o syntax.o 

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
