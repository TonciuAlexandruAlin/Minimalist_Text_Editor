CC = gcc
CFLAGS = -g -Wall -lm

all: clean build

build: 
	$(CC) -o editor tema_1.c $(CFLAGS)

.PHONY: clean
clean: 
	rm -f editor
	rm -f *.out
