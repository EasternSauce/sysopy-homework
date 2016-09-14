CC = gcc
CFLAGS = -Wall -std=c99

all: server client

server: server.c
	$(CC) $(CFLAGS) $^ -o $@

client: client.c
	$(CC) $(CFLAGS) $^ -o $@ -pthread

.PHONY: clean

clean:
	$(RM) server client
