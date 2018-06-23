CC=gcc
LUA=lua5.1
CFLAGS := -Wall -g -O0
CFLAGS += $(shell pkg-config --cflags lua5.1)
LDFLAGS := -lm
LDFLAGS += $(shell pkg-config  --libs lua5.1)
SCFLAGS=-shared -fPIC -Wl,-E
SOBJS=libperlin.so liblang.so

default: all
all: $(SOBJS)
	mkdir -p $(CURDIR)/images

libperlin.so: libperlin.c libperlin.h
	$(CC) $(SCFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $<

liblang.so: liblang.c liblang.h libperlin.c libperlin.h
	$(CC) $(SCFLAGS) $(CFLAGS) $(LDFLAGS) -o $@ $< libperlin.c

.PHONY: clean run
clean:
	-rm $(SOBJS)

run: all
	$(LUA) robo.lua 1 1149 5 512 512
