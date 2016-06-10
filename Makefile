CC=gcc
LLUA=-llua
LMATH=-lm
CFLAGS_EXTRA=-Wall -g -O0
CFLAGS=$(CFLAGS_EXTRA) $(LLUA) $(LMATH)
SCFLAGS=-shared -fPIC
SOBJS=libperlin.so
TESTS=test/libtree_test.c
TESTDEPS=libtree.o


default: all
all: $(SOBJS)

libperlin.so: libperlin.c libperlin.h
	$(CC) $(SCFLAGS) $(CFLAGS) -o $@ $<

.PHONY: clean test
clean:
	-rm $(SOBJS)
	-rm $(TESTDEPS)
	-rm test/test

test: test/test.c $(TESTS) $(TESTDEPS)
	$(CC) $(CFLAGS) -o test/test test/test.c $(TESTDEPS)
	test/test
