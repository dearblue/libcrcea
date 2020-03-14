CC=cc
AR=ar
LD=$(CC)

CPPFLAGS=-pedantic
CFLAGS=$(warnings) $(include) $(opti) $(debug) $(defs) -std=c99 -fPIC
warnings=-Wall -Wextra -Wno-shift-negative-value -Wno-shift-count-overflow #-Wno-unused-function -Wno-unused-variable -Wno-unused-const-variable
include=-I./include
opti=-O3 -funroll-loops
defs=#-DCRCEA_ONLY_INT64 -DCRCEA_SMALL
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
	$(CC) -xc -E -dM -DCRCEA_ACADEMIC -DCRCEA_PREFIX=x -DCRCEA_TYPE=uint8_t include/crcea/core.h | grep CRCEA | grep -Ev `echo "$(allowmacros)" | sed 's/  */|/g'` | sort

allowmacros = CRCEA_H__ \
	      CRCEA_DEFS_H__ \
	      CRCEA_BEGIN_C_DECL CRCEA_END_C_DECL \
	      CRCEA_VISIBILITY \
	      CRCEA_INLINE \
	      CRCEA_MALLOC \
	      CRCEA_STRIPE_SIZE \
	      CRCEA_MINIMAL \
	      CRCEA_TINY \
	      CRCEA_SMALL \
	      CRCEA_DEFAULT \
	      CRCEA_LARGE \
	      CRCEA_ACADEMIC \
	      CRCEA_DEFAULT_ALGORYTHM \
	      CRCEA_ENABLE_REFERENCE \
	      CRCEA_ENABLE_FALLBACK \
	      CRCEA_ENABLE_BITWISE_CONDXOR \
	      CRCEA_ENABLE_BITWISE_BRANCHASSIGN \
	      CRCEA_ENABLE_BITWISE_BRANCHMIX \
	      CRCEA_ENABLE_BITWISE_BRANCHLESS \
	      CRCEA_ENABLE_BITCOMBINE2 \
	      CRCEA_ENABLE_BITCOMBINE4 \
	      CRCEA_ENABLE_BITCOMBINE8 \
	      CRCEA_ENABLE_BITCOMBINE16 \
	      CRCEA_ENABLE_BITCOMBINE32 \
	      CRCEA_ENABLE_BY_SOLO \
	      CRCEA_ENABLE_BY1_SOLO \
	      CRCEA_ENABLE_BY2_SOLO \
	      CRCEA_ENABLE_BY4_SOLO \
	      CRCEA_ENABLE_BY8_SOLO \
	      CRCEA_ENABLE_BY16_SOLO \
	      CRCEA_ENABLE_BY32_SOLO \
	      CRCEA_ENABLE_BY_DUO \
	      CRCEA_ENABLE_BY1_DUO \
	      CRCEA_ENABLE_BY2_DUO \
	      CRCEA_ENABLE_BY4_DUO \
	      CRCEA_ENABLE_BY8_DUO \
	      CRCEA_ENABLE_BY16_DUO \
	      CRCEA_ENABLE_BY32_DUO \
	      CRCEA_ENABLE_BY_QUARTET \
	      CRCEA_ENABLE_BY1_QUARTET \
	      CRCEA_ENABLE_BY2_QUARTET \
	      CRCEA_ENABLE_BY4_QUARTET \
	      CRCEA_ENABLE_BY8_QUARTET \
	      CRCEA_ENABLE_BY16_QUARTET \
	      CRCEA_ENABLE_BY32_QUARTET \
	      CRCEA_ENABLE_BY1_OCTET \
	      CRCEA_ENABLE_BY2_OCTET \
	      CRCEA_ENABLE_BY4_OCTET \
	      CRCEA_ENABLE_BY8_OCTET \
	      CRCEA_ENABLE_BY16_OCTET \
	      CRCEA_ENABLE_BY32_OCTET \
	      CRCEA_ENABLE_BY2_SEXDECTET \
	      CRCEA_ENABLE_BY4_SEXDECTET \
	      CRCEA_ENABLE_BY8_SEXDECTET \
	      CRCEA_ENABLE_BY16_SEXDECTET \
	      CRCEA_ENABLE_BY32_SEXDECTET

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
