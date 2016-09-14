CCFLAGS=-std=gnu99 -Wall
TARGETS=producer consumer

all: $(TARGETS)

%: %.c
	$(CC) $(CCFLAGS) $< -o $@

.PHONY: clean
clean:
	rm $(TARGETS)
