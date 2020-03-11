/*
 * file:: test/benchmark.c
 * author:: dearblue <https://github.com/dearblue>
 * license:: Creative Commons Zero License (CC0 / Public Domain)
 *
 * CRC-32 のみに限定
 */

#include <stdio.h>
#include <stdint.h>
#include <zlib.h>
#include <lzma.h>

//#include "crcea.h"

//#define CRCEA_VISIBILITY
//#define CRCEA_INLINE
//#define CRCEA_STRIP_SIZE 8
#define CRCEA_ACADEMIC

#define CRCEA_PREFIX      crc8
#define CRCEA_TYPE        uint8_t
#include "../include/crcea/core.h"

#define CRCEA_PREFIX      crc16
#define CRCEA_TYPE        uint16_t
#include "../include/crcea/core.h"

#define CRCEA_PREFIX      crc32
#define CRCEA_TYPE        uint32_t
#include "../include/crcea/core.h"

#define CRCEA_PREFIX      crc64
#define CRCEA_TYPE        uint64_t
#include "../include/crcea/core.h"

void
printtable(uint32_t *table, int mode)
{
    int i, j;
    for (i = 0; i < 2; i ++) {
        printf("0x%04X:", 8 * i);
        for (j = 0; j < 8; j ++, table ++) {
            printf(" 0x%08X", *table);
        }
        puts("");
    }
}

/*
 * "xorshift" the psudo random number generator by George Marsaglia
 */
uint32_t
xorshift32(void)
{
    static uint32_t x = 2463534242ul;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

#ifdef __unix__
#   include <sys/types.h>
#   include <sys/time.h>
#   include <sys/resource.h>

    double
    ptime(void)
    {
        struct rusage ru;
        getrusage(0, &ru);
        return ru.ru_utime.tv_sec + (double)ru.ru_utime.tv_usec / 1000000.0;
    }
#else
#   include <time.h>

    double
    ptime(void)
    {
        return (double)clock() / CLOCKS_PER_SEC;
    }
#endif

int
main(int argc, char *argv[])
{
    //size_t size = 1 << 26;
    size_t size = 1 << 26;
    char *src = malloc(size);
    const char *srcend = src + (size);
    static uint32_t table_s16[32][65536];

    if (1) {
        for (char *p = src; p < srcend; p ++) {
            *p = xorshift32() >> 16;
        }
    }

#define SIZE 32

    static const crcea_model model = {
        .bitsize = SIZE,
        .polynomial = 0x04c11db7ul,
        .reflectin = 1,
        .reflectout = 1,
        .appendzero = 1,
        .xoroutput = ~0ul,
    };

#define MEASURE00(ALGO, TABLE, SIZE)                                     \
    do {                                                                       \
        static crcea_context cc = { \
            .model = &model, \
            .algorithm = (ALGO), \
            .table = (TABLE), \
            .alloc = NULL, \
        }; \
        \
        crc ## SIZE ## _build_table(&model, cc.algorithm, (TABLE));                                    \
        volatile uint32_t s = ~0; /* 最適化によって s が計算されないことを防止する */ \
        double t1 = ptime();                                                  \
        s = crc ## SIZE ## _setup(&model, 0);                                            \
        s = crc ## SIZE ## _update(&model, src, srcend - 1, s, (ALGO), (TABLE));                          \
        s = crc ## SIZE ## _finish(&model, s);                                                   \
        double t2 = ptime();                                                  \
        double ti = t2 - t1;                                      \
        double rate = (size) / ti / 1024.0 / 1024.0;                                 \
        printf("- { throughput: %8.2f MiB / sec., tablesize: %8d, algoright: %s }\n",    \
               rate, (int)crc ## SIZE ## _tablesize(cc.algorithm), #ALGO);            \
        fflush(stdout); \
    } while (0)                                                                     \

#define MEASURE0(ALGO, TABLE, SIZE) MEASURE00(ALGO, TABLE, SIZE)
#define MEASURE(ALGO, TABLE) MEASURE0(ALGO, TABLE, SIZE)

    puts("%YAML 1.1\n---");

    MEASURE(CRCEA_REFERENCE,       table_s16);
    MEASURE(CRCEA_FALLBACK, table_s16);
    MEASURE(CRCEA_BITWISE_CONDXOR, table_s16);
    MEASURE(CRCEA_BITWISE_BRANCHASSIGN, table_s16);
    MEASURE(CRCEA_BITWISE_BRANCHMIX, table_s16);
    MEASURE(CRCEA_BITWISE_BRANCHLESS, table_s16);
    MEASURE(CRCEA_BITCOMBINE2, table_s16);
    MEASURE(CRCEA_BITCOMBINE4, table_s16);
    MEASURE(CRCEA_BITCOMBINE8, table_s16);
    MEASURE(CRCEA_BITCOMBINE16, table_s16);
    MEASURE(CRCEA_BITCOMBINE32, table_s16);
    MEASURE(CRCEA_BY_SOLO, table_s16);
    MEASURE(CRCEA_BY1_SOLO, table_s16);
    MEASURE(CRCEA_BY2_SOLO, table_s16);
    MEASURE(CRCEA_BY4_SOLO, table_s16);
    MEASURE(CRCEA_BY8_SOLO, table_s16);
    MEASURE(CRCEA_BY16_SOLO, table_s16);
    MEASURE(CRCEA_BY32_SOLO, table_s16);
    MEASURE(CRCEA_BY_DUO, table_s16);
    MEASURE(CRCEA_BY1_DUO, table_s16);
    MEASURE(CRCEA_BY2_DUO, table_s16);
    MEASURE(CRCEA_BY4_DUO, table_s16);
    MEASURE(CRCEA_BY8_DUO, table_s16);
    MEASURE(CRCEA_BY16_DUO, table_s16);
    MEASURE(CRCEA_BY32_DUO, table_s16);
    MEASURE(CRCEA_BY_QUARTET, table_s16);
    MEASURE(CRCEA_BY1_QUARTET, table_s16);
    MEASURE(CRCEA_BY2_QUARTET, table_s16);
    MEASURE(CRCEA_BY4_QUARTET, table_s16);
    MEASURE(CRCEA_BY8_QUARTET, table_s16);
    MEASURE(CRCEA_BY16_QUARTET, table_s16);
    MEASURE(CRCEA_BY32_QUARTET, table_s16);
    MEASURE(CRCEA_BY1_OCTET,   table_s16);
    MEASURE(CRCEA_BY2_OCTET,   table_s16);
    MEASURE(CRCEA_BY4_OCTET,   table_s16);
    MEASURE(CRCEA_BY8_OCTET,   table_s16);
    MEASURE(CRCEA_BY16_OCTET,  table_s16);
    MEASURE(CRCEA_BY32_OCTET,  table_s16);
    MEASURE(CRCEA_BY2_SEXDECTET,   table_s16);
    MEASURE(CRCEA_BY4_SEXDECTET,   table_s16);
    MEASURE(CRCEA_BY8_SEXDECTET,   table_s16);
    MEASURE(CRCEA_BY16_SEXDECTET,   table_s16);
    MEASURE(CRCEA_BY32_SEXDECTET,   table_s16);

#define MEASURE_1(LABEL, TABLESIZE, CODE) \
    do { \
        volatile uint32_t s; /* 最適化によって s が計算されないことを防止する */ \
        double t1 = ptime(); \
        s = (CODE); \
        double t2 = ptime(); \
        double ti = t2 - t1; \
        double rate = (size) / ti / 1024.0 / 1024.0; \
        printf("- { throughput: %8.2f MiB / sec., tablesize: %8d, algoright: %s }\n", \
               rate, (int)(TABLESIZE), LABEL); \
        fflush(stdout); \
        (void)s; \
    } while (0) \

    {
        /* pre-running */
        volatile uint32_t x;
        x = crc32(0, (const Bytef *)src, 1001);
        x = lzma_crc32((const uint8_t *)src, 1001, 0);
        (void)x;
    }

    MEASURE_1("zlib crc32", sizeof(uint32_t[4][256]), (uint32_t)crc32(0, (const Bytef *)src, (srcend - 1) - src));
    MEASURE_1("liblzma crc32", sizeof(uint32_t[8][256]), (uint32_t)lzma_crc32((const uint8_t *)src, (srcend - 1) - src, 0));

    return 0;
}
