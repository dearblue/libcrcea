CC=cc
AR=ar
LD=$(CC)

CPPFLAGS=-pedantic
CFLAGS=$(warnings) $(include) $(opti) $(debug) -std=c99 -fPIC
warnings=-Wall -Wno-shift-negative-value -Wno-shift-count-overflow #-Wno-unused-function -Wno-unused-variable -Wno-unused-const-variable
include=-I./include
opti=-O3 -funroll-loops
debug=-g -ggdb
LDFLAGS=

all: lib/libcrcea.a test/basic test/benchmark

clean:
	-@ rm -vf *.[so] */*.[so] */*/*.[so]

test: testbasic

codesize: lib/libcrcea.a examples/static-crc32c/libcrc32c.a
	readelf -s lib/libcrcea.a | grep crc
	readelf -s examples/static-crc32c/libcrc32c.a | grep crc

benchmark: test/benchmark
	test/benchmark

testbasic: test/basic
	test/basic

checkdefs:
	$(CC) -xc -E -dM -DCRCEA_PREFIX=x -DCRCEA_TYPE=uint8_t include/crcea/core.h | grep CRCEA | sort

.PHONY: all clean test codesize benchmark testbasic checkdefs

test/benchmark: test/benchmark.s
	$(LD) $(LDFLAGS) -o test/benchmark test/benchmark.s -lz -llzma

test/basic: test/basic.o lib/libcrcea.a
	$(LD) $(LDFLAGS) -o test/basic test/basic.o lib/libcrcea.a

examples/static-crc32c/libcrc32c.a: examples/static-crc32c/crc32c.o
	$(AR) rc examples/static-crc32c/libcrc32c.a examples/static-crc32c/crc32c.o

lib/libcrcea.a: src/crcea.o
	mkdir -p lib
	$(AR) rc lib/libcrcea.a src/crcea.o

.c.o:
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

.c.s:
	$(CC) $(CPPFLAGS) $(CFLAGS) -S -o $@ $<

cores = include/crcea/core.h include/crcea/defs.h \
	include/crcea/_reference.h include/crcea/_fallback.h include/crcea/_table.h \
	include/crcea/_bitwise.h include/crcea/_bitcombine.h \
	include/crcea/_by_solo.h include/crcea/_by_duo.h include/crcea/_by_quartet.h \
	include/crcea/_by_octet.h include/crcea/_by_sexdectet.h

src/crcea.o: src/crcea.c include/crcea.h $(cores)

examples/static-crc32c/crc32c.o: examples/static-crc32c/crc32c.c $(cores)

test/benchmark.s: test/benchmark.c $(cores)
