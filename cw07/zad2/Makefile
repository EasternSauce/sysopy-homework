CCFLAGS=-std=gnu99 -Wall -lrt -pthread
TARGETS=writer reader

all: $(TARGETS)

%: %.c
	$(CC) $(CCFLAGS) $< -o $@

.PHONY: clean
clean:
	rm $(TARGETS) /dev/shm/pc.c /dev/shm/sem*
