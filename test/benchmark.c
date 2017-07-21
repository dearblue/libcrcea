/*
 * file:: test/benchmark.c
 * author:: dearblue <https://github.com/dearblue>
 * license:: Creative Commons Zero License (CC0 / Public Domain)
 *
 * CRC-32 のみに限定
 */

#include <stdio.h>
#include <stdint.h>

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
        FILE *fp = fopen("/dev/random", "rb");
        fread(src, 1, size, fp);
        fclose(fp);
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
        s = crc ## SIZE ## _setup(&model, 0);                                            \
        double t1 = ptime();                                                  \
        s = crc ## SIZE ## _update(&model, src, srcend - 1, s, (ALGO), (TABLE));                          \
        double t2 = ptime();                                                  \
        double ti = t2 - t1;                                      \
        double rate = (size) / ti / 1024.0 / 1024.0;                                 \
        s = crc ## SIZE ## _finish(&model, s);                                                   \
        printf("- { throughput: %8.2f MiB / sec., tablesize: %8d, algoright: %s }\n",    \
               rate, (int)crc ## SIZE ## _tablesize(cc.algorithm), #ALGO);            \
    } while (0)                                                                     \

#define MEASURE0(ALGO, TABLE, SIZE) MEASURE00(ALGO, TABLE, SIZE)
#define MEASURE(ALGO, TABLE) MEASURE0(ALGO, TABLE, SIZE)

    puts("%YAML 1.1\n---");

    MEASURE(CRCEA_REFERENCE,       table_s16);
    MEASURE(CRCEA_BITBYBIT,       table_s16);
    MEASURE(CRCEA_BITBYBIT_FAST,  table_s16);
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

    return 0;
}
