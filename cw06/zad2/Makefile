CCFLAGS=-std=gnu99 -Wall -lrt
TARGETS=client server

all: $(TARGETS)

%: %.c
	$(CC) $(CCFLAGS) $< -o $@

.PHONY: clean
clean:
	rm $(TARGETS)
