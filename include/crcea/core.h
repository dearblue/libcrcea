/**
 * @file core.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons License Zero (CC0 / Public Domain)
 *
 * This is a general CRC generator.
 *
 * It's used slice-by-16 algorithm with byte-order free and byte-alignment free.
 * This is based on the Intel's slice-by-eight algorithm.
 *
 * Worst point is need more memory!
 *
 * references:
 * * https://sourceforge.net/projects/slicing-by-8/
 * * xz-utils
 *      * http://tukaani.org/xz/
 *      * xz-5.2.2/src/liblzma/check/crc32_fast.c
 *      * xz-5.2.2/src/liblzma/check/crc32_tablegen.c
 * * http://www.hackersdelight.org/hdcodetxt/crc.c.txt
 * * http://create.stephan-brumme.com/crc32/
 *
 * Preprocessor definisions before included this file:
 *
 * [CRCEA_PREFIX]
 *      REQUIRED.
 *
 *      This definision is undefined in last of this file.
 *
 * [CRCEA_TYPE]
 *      REQUIRED
 *
 *      This definision is undefined in last of this file.
 *
 * [CRCEA_STRIP_SIZE]
 *      optional, 1 by default.
 *
 * [CRCEA_VISIBILITY]
 *      optional, static by default.
 *
 * [CRCEA_INLINE]
 *      optional, not defined by default.
 *
 * [CRCEA_DEFAULT_MALLOC]
 *      optional, not defined by default.
 *
 *
 * example: mycrc.c
 *      #define CRCEA_PREFIX mycrc
 *      #define CRCEA_TYPE uint32_t
 *      #include <crcea/core.h>
 *
 *      extern uint32_t
 *      crc32(const void *ptr, const void *ptrend, uint32_t previous_crc)
 *      {
 *          static const crcea_model model = {
 *              .bitsize = 32,
 *              .polynomial = 0x04c11db7ul,
 *              .reflect_input = 1,
 *              .reflect_output = 1,
 *              .xor_output = ~0ul,
 *          };
 *
 *          static crcea_context cc = {
 *              .model = &model,
 *              .inttype = CRCEA_TYPE_INT32,
 *              .algorithm = CRCEA_ALGORITHM_STANDARD_TABLE,
 *              .table = NULL,
 *              .alloc = NULL,
 *          };
 *
 *          if (!cc.table) {
 *              void *table = malloc(mycrc_tablesize(cc.algorithm));
 *              mycrc_build_table(&cc, table);
 *              cc.table = table;
 *          }
 *
 *          uint32_t state = mycrc_setup(&cc, previous_crc);
 *          state = mycrc_update_standard_table(&cc, ptr, ptrend, state);
 *          return mycrc_finish(&cc, state);
 *      }
 *
 *      extern uint32_t
 *      crc32c(const void *ptr, const void *ptrend, uint32_t previous_crc)
 *      {
 *          static const crcea_model model = {
 *              .bitsize = 32,
 *              .polynomial = 0x1edc6f41ul,
 *              .reflect_input = 1,
 *              .reflect_output = 1,
 *              .xor_output = ~0ul,
 *          };
 *          static crcea_context cc = {
 *              .model = &model,
 *              .inttype = CRCEA_TYPE_INT32,
 *              .algorithm = CRCEA_ALGORITHM_SLICING_BY_4,
 *              .table = NULL,
 *              .alloc = NULL,
 *          };
 *
 *          uint32_t s = mycrc_update(&cc, previous_crc);
 *          s = mycrc_update(&cc, ptr, ptrend, s);
 *          return mycrc_finish(&cc, s);
 *      }
 *
 *      extern uint32_t
 *      crc32_bzip2(const void *ptr, const void *ptrend, uint32_t previous_crc)
 *      {
 *          static const crcea_model model = {
 *              .bitsize = 32,
 *              .polynomial = 0x04c11db7ul,
 *              .reflect_input = 0,
 *              .reflect_output = 0,
 *              .xor_output = ~0ul,
 *          };
 *          static crcea_context cc = {
 *              .model = &model,
 *              .inttype = CRCEA_TYPE_INT32,
 *              .algorithm = CRCEA_ALGORITHM_BITBYBIT_FAST,
 *              .table = NULL,
 *              .alloc = NULL,
 *          };
 *
 *          uint32_t s = mycrc_setup(&cc, previous_crc);
 *          s = mycrc_update(&cc, ptr, ptrend, s);
 *          return mycrc_finish(&cc, s);
 *      }
 */

#if !defined(CRCEA_PREFIX) || !defined(CRCEA_TYPE)
#   error Please definision both CRCEA_PREFIX and CRCEA_TYPE macros in before include this file.
#endif

#include "../crcea.h"

#ifndef CHAR_BIT
#   include <limits.h>
#endif


#ifndef CRCEA_VISIBILITY
#   define CRCEA_VISIBILITY static
#endif

#ifndef CRCEA_INLINE
#   define CRCEA_INLINE
#endif

#ifndef CRCEA_NO_MALLOC
#   ifndef CRCEA_MALLOC
#       include <stdlib.h>
#       define CRCEA_MALLOC malloc
#   endif
#endif

#ifndef CRCEA_STRIPE_SIZE
#   define CRCEA_STRIPE_SIZE 1
#endif

#define CRCEA_TOKEN__2(PREFIX, NAME)  PREFIX ## NAME
#define CRCEA_TOKEN__1(PREFIX, NAME)  CRCEA_TOKEN__2(PREFIX, NAME)
#define CRCEA_TOKEN(NAME)             CRCEA_TOKEN__1(CRCEA_PREFIX, NAME)
#define CRCEA_BITREFLECT              CRCEA_TOKEN(_bitreflect)
#define CRCEA_SETUP                   CRCEA_TOKEN(_setup)
#define CRCEA_FINISH                  CRCEA_TOKEN(_finish)
#define CRCEA_UPDATE                  CRCEA_TOKEN(_update)
#define CRCEA_PREPARE_TABLE           CRCEA_TOKEN(_prepare_table)
#define CRCEA_TABLESIZE               CRCEA_TOKEN(_tablesize)
#define CRCEA_BUILD_TABLE             CRCEA_TOKEN(_build_table)
#define CRCEA_UPDATE_BITBYBIT         CRCEA_TOKEN(_update_bitbybit)
#define CRCEA_UPDATE_BITBYBIT_FAST    CRCEA_TOKEN(_update_bitbybit_fast)
#define CRCEA_UPDATE_HALFBYTE_TABLE   CRCEA_TOKEN(_update_halfbyte_table)
#define CRCEA_UPDATE_STANDARD_TABLE   CRCEA_TOKEN(_update_standard_table)
#define CRCEA_UPDATE_SLICING_BY_4     CRCEA_TOKEN(_update_slicing_by_4)
#define CRCEA_UPDATE_SLICING_BY_8     CRCEA_TOKEN(_update_slicing_by_8)
#define CRCEA_UPDATE_SLICING_BY_16    CRCEA_TOKEN(_update_slicing_by_16)

#define CRCEA_BITSIZE         (sizeof(CRCEA_TYPE) * CHAR_BIT)
#define CRCEA_LSH(N, OFF)     ((OFF) < CRCEA_BITSIZE ? (N) << (OFF) : 0)
#define CRCEA_RSH(N, OFF)     ((OFF) < CRCEA_BITSIZE ? (N) >> (OFF) : 0)
#define CRCEA_BITMASK(WID)    (~CRCEA_LSH(~(CRCEA_TYPE)0, WID))


CRCEA_BEGIN_C_DECL

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_BITREFLECT(CRCEA_TYPE n)
{
    if (sizeof(CRCEA_TYPE) <= 1) {
        n = ((n >> 4) & 0x0fu) | ( n          << 4);
        n = ((n >> 2) & 0x33u) | ((n & 0x33u) << 2);
        n = ((n >> 1) & 0x55u) | ((n & 0x55u) << 1);
    } else if (sizeof(CRCEA_TYPE) <= 2) {
        n = ((n >> 8) & 0x00ffu) | ( n            << 8);
        n = ((n >> 4) & 0x0f0fu) | ((n & 0x0f0fu) << 4);
        n = ((n >> 2) & 0x3333u) | ((n & 0x3333u) << 2);
        n = ((n >> 1) & 0x5555u) | ((n & 0x5555u) << 1);
    } else if (sizeof(CRCEA_TYPE) <= 4) {
        n = ((n >> 16) & 0x0000fffful) | ( n                 << 16);
        n = ((n >>  8) & 0x00ff00fful) | ((n & 0x00ff00fful) <<  8);
        n = ((n >>  4) & 0x0f0f0f0ful) | ((n & 0x0f0f0f0ful) <<  4);
        n = ((n >>  2) & 0x33333333ul) | ((n & 0x33333333ul) <<  2);
        n = ((n >>  1) & 0x55555555ul) | ((n & 0x55555555ul) <<  1);
    } else { /* if (sizeof(CRCEA_TYPE) <= 8) { */
        n = ((n >> 32) & 0x00000000ffffffffull) | ( n                          << 32);
        n = ((n >> 16) & 0x0000ffff0000ffffull) | ((n & 0x0000ffff0000ffffull) << 16);
        n = ((n >>  8) & 0x00ff00ff00ff00ffull) | ((n & 0x00ff00ff00ff00ffull) <<  8);
        n = ((n >>  4) & 0x0f0f0f0f0f0f0f0full) | ((n & 0x0f0f0f0f0f0f0f0full) <<  4);
        n = ((n >>  2) & 0x3333333333333333ull) | ((n & 0x3333333333333333ull) <<  2);
        n = ((n >>  1) & 0x5555555555555555ull) | ((n & 0x5555555555555555ull) <<  1);
    }
    return n;
}

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_SETUP(const crcea_context *cc, CRCEA_TYPE crc)
{
    CRCEA_TYPE state = (crc ^ cc->model->xor_output) & CRCEA_BITMASK(cc->model->bitsize);
    if (cc->model->reflect_input ^ cc->model->reflect_output) {
        state = CRCEA_BITREFLECT(state << (CRCEA_BITSIZE - cc->model->bitsize));
    }

    if (!cc->model->reflect_input) {
        state <<= (CRCEA_BITSIZE - cc->model->bitsize);
    }

    return state;
}

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_FINISH(const crcea_context *cc, CRCEA_TYPE state)
{
    if (!cc->model->reflect_input) {
        state >>= (CRCEA_BITSIZE - cc->model->bitsize);
    }

    if (cc->model->reflect_input ^ cc->model->reflect_output) {
        state = CRCEA_BITREFLECT(state << (CRCEA_BITSIZE - cc->model->bitsize));
    }

    state ^= cc->model->xor_output;

    return state & CRCEA_BITMASK(cc->model->bitsize);
}

CRCEA_VISIBILITY CRCEA_INLINE size_t
CRCEA_TABLESIZE(int algo)
{
    switch (algo) {
    case CRCEA_ALGORITHM_HALFBYTE_TABLE:
        return sizeof(CRCEA_TYPE[16]);
    case CRCEA_ALGORITHM_STANDARD_TABLE:
    case CRCEA_ALGORITHM_SLICING_BY_4:
    case CRCEA_ALGORITHM_SLICING_BY_8:
    case CRCEA_ALGORITHM_SLICING_BY_16:
        return sizeof(CRCEA_TYPE[algo][256]);
    default:
        return 0;
    }
}

CRCEA_VISIBILITY CRCEA_INLINE void
CRCEA_BUILD_TABLE(const crcea_context *cc, void *table)
{
    int times, slice, bits;
    if (cc->algorithm < 0) {
        return;
    } else if (cc->algorithm == CRCEA_ALGORITHM_HALFBYTE_TABLE) {
        times = 16;
        slice = 1;
        bits = 4;
    } else {
        // CRCEA_ALGORITHM_STANDARD_TABLE
        // CRCEA_ALGORITHM_SLICING_BY_4
        // CRCEA_ALGORITHM_SLICING_BY_8
        // CRCEA_ALGORITHM_SLICING_BY_16
        times = 256;
        slice = cc->algorithm;
        bits = 8;
    }

    CRCEA_TYPE *t = table;
    const CRCEA_TYPE *tt = t;
    if (cc->model->reflect_input) {
        int s, b, i;
        CRCEA_TYPE polynomial = CRCEA_BITREFLECT(cc->model->polynomial << (CRCEA_BITSIZE - cc->model->bitsize));

        for (b = 0; b < times; b ++, t ++) {
            CRCEA_TYPE r = b;
            for (i = bits; i > 0; i --) {
                r = (r >> 1) ^ (polynomial & -(r & 1));
            }
            *t = r;
        }

        for (s = 1; s < slice; s ++) {
            const CRCEA_TYPE *q = t - times;
            for (b = 0; b < times; b ++, t ++, q ++) {
                *t = tt[*q & 0xff] ^ (*q >> 8);
            }
        }
    } else {
        int s, b, i;
        CRCEA_TYPE polynomial = cc->model->polynomial << (CRCEA_BITSIZE - cc->model->bitsize);

        for (b = 0; b < times; b ++) {
            CRCEA_TYPE r = (CRCEA_TYPE)b << (CRCEA_BITSIZE - bits);
            for (i = bits; i > 0; i --) {
                r = (r << 1) ^ (polynomial & -(r >> (CRCEA_BITSIZE - 1)));
            }
            *t ++ = r;
        }

        for (s = 1; s < slice; s ++) {
            const CRCEA_TYPE *q = t - times;
            for (b = 0; b < times; b ++, t ++, q ++) {
                *t = tt[*q >> (CRCEA_BITSIZE - 8)] ^ (*q << 8);
            }
        }
    }
}

#define CRCEA_SETUP_POLYNOMIAL(POLY, BS)  ((POLY) << (CRCEA_BITSIZE - (BS)))
#define CRCEA_SHIFT_INPUT(B)              ((CRCEA_TYPE)(uint8_t)(B) << (CRCEA_BITSIZE - 8))
#define CRCEA_POPBIT(ST, N, L)            (CRCEA_RSH(ST, CRCEA_BITSIZE - ((N) + (L))) & CRCEA_BITMASK(L))
#define CRCEA_HEAD(ST)                    (int8_t)(CRCEA_RSH(ST, CRCEA_BITSIZE - 1))
#define CRCEA_SETUP_POLYNOMIAL_R(POLY, BS)    (CRCEA_BITREFLECT(POLY) >> (CRCEA_BITSIZE - (BS)))
#define CRCEA_SHIFT_INPUT_R(B)            ((CRCEA_TYPE)(uint8_t)(B))
#define CRCEA_POPBIT_R(ST, N, L)          (CRCEA_RSH(ST, N) & CRCEA_BITMASK(L))
#define CRCEA_HEAD_R(ST)                  ((int8_t)(ST) & 1)


#define CRCEA_UPDATE_CORE(P, PP, SLICESIZE, MAINCODE, SUBCODE)                \
    do {                                                                    \
        if (SLICESIZE > 1 || CRCEA_STRIPE_SIZE > 1) {                         \
            const size_t stripesize__ = CRCEA_STRIPE_SIZE * (SLICESIZE);      \
            const char *pps__ = P + ((PP - P) & ~(stripesize__ - 1));       \
            while (P < pps__) {                                             \
                int i__;                                                    \
                for (i__ = (CRCEA_STRIPE_SIZE); i__ > 0; i__ --, P += (SLICESIZE)) { \
                    MAINCODE;                                               \
                }                                                           \
            }                                                               \
        }                                                                   \
                                                                            \
        for (; P < PP; P ++) {                                              \
            if (SLICESIZE > 1) {                                            \
                SUBCODE;                                                    \
            } else {                                                        \
                MAINCODE;                                                   \
            }                                                               \
        }                                                                   \
    } while (0)                                                             \

#define CRCEA_UPDATE_DECL(CC, STATE, F)                                       \
    do {                                                                    \
        if ((CC)->model->reflect_input) {                                          \
            F(CRCEA_SETUP_POLYNOMIAL_R, CRCEA_SHIFT_INPUT_R, CRCEA_RSH, CRCEA_POPBIT_R, CRCEA_HEAD_R); \
        } else {                                                            \
            F(CRCEA_SETUP_POLYNOMIAL, CRCEA_SHIFT_INPUT, CRCEA_LSH, CRCEA_POPBIT, CRCEA_HEAD);  \
        }                                                                   \
    } while (0)                                                             \


CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITBYBIT(const crcea_context *cc, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITBYBIT_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, HEAD)     \
    CRCEA_TYPE poly = SETUP_POLYNOMIAL(cc->model->polynomial, cc->model->bitsize);          \
    CRCEA_UPDATE_CORE(p, pp, 1, {                                             \
        int i;                                                              \
        state ^= SHIFT_INPUT(*p);                                           \
        for (i = 8; i > 0; i --) {                                          \
            char head = HEAD(state);                                        \
            state = SHIFT(state, 1);                                        \
            if (head) { state ^= poly; }                                    \
        }                                                                   \
    }, );                                                                   \

    CRCEA_UPDATE_DECL(cc, state, CRCEA_BITBYBIT_DECL);

    return state;
}

/*
 * reference:
 * * http://www.hackersdelight.org/hdcodetxt/crc.c.txt#crc32h
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITBYBIT_FAST(const crcea_context *cc, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITBYBIT_FAST_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, HEAD) \
    const CRCEA_TYPE g0 = SETUP_POLYNOMIAL(cc->model->polynomial, cc->model->bitsize),      \
                   g1 = SHIFT(g0, 1) ^ (g0 & -POPBIT(g0, 0, 1)),            \
                   g2 = SHIFT(g1, 1) ^ (g0 & -POPBIT(g1, 0, 1)),            \
                   g3 = SHIFT(g2, 1) ^ (g0 & -POPBIT(g2, 0, 1)),            \
                   g4 = SHIFT(g3, 1) ^ (g0 & -POPBIT(g3, 0, 1)),            \
                   g5 = SHIFT(g4, 1) ^ (g0 & -POPBIT(g4, 0, 1)),            \
                   g6 = SHIFT(g5, 1) ^ (g0 & -POPBIT(g5, 0, 1)),            \
                   g7 = SHIFT(g6, 1) ^ (g0 & -POPBIT(g6, 0, 1));            \
                                                                            \
    CRCEA_UPDATE_CORE(p, pp, 1, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        state = SHIFT(state, 8) ^                                           \
                (g7 & -POPBIT(state, 0, 1)) ^                               \
                (g6 & -POPBIT(state, 1, 1)) ^                               \
                (g5 & -POPBIT(state, 2, 1)) ^                               \
                (g4 & -POPBIT(state, 3, 1)) ^                               \
                (g3 & -POPBIT(state, 4, 1)) ^                               \
                (g2 & -POPBIT(state, 5, 1)) ^                               \
                (g1 & -POPBIT(state, 6, 1)) ^                               \
                (g0 & -POPBIT(state, 7, 1));                                \
    }, );                                                                   \

    CRCEA_UPDATE_DECL(cc, state, CRCEA_BITBYBIT_FAST_DECL);

    return state;
}

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_HALFBYTE_TABLE(const crcea_context *cc, const char *p, const char *pp, CRCEA_TYPE state)
{
    const CRCEA_TYPE *t = cc->table;

#define CRCEA_HALFBYTE_TABLE_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, HEAD) \
    CRCEA_UPDATE_CORE(p, pp, 1, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        state = SHIFT(state, 4) ^ t[POPBIT(state, 0, 4)];                   \
        state = SHIFT(state, 4) ^ t[POPBIT(state, 0, 4)];                   \
    }, );                                                                   \

    CRCEA_UPDATE_DECL(cc, state, CRCEA_HALFBYTE_TABLE_DECL);

    return state;
}

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_STANDARD_TABLE(const crcea_context *cc, const char *p, const char *pp, CRCEA_TYPE state)
{
    const CRCEA_TYPE *t = cc->table;

#define CRCEA_STANDARD_TABLE_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, HEAD) \
    CRCEA_UPDATE_CORE(p, pp, 1, {                                             \
        state = SHIFT(state, 8) ^ t[(uint8_t)*p ^ POPBIT(state, 0, 8)];     \
    }, );                                                                   \

    CRCEA_UPDATE_DECL(cc, state, CRCEA_STANDARD_TABLE_DECL);

    return state;
}

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_SLICING_BY_4(const crcea_context *cc, const char *p, const char *pp, CRCEA_TYPE state)
{
    const CRCEA_TYPE (*t)[256] = cc->table;

#define CRCEA_SLICING_BY_4_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, HEAD) \
    CRCEA_UPDATE_CORE(p, pp, 4, {                                             \
        state = SHIFT(state, 32) ^                                          \
                t[3][(uint8_t)p[0] ^ POPBIT(state,  0, 8)] ^                \
                t[2][(uint8_t)p[1] ^ POPBIT(state,  8, 8)] ^                \
                t[1][(uint8_t)p[2] ^ POPBIT(state, 16, 8)] ^                \
                t[0][(uint8_t)p[3] ^ POPBIT(state, 24, 8)];                 \
    }, {                                                                    \
        state = SHIFT(state, 8) ^ t[0][(uint8_t)*p ^ POPBIT(state, 0, 8)];  \
    });                                                                     \

    CRCEA_UPDATE_DECL(cc, state, CRCEA_SLICING_BY_4_DECL);

    return state;
}

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_SLICING_BY_8(const crcea_context *cc, const char *p, const char *pp, CRCEA_TYPE state)
{
    const CRCEA_TYPE (*t)[256] = cc->table;

#define CRCEA_SLICING_BY_8_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, HEAD) \
    CRCEA_UPDATE_CORE(p, pp, 8, {                                             \
        state = SHIFT(state, 64) ^                                          \
                t[7][(uint8_t)p[0] ^ POPBIT(state,  0, 8)] ^                \
                t[6][(uint8_t)p[1] ^ POPBIT(state,  8, 8)] ^                \
                t[5][(uint8_t)p[2] ^ POPBIT(state, 16, 8)] ^                \
                t[4][(uint8_t)p[3] ^ POPBIT(state, 24, 8)] ^                \
                t[3][(uint8_t)p[4] ^ POPBIT(state, 32, 8)] ^                \
                t[2][(uint8_t)p[5] ^ POPBIT(state, 40, 8)] ^                \
                t[1][(uint8_t)p[6] ^ POPBIT(state, 48, 8)] ^                \
                t[0][(uint8_t)p[7] ^ POPBIT(state, 56, 8)];                 \
    }, {                                                                    \
        state = SHIFT(state, 8) ^ t[0][(uint8_t)*p ^ POPBIT(state, 0, 8)];  \
    });                                                                     \

    CRCEA_UPDATE_DECL(cc, state, CRCEA_SLICING_BY_8_DECL);

    return state;
}

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_SLICING_BY_16(const crcea_context *cc, const char *p, const char *pp, CRCEA_TYPE state)
{
    const CRCEA_TYPE (*t)[256] = cc->table;

#define CRCEA_SLICING_BY_16_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, HEAD) \
    CRCEA_UPDATE_CORE(p, pp, 16, {                                            \
        state = SHIFT(state, 128) ^                                         \
                t[15][(uint8_t)p[ 0] ^ POPBIT(state,   0, 8)] ^             \
                t[14][(uint8_t)p[ 1] ^ POPBIT(state,   8, 8)] ^             \
                t[13][(uint8_t)p[ 2] ^ POPBIT(state,  16, 8)] ^             \
                t[12][(uint8_t)p[ 3] ^ POPBIT(state,  24, 8)] ^             \
                t[11][(uint8_t)p[ 4] ^ POPBIT(state,  32, 8)] ^             \
                t[10][(uint8_t)p[ 5] ^ POPBIT(state,  40, 8)] ^             \
                t[ 9][(uint8_t)p[ 6] ^ POPBIT(state,  48, 8)] ^             \
                t[ 8][(uint8_t)p[ 7] ^ POPBIT(state,  56, 8)] ^             \
                t[ 7][(uint8_t)p[ 8] ^ POPBIT(state,  64, 8)] ^             \
                t[ 6][(uint8_t)p[ 9] ^ POPBIT(state,  72, 8)] ^             \
                t[ 5][(uint8_t)p[10] ^ POPBIT(state,  80, 8)] ^             \
                t[ 4][(uint8_t)p[11] ^ POPBIT(state,  88, 8)] ^             \
                t[ 3][(uint8_t)p[12] ^ POPBIT(state,  96, 8)] ^             \
                t[ 2][(uint8_t)p[13] ^ POPBIT(state, 104, 8)] ^             \
                t[ 1][(uint8_t)p[14] ^ POPBIT(state, 112, 8)] ^             \
                t[ 0][(uint8_t)p[15] ^ POPBIT(state, 120, 8)];              \
    }, {                                                                    \
        state = SHIFT(state, 8) ^ t[0][(uint8_t)*p ^ POPBIT(state, 0, 8)];  \
    });                                                                     \

    CRCEA_UPDATE_DECL(cc, state, CRCEA_SLICING_BY_16_DECL);

    return state;
}

CRCEA_VISIBILITY CRCEA_INLINE int
CRCEA_PREPARE_TABLE(crcea_context *cc)
{
    int algo = cc->algorithm;

    if (!cc->table && algo >= CRCEA_ALGORITHM_HALFBYTE_TABLE) {
        crcea_alloc_f *alloc = cc->alloc;
        if (!alloc) {
#ifdef CRCEA_DEFAULT_MALLOC
            alloc = CRCEA_DEFAULT_MALLOC;
#else
            return CRCEA_ALGORITHM_BITBYBIT_FAST;
#endif
        }

        if (alloc && algo >= CRCEA_ALGORITHM_HALFBYTE_TABLE) {
            void *bufp = alloc(cc, CRCEA_TABLESIZE(algo));
            CRCEA_BUILD_TABLE(cc, bufp);
            cc->table = bufp;
        }
    }

    return algo;
}

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE(crcea_context *cc, const char *p, const char *pp, CRCEA_TYPE state)
{
    int algo = CRCEA_PREPARE_TABLE(cc);

    switch (algo) {
    case CRCEA_ALGORITHM_BITBYBIT:
        return CRCEA_UPDATE_BITBYBIT(cc, p, pp, state);
    case CRCEA_ALGORITHM_HALFBYTE_TABLE:
        return CRCEA_UPDATE_HALFBYTE_TABLE(cc, p, pp, state);
    case CRCEA_ALGORITHM_STANDARD_TABLE:
        return CRCEA_UPDATE_STANDARD_TABLE(cc, p, pp, state);
    case CRCEA_ALGORITHM_SLICING_BY_4:
        return CRCEA_UPDATE_SLICING_BY_4(cc, p, pp, state);
    case CRCEA_ALGORITHM_SLICING_BY_8:
        return CRCEA_UPDATE_SLICING_BY_8(cc, p, pp, state);
    case CRCEA_ALGORITHM_SLICING_BY_16:
        return CRCEA_UPDATE_SLICING_BY_16(cc, p, pp, state);
    case CRCEA_ALGORITHM_BITBYBIT_FAST:
    default:
        return CRCEA_UPDATE_BITBYBIT_FAST(cc, p, pp, state);
    }
}

CRCEA_END_C_DECL


#undef CRCEA_PREFIX
#undef CRCEA_TYPE
#undef CRCEA_BUILD_TABLES
#undef CRCEA_BUILD_REFLECT_TABLES
#undef CRCEA_REFLECT_UPDATE


#undef CRCEA_TOKEN__2
#undef CRCEA_TOKEN__1
#undef CRCEA_TOKEN
#undef CRCEA_BITREFLECT
#undef CRCEA_SETUP
#undef CRCEA_FINISH
#undef CRCEA_UPDATE
#undef CRCEA_PREPARE_TABLE
#undef CRCEA_TABLESIZE
#undef CRCEA_BUILD_TABLE
#undef CRCEA_UPDATE_BITBYBIT
#undef CRCEA_UPDATE_BITBYBIT_FAST
#undef CRCEA_UPDATE_HALFBYTE_TABLE
#undef CRCEA_UPDATE_STANDARD_TABLE
#undef CRCEA_UPDATE_SLICING_BY_4
#undef CRCEA_UPDATE_SLICING_BY_8
#undef CRCEA_UPDATE_SLICING_BY_16
#undef CRCEA_BITSIZE
#undef CRCEA_LSH
#undef CRCEA_RSH
#undef CRCEA_BITMASK

#undef CRCEA_SETUP_POLYNOMIAL
#undef CRCEA_SHIFT_INPUT
#undef CRCEA_SHIFT
#undef CRCEA_POPBIT
#undef CRCEA_SETUP_POLYNOMIAL_R
#undef CRCEA_SHIFT_INPUT_R
#undef CRCEA_SHIFT_R
#undef CRCEA_POPBIT_R
#undef CRCEA_UPDATE_CORE
#undef CRCEA_UPDATE_DECL
#undef CRCEA_BITBYBIT_DECL
#undef CRCEA_BITBYBIT_FAST_DECL
#undef CRCEA_HALFBYTE_TABLE_DECL
#undef CRCEA_STANDARD_TABLE_DECL
#undef CRCEA_SLICING_BY_16_DECL
#undef CRCEA_SLICING_BY_4_DECL
#undef CRCEA_SLICING_BY_8_DECL
