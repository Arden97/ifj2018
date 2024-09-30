SRC=./main.c $(wildcard */*.c)
OBJ=${SRC:.c=.o}

CC=clang
PREFIX=/usr/local
CFLAGS=-std=c99 -g -O0 -Wno-parentheses -Wno-switch-enum -Wno-unused-value -w
LDFLAGS+=-lm

# test
TEST_SRC=$(shell find */*.c test/*.c | sed '/ifj18/d')
TEST_OBJ=${TEST_SRC:.c=.o}

CFLAGS+=-I src

# output

OUT=ifj18

$(OUT): $(OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

%.o: %.c
	@$(CC) -c $(CFLAGS) $< -o $@
	@printf "\e[36mCC\e[90m %s\e[0m\n" $@

.PHONY: clean test install uninstall

test: test_runner
	@./$<

test_runner: $(TEST_OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

install: ifj18
	install ifj18 $(PREFIX)/bin

uninstall:
	rm $(PREFIX)/bin/ifj18

clean:
	rm -f ifj18 test_runner $(OBJ) $(TEST_OBJ)
