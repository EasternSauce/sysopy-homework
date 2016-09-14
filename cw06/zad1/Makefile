CCFLAGS=-std=gnu99 -Wall
TARGETS=client server

all: $(TARGETS)

%: %.c
	$(CC) $(CCFLAGS) $< -o $@

.PHONY: clean
clean:
	rm $(TARGETS)
