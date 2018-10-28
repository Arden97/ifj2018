CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -pedantic -lm -I.
DEPS = scanner.c scanner.h strlib.h strlib.c main.c
OBJ = scanner.o strlib.o main.o

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

ifj2018: $(OBJ)
	gcc -g -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o ifj2018

.PHONY:test

test:
	./tests/tttest
