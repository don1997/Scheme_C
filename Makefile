sources=$(wildcard *.c)

CFLAGS=-Wall -O0 -g --std=c99 -D_GNU_SOURCE

objects=$(sources:.c=.o)

lisp: $(objects)
	$(CC) $(CFLAGS) -o $@ $^

$(objects): $(wildcard *.h)

.PHONY: clean
clean:
	$(RM) *.o lisp
