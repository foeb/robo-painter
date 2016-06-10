CC=gcc
LLUA=-llua
LMATH=-lm
CFLAGS_EXTRA=-Wall -g -O0
CFLAGS=$(CFLAGS_EXTRA) $(LLUA) $(LMATH)
SCFLAGS=-shared -fPIC
SOBJS=libperlin.so liblang.so
TESTS=
TESTDEPS=


default: all
all: $(SOBJS)

libperlin.so: libperlin.c libperlin.h
	$(CC) $(SCFLAGS) $(CFLAGS) -o $@ $<

liblang.so: liblang.c liblang.h libperlin.c libperlin.h
	$(CC) $(SCFLAGS) $(CFLAGS) -o $@ $< libperlin.c

.PHONY: clean test
clean:
	-rm $(SOBJS)
	-rm $(TESTDEPS)
	-rm test/test

test: test/test.c $(TESTS) $(TESTDEPS)
	$(CC) $(CFLAGS) -o test/test test/test.c $(TESTDEPS)
	test/test
