CC ?= gcc
PREFIX ?= /usr/local

CFLAGS = -O3 -std=c99 -Wall -Wextra -Ideps -pedantic
LDFLAGS += -lm

SRCS = list.c \
	   list_node.c \
	   list_iterator.c \
	   main.c

OBJS = $(SRCS:.c=.o)

TEST_SRC = $(shell find *.c test/*.c | sed '/rb/d')
TEST_OBJ = ${TEST_SRC:.c=.o}

OUT = ifj18


$(OUT): $(OBJ)
	$(CC) $^ $(SRCS) $(CFLAGS) -o $@


%.o: %.c
	@$(CC) -c $(CFLAGS) $< -o $@


test: test_runner
	@./$<

test_runner: $(TEST_OBJ)
	$(CC) $^ $(LDFLAGS) -o $@

clean:
	rm -f $(OUT) test_runner $(OBJ) $(TEST_OBJ)

.PHONY: test
