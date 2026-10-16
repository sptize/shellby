#Makefile - shellby
CC ?= gcc
CFLAGS ?= -Wall -Werror -Wextra -pedantic -std=gnu11 -O2 -g -D_GNU_SOURCE
LDFLAGS ?= -pthread
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
BIN = shellby

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

build/%.o: src/%.c src/shell.h | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

test: $(BIN)
	sh tests/run_tests.sh

clean:
	rm -rf build $(BIN)

.PHONY: all test clean