CC = gcc
CFLAGS = -Wall -std=c99

all: main

main: main.c
	$(CC) $(CFLAGS) $^ -o $@ -pthread

.PHONY: clean

clean:
	$(RM) main
