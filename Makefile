CC=gcc
LLUA=-llua
LMATH=-lm
CFLAGS_EXTRA=-Wall -g
CFLAGS=-shared -fPIC $(CFLAGS_EXTRA) $(LLUA) $(LMATH)
SOBJS=libperlin.so

default: all
all: $(SOBJS)

libperlin.so: libperlin.c libperlin.h
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	-rm $(SOBJS)
