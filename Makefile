CC=gcc
CFLAGS= -Wall -Wextra -std=c99 -pedantic -lm -I.
DEPS = ifj2017.h string_ops.c scanner.c symtable.c parser.c psa_parser.c psa_stack.c psa_tac.c
OBJ = string_ops.o scanner.o symtable.o parser.o psa_parser.o psa_stack.o psa_tac.o

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

ifj2017: $(OBJ)
	gcc -g -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o ifj2017

.PHONY:test

test:
	./tests/tttest
