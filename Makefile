CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -I.
DEPS=scanner.h syntax.h error.h
FILES=main.c syntax.c scanner.c error.c
OBJ=main.o syntax.o scanner.o

all: project

project: $(OBJ) $(DEPS)
	$(CC) $(OBJ) -o project -I.

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


clean:
	rm *o project

test: project
	./project test0.java
