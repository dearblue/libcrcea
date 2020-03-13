/*
 * file:: test/basic.c
 * author:: dearblue <https://github.com/dearblue>
 * license:: Creative Commons Zero License (CC0 / Public Domain)
 */

#include <stdio.h>
#include <stdint.h>

#include "../include/crcea.h"


#define ELEMENTOF(L)    (sizeof(L) / sizeof(L[0]))

#define ENDOF(L)        (L + ELEMENTOF(L))

#define FOREACH_LIST(TYPE, I, ...) \
        for (const TYPE items__[] = { __VA_ARGS__ }, \
                        *items_end__ = ENDOF(items__), \
                        *I = items__; \
             I < items_end__; \
             I ++) \


/*
 * Xorshift by George Marsaglia
 */
uint32_t
xor64(void)
{
    static uint64_t x = 88172645463325252ull;
    x = x ^ (x << 13);
    x = x ^ (x >> 7);
    x = x ^ (x << 17);
    return x;
}

static const char *
lookup_algorithm_name(int algo)
{
#define CASE_TO_STRING(N) case N: return #N

    switch (algo) {
    CASE_TO_STRING(CRCEA_FALLBACK);
    CASE_TO_STRING(CRCEA_BITWISE_CONDXOR);
    CASE_TO_STRING(CRCEA_BITWISE_BRANCHASSIGN);
    CASE_TO_STRING(CRCEA_BITWISE_BRANCHMIX);
    CASE_TO_STRING(CRCEA_BITWISE_BRANCHLESS);
    CASE_TO_STRING(CRCEA_BITCOMBINE2);
    CASE_TO_STRING(CRCEA_BITCOMBINE4);
    CASE_TO_STRING(CRCEA_BITCOMBINE8);
    CASE_TO_STRING(CRCEA_BITCOMBINE16);
    CASE_TO_STRING(CRCEA_BITCOMBINE32);
    CASE_TO_STRING(CRCEA_BY_SOLO);
    CASE_TO_STRING(CRCEA_BY1_SOLO);
    CASE_TO_STRING(CRCEA_BY2_SOLO);
    CASE_TO_STRING(CRCEA_BY4_SOLO);
    CASE_TO_STRING(CRCEA_BY8_SOLO);
    CASE_TO_STRING(CRCEA_BY16_SOLO);
    CASE_TO_STRING(CRCEA_BY32_SOLO);
    CASE_TO_STRING(CRCEA_BY_DUO);
    CASE_TO_STRING(CRCEA_BY1_DUO);
    CASE_TO_STRING(CRCEA_BY2_DUO);
    CASE_TO_STRING(CRCEA_BY4_DUO);
    CASE_TO_STRING(CRCEA_BY8_DUO);
    CASE_TO_STRING(CRCEA_BY16_DUO);
    CASE_TO_STRING(CRCEA_BY32_DUO);
    CASE_TO_STRING(CRCEA_BY_QUARTET);
    CASE_TO_STRING(CRCEA_BY1_QUARTET);
    CASE_TO_STRING(CRCEA_BY2_QUARTET);
    CASE_TO_STRING(CRCEA_BY4_QUARTET);
    CASE_TO_STRING(CRCEA_BY8_QUARTET);
    CASE_TO_STRING(CRCEA_BY16_QUARTET);
    CASE_TO_STRING(CRCEA_BY32_QUARTET);
    CASE_TO_STRING(CRCEA_BY1_OCTET);
    CASE_TO_STRING(CRCEA_BY2_OCTET);
    CASE_TO_STRING(CRCEA_BY4_OCTET);
    CASE_TO_STRING(CRCEA_BY8_OCTET);
    CASE_TO_STRING(CRCEA_BY16_OCTET);
    CASE_TO_STRING(CRCEA_BY32_OCTET);
    CASE_TO_STRING(CRCEA_BY2_SEXDECTET);
    CASE_TO_STRING(CRCEA_BY4_SEXDECTET);
    CASE_TO_STRING(CRCEA_BY8_SEXDECTET);
    CASE_TO_STRING(CRCEA_BY16_SEXDECTET);
    CASE_TO_STRING(CRCEA_BY32_SEXDECTET);
    default: return "unknown (test code bug)";
    }
}


int
main(int argc, char *argv[])
{
    int bad = 0;

    char seq[191];
    for (int i = 0; i < sizeof(seq); i ++) {
        seq[i] = xor64() >> 13;
    }

    FOREACH_LIST(int, bitsize, 3, 5, 8, 13, 16, 21, 29, 32, 41) {
        FOREACH_LIST(int, refin, 0, 1) {
            FOREACH_LIST(int, refout, 0, 1) {
                FOREACH_LIST(int, append, 0, 1) {
                    FOREACH_LIST(uint32_t, poly, 0x04C11DB7ul, 0x1EDC6F41ul) {
                        const crcea_design design = {
                            .bitsize = *bitsize,
                            .polynomial = *poly,
                            .reflectin = *refin,
                            .reflectout = *refout,
                            .appendzero = *append,
                            .xoroutput = ~0ul,
                        };

                        crcea_context ref = {
                            .design = &design,
                            .algorithm = CRCEA_REFERENCE,
                            .table = NULL,
                            .alloc = NULL,
                        };

                        uint64_t r = crcea(&ref, seq, seq + sizeof(seq), 0);

                        FOREACH_LIST(int, algo,
                                     CRCEA_FALLBACK,
                                     CRCEA_BITWISE_CONDXOR,
                                     CRCEA_BITWISE_BRANCHASSIGN,
                                     CRCEA_BITWISE_BRANCHMIX,
                                     CRCEA_BITWISE_BRANCHLESS,
                                     CRCEA_BITCOMBINE2,
                                     CRCEA_BITCOMBINE4,
                                     CRCEA_BITCOMBINE8,
                                     CRCEA_BITCOMBINE16,
                                     CRCEA_BITCOMBINE32,
                                     CRCEA_BY_SOLO,
                                     CRCEA_BY1_SOLO,
                                     CRCEA_BY2_SOLO,
                                     CRCEA_BY4_SOLO,
                                     CRCEA_BY8_SOLO,
                                     CRCEA_BY16_SOLO,
                                     CRCEA_BY32_SOLO,
                                     CRCEA_BY_DUO,
                                     CRCEA_BY1_DUO,
                                     CRCEA_BY2_DUO,
                                     CRCEA_BY4_DUO,
                                     CRCEA_BY8_DUO,
                                     CRCEA_BY16_DUO,
                                     CRCEA_BY32_DUO,
                                     CRCEA_BY_QUARTET,
                                     CRCEA_BY1_QUARTET,
                                     CRCEA_BY2_QUARTET,
                                     CRCEA_BY4_QUARTET,
                                     CRCEA_BY8_QUARTET,
                                     CRCEA_BY16_QUARTET,
                                     CRCEA_BY32_QUARTET,
                                     CRCEA_BY1_OCTET,
                                     CRCEA_BY2_OCTET,
                                     CRCEA_BY4_OCTET,
                                     CRCEA_BY8_OCTET,
                                     CRCEA_BY16_OCTET,
                                     CRCEA_BY32_OCTET,
                                     CRCEA_BY2_SEXDECTET,
                                     CRCEA_BY4_SEXDECTET,
                                     CRCEA_BY8_SEXDECTET,
                                     CRCEA_BY16_SEXDECTET,
                                     CRCEA_BY32_SEXDECTET) {
                            crcea_context bbb = {
                                .design = &design,
                                .algorithm = *algo,
                                .table = NULL,
                                .alloc = NULL,
                            };

                            uint64_t s = crcea(&bbb, seq, seq + sizeof(seq), 0);
                            if (r != s) {
                                bad = 1;
                                fprintf(stdout, "CRC-%02d-0x%08x (%c%c%c), expect 0x%016lx, actual 0x%016lx (%s) - %s\n",
                                        *bitsize, (uint32_t)design.polynomial & ~(~0 << 1 << (design.bitsize - 1)),
                                        (design.reflectin ? 'i' : '-'),
                                        (design.reflectout ? 'o' : '-'),
                                        (design.appendzero ? 'z' : '-'),
                                        r, s, (r == s ? "OK" : "FAILED"),
                                        lookup_algorithm_name(*algo));
                            }
                        }
                    }
                }
            }
        }
    }

    if (!bad) {
        puts("Test was passed all.");
    }

    return bad;
}
