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
 *              .inttype = CRCEA_INT32,
 *              .algorithm = CRCEA_BY1_OCTET,
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
 *              .inttype = CRCEA_INT32,
 *              .algorithm = CRCEA_BY4_OCTET,
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
 *              .inttype = CRCEA_INT32,
 *              .algorithm = CRCEA_BITBYBIT_FAST,
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
#define CRCEA_UPDATE_BY_SOLO          CRCEA_TOKEN(_update_by_solo)
#define CRCEA_UPDATE_BY1_SOLO         CRCEA_TOKEN(_update_by1_solo)
#define CRCEA_UPDATE_BY2_SOLO         CRCEA_TOKEN(_update_by2_solo)
#define CRCEA_UPDATE_BY4_SOLO         CRCEA_TOKEN(_update_by4_solo)
#define CRCEA_UPDATE_BY8_SOLO         CRCEA_TOKEN(_update_by8_solo)
#define CRCEA_UPDATE_BY16_SOLO        CRCEA_TOKEN(_update_by16_solo)
#define CRCEA_UPDATE_BY32_SOLO        CRCEA_TOKEN(_update_by32_solo)
#define CRCEA_UPDATE_BY_DUO           CRCEA_TOKEN(_update_by_duo)
#define CRCEA_UPDATE_BY1_DUO          CRCEA_TOKEN(_update_by1_duo)
#define CRCEA_UPDATE_BY2_DUO          CRCEA_TOKEN(_update_by2_duo)
#define CRCEA_UPDATE_BY4_DUO          CRCEA_TOKEN(_update_by4_duo)
#define CRCEA_UPDATE_BY8_DUO          CRCEA_TOKEN(_update_by8_duo)
#define CRCEA_UPDATE_BY16_DUO         CRCEA_TOKEN(_update_by16_duo)
#define CRCEA_UPDATE_BY32_DUO         CRCEA_TOKEN(_update_by32_duo)
#define CRCEA_UPDATE_BY_QUARTET       CRCEA_TOKEN(_update_by_quartet)
#define CRCEA_UPDATE_BY1_QUARTET      CRCEA_TOKEN(_update_by1_quartet)
#define CRCEA_UPDATE_BY2_QUARTET      CRCEA_TOKEN(_update_by2_quartet)
#define CRCEA_UPDATE_BY4_QUARTET      CRCEA_TOKEN(_update_by4_quartet)
#define CRCEA_UPDATE_BY8_QUARTET      CRCEA_TOKEN(_update_by8_quartet)
#define CRCEA_UPDATE_BY16_QUARTET     CRCEA_TOKEN(_update_by16_quartet)
#define CRCEA_UPDATE_BY32_QUARTET     CRCEA_TOKEN(_update_by32_quartet)
#define CRCEA_UPDATE_BY1_OCTET        CRCEA_TOKEN(_update_by1_octet)
#define CRCEA_UPDATE_BY2_OCTET        CRCEA_TOKEN(_update_by2_octet)
#define CRCEA_UPDATE_BY4_OCTET        CRCEA_TOKEN(_update_by4_octet)
#define CRCEA_UPDATE_BY8_OCTET        CRCEA_TOKEN(_update_by8_octet)
#define CRCEA_UPDATE_BY16_OCTET       CRCEA_TOKEN(_update_by16_octet)
#define CRCEA_UPDATE_BY32_OCTET       CRCEA_TOKEN(_update_by32_octet)
#define CRCEA_UPDATE_BY2_SEXDECTET    CRCEA_TOKEN(_update_by2_sexdectet)
#define CRCEA_UPDATE_BY4_SEXDECTET    CRCEA_TOKEN(_update_by4_sexdectet)
#define CRCEA_UPDATE_BY8_SEXDECTET    CRCEA_TOKEN(_update_by8_sexdectet)
#define CRCEA_UPDATE_BY16_SEXDECTET   CRCEA_TOKEN(_update_by16_sexdectet)
#define CRCEA_UPDATE_BY32_SEXDECTET   CRCEA_TOKEN(_update_by32_sexdectet)

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
CRCEA_SETUP(const crcea_model *model, CRCEA_TYPE crc)
{
    CRCEA_TYPE state = (crc ^ model->xor_output) & CRCEA_BITMASK(model->bitsize);
    if (model->reflect_input ^ model->reflect_output) {
        state = CRCEA_BITREFLECT(state << (CRCEA_BITSIZE - model->bitsize));
    }

    if (!model->reflect_input) {
        state <<= (CRCEA_BITSIZE - model->bitsize);
    }

    return state;
}

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_FINISH(const crcea_model *model, CRCEA_TYPE state)
{
    if (!model->reflect_input) {
        state >>= (CRCEA_BITSIZE - model->bitsize);
    }

    if (model->reflect_input ^ model->reflect_output) {
        state = CRCEA_BITREFLECT(state << (CRCEA_BITSIZE - model->bitsize));
    }

    state ^= model->xor_output;

    return state & CRCEA_BITMASK(model->bitsize);
}

CRCEA_VISIBILITY CRCEA_INLINE size_t
CRCEA_TABLESIZE(int algo)
{
    switch (algo) {
    case CRCEA_BY_SOLO:
        return sizeof(CRCEA_TYPE[2]);
    case CRCEA_BY1_SOLO:
    case CRCEA_BY2_SOLO:
    case CRCEA_BY4_SOLO:
    case CRCEA_BY8_SOLO:
    case CRCEA_BY16_SOLO:
    case CRCEA_BY32_SOLO:
        return sizeof(CRCEA_TYPE[8 * (1 << (algo & 0xff) >> 1)][2]);
    case CRCEA_BY_DUO:
        return sizeof(CRCEA_TYPE[4]);
    case CRCEA_BY1_DUO:
    case CRCEA_BY2_DUO:
    case CRCEA_BY4_DUO:
    case CRCEA_BY8_DUO:
    case CRCEA_BY16_DUO:
    case CRCEA_BY32_DUO:
        return sizeof(CRCEA_TYPE[4 * (1 << (algo & 0xff) >> 1)][4]);
    case CRCEA_BY_QUARTET:
        return sizeof(CRCEA_TYPE[16]);
    case CRCEA_BY1_QUARTET:
    case CRCEA_BY2_QUARTET:
    case CRCEA_BY4_QUARTET:
    case CRCEA_BY8_QUARTET:
    case CRCEA_BY16_QUARTET:
    case CRCEA_BY32_QUARTET:
        return sizeof(CRCEA_TYPE[2 * (1 << (algo & 0xff) >> 1)][16]);
    case CRCEA_BY1_OCTET:
    case CRCEA_BY2_OCTET:
    case CRCEA_BY4_OCTET:
    case CRCEA_BY8_OCTET:
    case CRCEA_BY16_OCTET:
    case CRCEA_BY32_OCTET:
        return sizeof(CRCEA_TYPE[1 * (1 << (algo & 0xff) >> 1)][256]);
    case CRCEA_BY2_SEXDECTET:
    case CRCEA_BY4_SEXDECTET:
    case CRCEA_BY8_SEXDECTET:
    case CRCEA_BY16_SEXDECTET:
    case CRCEA_BY32_SEXDECTET:
        return sizeof(CRCEA_TYPE[1 * (1 << (algo & 0xff) >> 2)][65536]);
    default:
        return 0;
    }
}

CRCEA_VISIBILITY CRCEA_INLINE void
CRCEA_BUILD_TABLE(const crcea_model *model, int algorithm, void *table)
{
    int times, slice, bits;
    switch (algorithm) {
    case CRCEA_BY_SOLO:
    case CRCEA_BY1_SOLO:
    case CRCEA_BY2_SOLO:
    case CRCEA_BY4_SOLO:
    case CRCEA_BY8_SOLO:
    case CRCEA_BY16_SOLO:
    case CRCEA_BY32_SOLO:
        slice = 8 * (1 << (algorithm & 0xff) >> 1);
        bits = 1;
        break;
    case CRCEA_BY_DUO:
    case CRCEA_BY1_DUO:
    case CRCEA_BY2_DUO:
    case CRCEA_BY4_DUO:
    case CRCEA_BY8_DUO:
    case CRCEA_BY16_DUO:
    case CRCEA_BY32_DUO:
        slice = 4 * (1 << (algorithm & 0xff) >> 1);
        bits = 2;
        break;
    case CRCEA_BY_QUARTET:
    case CRCEA_BY1_QUARTET:
    case CRCEA_BY2_QUARTET:
    case CRCEA_BY4_QUARTET:
    case CRCEA_BY8_QUARTET:
    case CRCEA_BY16_QUARTET:
    case CRCEA_BY32_QUARTET:
        slice = 2 * (1 << (algorithm & 0xff) >> 1);
        bits = 4;
        break;
    case CRCEA_BY1_OCTET:
    case CRCEA_BY2_OCTET:
    case CRCEA_BY4_OCTET:
    case CRCEA_BY8_OCTET:
    case CRCEA_BY16_OCTET:
    case CRCEA_BY32_OCTET:
        slice = (1 << (algorithm & 0xff) >> 1);
        bits = 8;
        break;
    case CRCEA_BY2_SEXDECTET:
    case CRCEA_BY4_SEXDECTET:
    case CRCEA_BY8_SEXDECTET:
    case CRCEA_BY16_SEXDECTET:
    case CRCEA_BY32_SEXDECTET:
        slice = (1 << (algorithm & 0xff) >> 2);
        bits = 16;
        break;
    default:
        return;
    }

    times = 1 << bits;

    CRCEA_TYPE *t = table;
    const CRCEA_TYPE *tt = t;
    if (model->reflect_input) {
        int s, b, i;
        CRCEA_TYPE polynomial = CRCEA_BITREFLECT(model->polynomial << (CRCEA_BITSIZE - model->bitsize));

        for (b = 0; b < times; b ++, t ++) {
            CRCEA_TYPE r = b;
            for (i = bits; i > 0; i --) {
                r = (r >> 1) ^ (polynomial & -(r & 1));
            }
            *t = r;
        }

        const int bitmask = ~(~0 << bits);
        for (s = 1; s < slice; s ++) {
            const CRCEA_TYPE *q = t - times;
            for (b = 0; b < times; b ++, t ++, q ++) {
                *t = tt[*q & bitmask] ^ (*q >> bits);
            }
        }
    } else {
        int s, b, i;
        CRCEA_TYPE polynomial = model->polynomial << (CRCEA_BITSIZE - model->bitsize);

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
                *t = tt[*q >> (CRCEA_BITSIZE - bits)] ^ (*q << bits);
            }
        }
    }
}

#define CRCEA_SETUP_POLYNOMIAL(POLY, BS)  ((POLY) << (CRCEA_BITSIZE - (BS)))
#define CRCEA_SHIFT_INPUT(B)              ((CRCEA_TYPE)(uint8_t)(B) << (CRCEA_BITSIZE - 8))
#define CRCEA_POPBIT(ST, N, L)            (CRCEA_RSH(ST, CRCEA_BITSIZE - ((N) + (L))) & CRCEA_BITMASK(L))
#define CRCEA_POPBIT8(X, N, L)            (CRCEA_RSH(X, 8 - ((N) + (L))) & CRCEA_BITMASK(L))
#define CRCEA_LOAD16(P)                   (((uint16_t)*((uint8_t *)P + 0) << 8) | ((uint16_t)*((uint8_t *)P + 1) << 0))
#define CRCEA_INDEX16(A)                  ((uint16_t)(uint16_t)(A))
#define CRCEA_SETUP_POLYNOMIAL_R(POLY, BS)    (CRCEA_BITREFLECT(POLY) >> (CRCEA_BITSIZE - (BS)))
#define CRCEA_SHIFT_INPUT_R(B)            ((CRCEA_TYPE)(uint8_t)(B))
#define CRCEA_POPBIT_R(ST, N, L)          (CRCEA_RSH(ST, N) & CRCEA_BITMASK(L))
#define CRCEA_POPBIT8_R(X, N, L)          (CRCEA_RSH(X, N) & CRCEA_BITMASK(L))
#define CRCEA_LOAD16_R(P)                 (((uint16_t)*((uint8_t *)P + 0) << 0) | ((uint16_t)*((uint8_t *)P + 1) << 8))
#define CRCEA_INDEX16_R(A)                ((uint16_t)(uint16_t)(A) << 8)


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

#define CRCEA_UPDATE_DECL(MODEL, STATE, F)                                       \
    do {                                                                    \
        if ((MODEL)->reflect_input) {                                          \
            F(CRCEA_SETUP_POLYNOMIAL_R, CRCEA_SHIFT_INPUT_R, CRCEA_RSH, CRCEA_POPBIT_R, CRCEA_POPBIT8_R, CRCEA_LOAD16_R, CRCEA_INDEX16_R); \
        } else {                                                            \
            F(CRCEA_SETUP_POLYNOMIAL, CRCEA_SHIFT_INPUT, CRCEA_LSH, CRCEA_POPBIT, CRCEA_POPBIT8, CRCEA_LOAD16, CRCEA_INDEX16);  \
        }                                                                   \
    } while (0)                                                             \


CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITBYBIT(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITBYBIT_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16)     \
    CRCEA_TYPE poly = SETUP_POLYNOMIAL(model->polynomial, model->bitsize);          \
    CRCEA_UPDATE_CORE(p, pp, 1, {                                             \
        int i;                                                              \
        state ^= SHIFT_INPUT(*p);                                           \
        for (i = 8; i > 0; i --) {                                          \
            char head = POPBIT(state, 0, 1);                                \
            state = SHIFT(state, 1);                                        \
            if (head) { state ^= poly; }                                    \
        }                                                                   \
    }, );                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BITBYBIT_DECL);

    return state;
}

/*
 * reference:
 * * http://www.hackersdelight.org/hdcodetxt/crc.c.txt#crc32h
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITBYBIT_FAST(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITBYBIT_FAST_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    const CRCEA_TYPE g0 = SETUP_POLYNOMIAL(model->polynomial, model->bitsize),      \
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

    CRCEA_UPDATE_DECL(model, state, CRCEA_BITBYBIT_FAST_DECL);

    return state;
}

/*
 * Slicing by Single Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY_DUO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE *t = (const CRCEA_TYPE *)table;

#define CRCEA_BY_DUO_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 1, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        state = SHIFT(state, 2) ^ t[POPBIT(state, 0, 2)];                   \
        state = SHIFT(state, 2) ^ t[POPBIT(state, 0, 2)];                   \
        state = SHIFT(state, 2) ^ t[POPBIT(state, 0, 2)];                   \
        state = SHIFT(state, 2) ^ t[POPBIT(state, 0, 2)];                   \
    }, );                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY_DUO_DECL);

    return state;
}

/*
 * Slicing by Quadruple Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY1_DUO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[4] = (const CRCEA_TYPE (*)[4])table;

#define CRCEA_BY1_DUO_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 1, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        const uint8_t n = POPBIT(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[3][POPBIT8(n, 0, 2)] ^ \
                t[2][POPBIT8(n, 2, 2)] ^ \
                t[1][POPBIT8(n, 4, 2)] ^ \
                t[0][POPBIT8(n, 6, 2)];                   \
    }, );                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY1_DUO_DECL);

    return state;
}

/*
 * Slicing by Octuple Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY2_DUO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[4] = (const CRCEA_TYPE (*)[4])table;

#define CRCEA_BY2_DUO_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 2, {                                             \
        const uint8_t n0 = (uint8_t)p[0] ^ POPBIT(state, 0, 8); \
        const uint8_t n1 = (uint8_t)p[1] ^ POPBIT(state, 8, 8); \
        state = SHIFT(state, 16) ^ \
                t[7][POPBIT8(n0, 0, 2)] ^ \
                t[6][POPBIT8(n0, 2, 2)] ^ \
                t[5][POPBIT8(n0, 4, 2)] ^ \
                t[4][POPBIT8(n0, 6, 2)] ^ \
                t[3][POPBIT8(n1, 0, 2)] ^ \
                t[2][POPBIT8(n1, 2, 2)] ^ \
                t[1][POPBIT8(n1, 4, 2)] ^ \
                t[0][POPBIT8(n1, 6, 2)]; \
    }, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        const uint8_t n = POPBIT(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[3][POPBIT8(n, 0, 2)] ^ \
                t[2][POPBIT8(n, 2, 2)] ^ \
                t[1][POPBIT8(n, 4, 2)] ^ \
                t[0][POPBIT8(n, 6, 2)];                   \
    });                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY2_DUO_DECL);

    return state;
}

/*
 * Slicing by Sexdecuple Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY4_DUO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[4] = (const CRCEA_TYPE (*)[4])table;

#define CRCEA_BY4_DUO_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 4, {                                             \
        const uint8_t n0 = (uint8_t)p[0] ^ POPBIT(state,  0, 8); \
        const uint8_t n1 = (uint8_t)p[1] ^ POPBIT(state,  8, 8); \
        const uint8_t n2 = (uint8_t)p[2] ^ POPBIT(state, 16, 8); \
        const uint8_t n3 = (uint8_t)p[3] ^ POPBIT(state, 24, 8); \
        state = SHIFT(state, 32) ^ \
                t[15][POPBIT8(n0, 0, 2)] ^ \
                t[14][POPBIT8(n0, 2, 2)] ^ \
                t[13][POPBIT8(n0, 4, 2)] ^ \
                t[12][POPBIT8(n0, 6, 2)] ^ \
                t[11][POPBIT8(n1, 0, 2)] ^ \
                t[10][POPBIT8(n1, 2, 2)] ^ \
                t[ 9][POPBIT8(n1, 4, 2)] ^ \
                t[ 8][POPBIT8(n1, 6, 2)] ^ \
                t[ 7][POPBIT8(n2, 0, 2)] ^ \
                t[ 6][POPBIT8(n2, 2, 2)] ^ \
                t[ 5][POPBIT8(n2, 4, 2)] ^ \
                t[ 4][POPBIT8(n2, 6, 2)] ^ \
                t[ 3][POPBIT8(n3, 0, 2)] ^ \
                t[ 2][POPBIT8(n3, 2, 2)] ^ \
                t[ 1][POPBIT8(n3, 4, 2)] ^ \
                t[ 0][POPBIT8(n3, 6, 2)]; \
    }, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        const uint8_t n = POPBIT(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[3][POPBIT8(n, 0, 2)] ^ \
                t[2][POPBIT8(n, 2, 2)] ^ \
                t[1][POPBIT8(n, 4, 2)] ^ \
                t[0][POPBIT8(n, 6, 2)];                   \
    });                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY4_DUO_DECL);

    return state;
}

/*
 * Slicing by Duotriguple Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY8_DUO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[4] = (const CRCEA_TYPE (*)[4])table;

#define CRCEA_BY8_DUO_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 8, {                                             \
        const uint8_t n0 = (uint8_t)p[0] ^ POPBIT(state,  0, 8); \
        const uint8_t n1 = (uint8_t)p[1] ^ POPBIT(state,  8, 8); \
        const uint8_t n2 = (uint8_t)p[2] ^ POPBIT(state, 16, 8); \
        const uint8_t n3 = (uint8_t)p[3] ^ POPBIT(state, 24, 8); \
        const uint8_t n4 = (uint8_t)p[4] ^ POPBIT(state, 32, 8); \
        const uint8_t n5 = (uint8_t)p[5] ^ POPBIT(state, 40, 8); \
        const uint8_t n6 = (uint8_t)p[6] ^ POPBIT(state, 48, 8); \
        const uint8_t n7 = (uint8_t)p[7] ^ POPBIT(state, 56, 8); \
        state = SHIFT(state, 64) ^ \
                t[31][POPBIT8(n0, 0, 2)] ^ \
                t[30][POPBIT8(n0, 2, 2)] ^ \
                t[29][POPBIT8(n0, 4, 2)] ^ \
                t[28][POPBIT8(n0, 6, 2)] ^ \
                t[27][POPBIT8(n1, 0, 2)] ^ \
                t[26][POPBIT8(n1, 2, 2)] ^ \
                t[25][POPBIT8(n1, 4, 2)] ^ \
                t[24][POPBIT8(n1, 6, 2)] ^ \
                t[23][POPBIT8(n2, 0, 2)] ^ \
                t[22][POPBIT8(n2, 2, 2)] ^ \
                t[21][POPBIT8(n2, 4, 2)] ^ \
                t[20][POPBIT8(n2, 6, 2)] ^ \
                t[19][POPBIT8(n3, 0, 2)] ^ \
                t[18][POPBIT8(n3, 2, 2)] ^ \
                t[17][POPBIT8(n3, 4, 2)] ^ \
                t[16][POPBIT8(n3, 6, 2)] ^ \
                t[15][POPBIT8(n4, 0, 2)] ^ \
                t[14][POPBIT8(n4, 2, 2)] ^ \
                t[13][POPBIT8(n4, 4, 2)] ^ \
                t[12][POPBIT8(n4, 6, 2)] ^ \
                t[11][POPBIT8(n5, 0, 2)] ^ \
                t[10][POPBIT8(n5, 2, 2)] ^ \
                t[ 9][POPBIT8(n5, 4, 2)] ^ \
                t[ 8][POPBIT8(n5, 6, 2)] ^ \
                t[ 7][POPBIT8(n6, 0, 2)] ^ \
                t[ 6][POPBIT8(n6, 2, 2)] ^ \
                t[ 5][POPBIT8(n6, 4, 2)] ^ \
                t[ 4][POPBIT8(n6, 6, 2)] ^ \
                t[ 3][POPBIT8(n7, 0, 2)] ^ \
                t[ 2][POPBIT8(n7, 2, 2)] ^ \
                t[ 1][POPBIT8(n7, 4, 2)] ^ \
                t[ 0][POPBIT8(n7, 6, 2)]; \
    }, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        const uint8_t n = POPBIT(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[3][POPBIT8(n, 0, 2)] ^ \
                t[2][POPBIT8(n, 2, 2)] ^ \
                t[1][POPBIT8(n, 4, 2)] ^ \
                t[0][POPBIT8(n, 6, 2)];                   \
    });                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY8_DUO_DECL);

    return state;
}

/*
 * Slicing by Quattuorsexaguple Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY16_DUO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[4] = (const CRCEA_TYPE (*)[4])table;

#define CRCEA_BY16_DUO_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 16, {                                             \
        const uint8_t n0  = (uint8_t)p[ 0] ^ POPBIT(state,   0, 8); \
        const uint8_t n1  = (uint8_t)p[ 1] ^ POPBIT(state,   8, 8); \
        const uint8_t n2  = (uint8_t)p[ 2] ^ POPBIT(state,  16, 8); \
        const uint8_t n3  = (uint8_t)p[ 3] ^ POPBIT(state,  24, 8); \
        const uint8_t n4  = (uint8_t)p[ 4] ^ POPBIT(state,  32, 8); \
        const uint8_t n5  = (uint8_t)p[ 5] ^ POPBIT(state,  40, 8); \
        const uint8_t n6  = (uint8_t)p[ 6] ^ POPBIT(state,  48, 8); \
        const uint8_t n7  = (uint8_t)p[ 7] ^ POPBIT(state,  56, 8); \
        const uint8_t n8  = (uint8_t)p[ 8] ^ POPBIT(state,  64, 8); \
        const uint8_t n9  = (uint8_t)p[ 9] ^ POPBIT(state,  72, 8); \
        const uint8_t n10 = (uint8_t)p[10] ^ POPBIT(state,  80, 8); \
        const uint8_t n11 = (uint8_t)p[11] ^ POPBIT(state,  88, 8); \
        const uint8_t n12 = (uint8_t)p[12] ^ POPBIT(state,  96, 8); \
        const uint8_t n13 = (uint8_t)p[13] ^ POPBIT(state, 104, 8); \
        const uint8_t n14 = (uint8_t)p[14] ^ POPBIT(state, 112, 8); \
        const uint8_t n15 = (uint8_t)p[15] ^ POPBIT(state, 120, 8); \
        state = SHIFT(state, 128) ^ \
                t[63][POPBIT8(n0,  0, 2)] ^ \
                t[62][POPBIT8(n0,  2, 2)] ^ \
                t[61][POPBIT8(n0,  4, 2)] ^ \
                t[60][POPBIT8(n0,  6, 2)] ^ \
                t[59][POPBIT8(n1,  0, 2)] ^ \
                t[58][POPBIT8(n1,  2, 2)] ^ \
                t[57][POPBIT8(n1,  4, 2)] ^ \
                t[56][POPBIT8(n1,  6, 2)] ^ \
                t[55][POPBIT8(n2,  0, 2)] ^ \
                t[54][POPBIT8(n2,  2, 2)] ^ \
                t[53][POPBIT8(n2,  4, 2)] ^ \
                t[52][POPBIT8(n2,  6, 2)] ^ \
                t[51][POPBIT8(n3,  0, 2)] ^ \
                t[50][POPBIT8(n3,  2, 2)] ^ \
                t[49][POPBIT8(n3,  4, 2)] ^ \
                t[48][POPBIT8(n3,  6, 2)] ^ \
                t[47][POPBIT8(n4,  0, 2)] ^ \
                t[46][POPBIT8(n4,  2, 2)] ^ \
                t[45][POPBIT8(n4,  4, 2)] ^ \
                t[44][POPBIT8(n4,  6, 2)] ^ \
                t[43][POPBIT8(n5,  0, 2)] ^ \
                t[42][POPBIT8(n5,  2, 2)] ^ \
                t[41][POPBIT8(n5,  4, 2)] ^ \
                t[40][POPBIT8(n5,  6, 2)] ^ \
                t[39][POPBIT8(n6,  0, 2)] ^ \
                t[38][POPBIT8(n6,  2, 2)] ^ \
                t[37][POPBIT8(n6,  4, 2)] ^ \
                t[36][POPBIT8(n6,  6, 2)] ^ \
                t[35][POPBIT8(n7,  0, 2)] ^ \
                t[34][POPBIT8(n7,  2, 2)] ^ \
                t[33][POPBIT8(n7,  4, 2)] ^ \
                t[32][POPBIT8(n7,  6, 2)] ^ \
                t[31][POPBIT8(n8,  0, 2)] ^ \
                t[30][POPBIT8(n8,  2, 2)] ^ \
                t[29][POPBIT8(n8,  4, 2)] ^ \
                t[28][POPBIT8(n8,  6, 2)] ^ \
                t[27][POPBIT8(n9,  0, 2)] ^ \
                t[26][POPBIT8(n9,  2, 2)] ^ \
                t[25][POPBIT8(n9,  4, 2)] ^ \
                t[24][POPBIT8(n9,  6, 2)] ^ \
                t[23][POPBIT8(n10, 0, 2)] ^ \
                t[22][POPBIT8(n10, 2, 2)] ^ \
                t[21][POPBIT8(n10, 4, 2)] ^ \
                t[20][POPBIT8(n10, 6, 2)] ^ \
                t[19][POPBIT8(n11, 0, 2)] ^ \
                t[18][POPBIT8(n11, 2, 2)] ^ \
                t[17][POPBIT8(n11, 4, 2)] ^ \
                t[16][POPBIT8(n11, 6, 2)] ^ \
                t[15][POPBIT8(n12, 0, 2)] ^ \
                t[14][POPBIT8(n12, 2, 2)] ^ \
                t[13][POPBIT8(n12, 4, 2)] ^ \
                t[12][POPBIT8(n12, 6, 2)] ^ \
                t[11][POPBIT8(n13, 0, 2)] ^ \
                t[10][POPBIT8(n13, 2, 2)] ^ \
                t[ 9][POPBIT8(n13, 4, 2)] ^ \
                t[ 8][POPBIT8(n13, 6, 2)] ^ \
                t[ 7][POPBIT8(n14, 0, 2)] ^ \
                t[ 6][POPBIT8(n14, 2, 2)] ^ \
                t[ 5][POPBIT8(n14, 4, 2)] ^ \
                t[ 4][POPBIT8(n14, 6, 2)] ^ \
                t[ 3][POPBIT8(n15, 0, 2)] ^ \
                t[ 2][POPBIT8(n15, 2, 2)] ^ \
                t[ 1][POPBIT8(n15, 4, 2)] ^ \
                t[ 0][POPBIT8(n15, 6, 2)]; \
    }, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        const uint8_t n = POPBIT(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[3][POPBIT8(n, 0, 2)] ^ \
                t[2][POPBIT8(n, 2, 2)] ^ \
                t[1][POPBIT8(n, 4, 2)] ^ \
                t[0][POPBIT8(n, 6, 2)];                   \
    });                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY16_DUO_DECL);

    return state;
}

/*
 * Slicing by Octovigcentuple Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY32_DUO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[4] = (const CRCEA_TYPE (*)[4])table;

#define CRCEA_BY32_DUO_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 32, {                                             \
        const uint8_t n0  = (uint8_t)p[ 0] ^ POPBIT(state,   0, 8); \
        const uint8_t n1  = (uint8_t)p[ 1] ^ POPBIT(state,   8, 8); \
        const uint8_t n2  = (uint8_t)p[ 2] ^ POPBIT(state,  16, 8); \
        const uint8_t n3  = (uint8_t)p[ 3] ^ POPBIT(state,  24, 8); \
        const uint8_t n4  = (uint8_t)p[ 4] ^ POPBIT(state,  32, 8); \
        const uint8_t n5  = (uint8_t)p[ 5] ^ POPBIT(state,  40, 8); \
        const uint8_t n6  = (uint8_t)p[ 6] ^ POPBIT(state,  48, 8); \
        const uint8_t n7  = (uint8_t)p[ 7] ^ POPBIT(state,  56, 8); \
        const uint8_t n8  = (uint8_t)p[ 8] ^ POPBIT(state,  64, 8); \
        const uint8_t n9  = (uint8_t)p[ 9] ^ POPBIT(state,  72, 8); \
        const uint8_t n10 = (uint8_t)p[10] ^ POPBIT(state,  80, 8); \
        const uint8_t n11 = (uint8_t)p[11] ^ POPBIT(state,  88, 8); \
        const uint8_t n12 = (uint8_t)p[12] ^ POPBIT(state,  96, 8); \
        const uint8_t n13 = (uint8_t)p[13] ^ POPBIT(state, 104, 8); \
        const uint8_t n14 = (uint8_t)p[14] ^ POPBIT(state, 112, 8); \
        const uint8_t n15 = (uint8_t)p[15] ^ POPBIT(state, 120, 8); \
        const uint8_t n16 = (uint8_t)p[16] ^ POPBIT(state, 128, 8); \
        const uint8_t n17 = (uint8_t)p[17] ^ POPBIT(state, 136, 8); \
        const uint8_t n18 = (uint8_t)p[18] ^ POPBIT(state, 144, 8); \
        const uint8_t n19 = (uint8_t)p[19] ^ POPBIT(state, 152, 8); \
        const uint8_t n20 = (uint8_t)p[20] ^ POPBIT(state, 160, 8); \
        const uint8_t n21 = (uint8_t)p[21] ^ POPBIT(state, 168, 8); \
        const uint8_t n22 = (uint8_t)p[22] ^ POPBIT(state, 176, 8); \
        const uint8_t n23 = (uint8_t)p[23] ^ POPBIT(state, 184, 8); \
        const uint8_t n24 = (uint8_t)p[24] ^ POPBIT(state, 192, 8); \
        const uint8_t n25 = (uint8_t)p[25] ^ POPBIT(state, 200, 8); \
        const uint8_t n26 = (uint8_t)p[26] ^ POPBIT(state, 208, 8); \
        const uint8_t n27 = (uint8_t)p[27] ^ POPBIT(state, 216, 8); \
        const uint8_t n28 = (uint8_t)p[28] ^ POPBIT(state, 224, 8); \
        const uint8_t n29 = (uint8_t)p[29] ^ POPBIT(state, 232, 8); \
        const uint8_t n30 = (uint8_t)p[30] ^ POPBIT(state, 240, 8); \
        const uint8_t n31 = (uint8_t)p[31] ^ POPBIT(state, 248, 8); \
        state = SHIFT(state, 256) ^ \
                t[127][POPBIT8(n0,  0, 2)] ^ \
                t[126][POPBIT8(n0,  2, 2)] ^ \
                t[125][POPBIT8(n0,  4, 2)] ^ \
                t[124][POPBIT8(n0,  6, 2)] ^ \
                t[123][POPBIT8(n1,  0, 2)] ^ \
                t[122][POPBIT8(n1,  2, 2)] ^ \
                t[121][POPBIT8(n1,  4, 2)] ^ \
                t[120][POPBIT8(n1,  6, 2)] ^ \
                t[119][POPBIT8(n2,  0, 2)] ^ \
                t[118][POPBIT8(n2,  2, 2)] ^ \
                t[117][POPBIT8(n2,  4, 2)] ^ \
                t[116][POPBIT8(n2,  6, 2)] ^ \
                t[115][POPBIT8(n3,  0, 2)] ^ \
                t[114][POPBIT8(n3,  2, 2)] ^ \
                t[113][POPBIT8(n3,  4, 2)] ^ \
                t[112][POPBIT8(n3,  6, 2)] ^ \
                t[111][POPBIT8(n4,  0, 2)] ^ \
                t[110][POPBIT8(n4,  2, 2)] ^ \
                t[109][POPBIT8(n4,  4, 2)] ^ \
                t[108][POPBIT8(n4,  6, 2)] ^ \
                t[107][POPBIT8(n5,  0, 2)] ^ \
                t[106][POPBIT8(n5,  2, 2)] ^ \
                t[105][POPBIT8(n5,  4, 2)] ^ \
                t[104][POPBIT8(n5,  6, 2)] ^ \
                t[103][POPBIT8(n6,  0, 2)] ^ \
                t[102][POPBIT8(n6,  2, 2)] ^ \
                t[101][POPBIT8(n6,  4, 2)] ^ \
                t[100][POPBIT8(n6,  6, 2)] ^ \
                t[ 99][POPBIT8(n7,  0, 2)] ^ \
                t[ 98][POPBIT8(n7,  2, 2)] ^ \
                t[ 97][POPBIT8(n7,  4, 2)] ^ \
                t[ 96][POPBIT8(n7,  6, 2)] ^ \
                t[ 95][POPBIT8(n8,  0, 2)] ^ \
                t[ 94][POPBIT8(n8,  2, 2)] ^ \
                t[ 93][POPBIT8(n8,  4, 2)] ^ \
                t[ 92][POPBIT8(n8,  6, 2)] ^ \
                t[ 91][POPBIT8(n9,  0, 2)] ^ \
                t[ 90][POPBIT8(n9,  2, 2)] ^ \
                t[ 89][POPBIT8(n9,  4, 2)] ^ \
                t[ 88][POPBIT8(n9,  6, 2)] ^ \
                t[ 87][POPBIT8(n10, 0, 2)] ^ \
                t[ 86][POPBIT8(n10, 2, 2)] ^ \
                t[ 85][POPBIT8(n10, 4, 2)] ^ \
                t[ 84][POPBIT8(n10, 6, 2)] ^ \
                t[ 83][POPBIT8(n11, 0, 2)] ^ \
                t[ 82][POPBIT8(n11, 2, 2)] ^ \
                t[ 81][POPBIT8(n11, 4, 2)] ^ \
                t[ 80][POPBIT8(n11, 6, 2)] ^ \
                t[ 79][POPBIT8(n12, 0, 2)] ^ \
                t[ 78][POPBIT8(n12, 2, 2)] ^ \
                t[ 77][POPBIT8(n12, 4, 2)] ^ \
                t[ 76][POPBIT8(n12, 6, 2)] ^ \
                t[ 75][POPBIT8(n13, 0, 2)] ^ \
                t[ 74][POPBIT8(n13, 2, 2)] ^ \
                t[ 73][POPBIT8(n13, 4, 2)] ^ \
                t[ 72][POPBIT8(n13, 6, 2)] ^ \
                t[ 71][POPBIT8(n14, 0, 2)] ^ \
                t[ 70][POPBIT8(n14, 2, 2)] ^ \
                t[ 69][POPBIT8(n14, 4, 2)] ^ \
                t[ 68][POPBIT8(n14, 6, 2)] ^ \
                t[ 67][POPBIT8(n15, 0, 2)] ^ \
                t[ 66][POPBIT8(n15, 2, 2)] ^ \
                t[ 65][POPBIT8(n15, 4, 2)] ^ \
                t[ 64][POPBIT8(n15, 6, 2)] ^ \
                t[ 63][POPBIT8(n16, 0, 2)] ^ \
                t[ 62][POPBIT8(n16, 2, 2)] ^ \
                t[ 61][POPBIT8(n16, 4, 2)] ^ \
                t[ 60][POPBIT8(n16, 6, 2)] ^ \
                t[ 59][POPBIT8(n17, 0, 2)] ^ \
                t[ 58][POPBIT8(n17, 2, 2)] ^ \
                t[ 57][POPBIT8(n17, 4, 2)] ^ \
                t[ 56][POPBIT8(n17, 6, 2)] ^ \
                t[ 55][POPBIT8(n18, 0, 2)] ^ \
                t[ 54][POPBIT8(n18, 2, 2)] ^ \
                t[ 53][POPBIT8(n18, 4, 2)] ^ \
                t[ 52][POPBIT8(n18, 6, 2)] ^ \
                t[ 51][POPBIT8(n19, 0, 2)] ^ \
                t[ 50][POPBIT8(n19, 2, 2)] ^ \
                t[ 49][POPBIT8(n19, 4, 2)] ^ \
                t[ 48][POPBIT8(n19, 6, 2)] ^ \
                t[ 47][POPBIT8(n20, 0, 2)] ^ \
                t[ 46][POPBIT8(n20, 2, 2)] ^ \
                t[ 45][POPBIT8(n20, 4, 2)] ^ \
                t[ 44][POPBIT8(n20, 6, 2)] ^ \
                t[ 43][POPBIT8(n21, 0, 2)] ^ \
                t[ 42][POPBIT8(n21, 2, 2)] ^ \
                t[ 41][POPBIT8(n21, 4, 2)] ^ \
                t[ 40][POPBIT8(n21, 6, 2)] ^ \
                t[ 39][POPBIT8(n22, 0, 2)] ^ \
                t[ 38][POPBIT8(n22, 2, 2)] ^ \
                t[ 37][POPBIT8(n22, 4, 2)] ^ \
                t[ 36][POPBIT8(n22, 6, 2)] ^ \
                t[ 35][POPBIT8(n23, 0, 2)] ^ \
                t[ 34][POPBIT8(n23, 2, 2)] ^ \
                t[ 33][POPBIT8(n23, 4, 2)] ^ \
                t[ 32][POPBIT8(n23, 6, 2)] ^ \
                t[ 31][POPBIT8(n24, 0, 2)] ^ \
                t[ 30][POPBIT8(n24, 2, 2)] ^ \
                t[ 29][POPBIT8(n24, 4, 2)] ^ \
                t[ 28][POPBIT8(n24, 6, 2)] ^ \
                t[ 27][POPBIT8(n25, 0, 2)] ^ \
                t[ 26][POPBIT8(n25, 2, 2)] ^ \
                t[ 25][POPBIT8(n25, 4, 2)] ^ \
                t[ 24][POPBIT8(n25, 6, 2)] ^ \
                t[ 23][POPBIT8(n26, 0, 2)] ^ \
                t[ 22][POPBIT8(n26, 2, 2)] ^ \
                t[ 21][POPBIT8(n26, 4, 2)] ^ \
                t[ 20][POPBIT8(n26, 6, 2)] ^ \
                t[ 19][POPBIT8(n27, 0, 2)] ^ \
                t[ 18][POPBIT8(n27, 2, 2)] ^ \
                t[ 17][POPBIT8(n27, 4, 2)] ^ \
                t[ 16][POPBIT8(n27, 6, 2)] ^ \
                t[ 15][POPBIT8(n28, 0, 2)] ^ \
                t[ 14][POPBIT8(n28, 2, 2)] ^ \
                t[ 13][POPBIT8(n28, 4, 2)] ^ \
                t[ 12][POPBIT8(n28, 6, 2)] ^ \
                t[ 11][POPBIT8(n29, 0, 2)] ^ \
                t[ 10][POPBIT8(n29, 2, 2)] ^ \
                t[  9][POPBIT8(n29, 4, 2)] ^ \
                t[  8][POPBIT8(n29, 6, 2)] ^ \
                t[  7][POPBIT8(n30, 0, 2)] ^ \
                t[  6][POPBIT8(n30, 2, 2)] ^ \
                t[  5][POPBIT8(n30, 4, 2)] ^ \
                t[  4][POPBIT8(n30, 6, 2)] ^ \
                t[  3][POPBIT8(n31, 0, 2)] ^ \
                t[  2][POPBIT8(n31, 2, 2)] ^ \
                t[  1][POPBIT8(n31, 4, 2)] ^ \
                t[  0][POPBIT8(n31, 6, 2)]; \
    }, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        const uint8_t n = POPBIT(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[3][POPBIT8(n, 0, 2)] ^ \
                t[2][POPBIT8(n, 2, 2)] ^ \
                t[1][POPBIT8(n, 4, 2)] ^ \
                t[0][POPBIT8(n, 6, 2)];                   \
    });                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY32_DUO_DECL);

    return state;
}

/*
 * Slicing by Single Quartet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE *t = (const CRCEA_TYPE *)table;

#define CRCEA_BY_QUARTET_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 1, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        state = SHIFT(state, 4) ^ t[POPBIT(state, 0, 4)];                   \
        state = SHIFT(state, 4) ^ t[POPBIT(state, 0, 4)];                   \
    }, );                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY_QUARTET_DECL);

    return state;
}

/*
 * Slicing by Double Quartet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY1_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[16] = (const CRCEA_TYPE (*)[16])table;

#define CRCEA_BY1_QUARTET_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 1, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        const uint8_t n = POPBIT(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[1][POPBIT8(n, 0, 4)] ^ \
                t[0][POPBIT8(n, 4, 4)];                   \
    }, );                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY1_QUARTET_DECL);

    return state;
}

/*
 * Slicing by Quadruple Quartet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY2_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[16] = (const CRCEA_TYPE (*)[16])table;

#define CRCEA_BY2_QUARTET_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 2, {                                             \
        const uint8_t n0 = (uint8_t)p[0] ^ POPBIT(state, 0, 8); \
        const uint8_t n1 = (uint8_t)p[1] ^ POPBIT(state, 8, 8); \
        state = SHIFT(state, 16) ^ \
                t[3][POPBIT8(n0, 0, 4)] ^ \
                t[2][POPBIT8(n0, 4, 4)] ^ \
                t[1][POPBIT8(n1, 0, 4)] ^ \
                t[0][POPBIT8(n1, 4, 4)]; \
    }, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        const uint8_t n = POPBIT(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[1][POPBIT8(n, 0, 4)] ^ \
                t[0][POPBIT8(n, 4, 4)];                   \
    });                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY2_QUARTET_DECL);

    return state;
}

/*
 * Slicing by Octuple Quartet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY4_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[16] = (const CRCEA_TYPE (*)[16])table;

#define CRCEA_BY4_QUARTET_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 4, {                                             \
        const uint8_t n0 = (uint8_t)p[0] ^ (uint8_t)POPBIT(state,  0, 8); \
        const uint8_t n1 = (uint8_t)p[1] ^ (uint8_t)POPBIT(state,  8, 8); \
        const uint8_t n2 = (uint8_t)p[2] ^ (uint8_t)POPBIT(state, 16, 8); \
        const uint8_t n3 = (uint8_t)p[3] ^ (uint8_t)POPBIT(state, 24, 8); \
        state = SHIFT(state, 32) ^ \
                t[7][POPBIT8(n0, 0, 4)] ^ \
                t[6][POPBIT8(n0, 4, 4)] ^ \
                t[5][POPBIT8(n1, 0, 4)] ^ \
                t[4][POPBIT8(n1, 4, 4)] ^ \
                t[3][POPBIT8(n2, 0, 4)] ^ \
                t[2][POPBIT8(n2, 4, 4)] ^ \
                t[1][POPBIT8(n3, 0, 4)] ^ \
                t[0][POPBIT8(n3, 4, 4)]; \
    }, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        const uint8_t n = POPBIT(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[1][POPBIT8(n, 0, 4)] ^ \
                t[0][POPBIT8(n, 4, 4)];                   \
    });                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY4_QUARTET_DECL);

    return state;
}

/*
 * Slicing by Sexdecuple Quartet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY8_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[16] = (const CRCEA_TYPE (*)[16])table;

#define CRCEA_BY8_QUARTET_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 8, {                                             \
        const uint8_t n0 = (uint8_t)p[0] ^ (uint8_t)POPBIT(state,  0, 8); \
        const uint8_t n1 = (uint8_t)p[1] ^ (uint8_t)POPBIT(state,  8, 8); \
        const uint8_t n2 = (uint8_t)p[2] ^ (uint8_t)POPBIT(state, 16, 8); \
        const uint8_t n3 = (uint8_t)p[3] ^ (uint8_t)POPBIT(state, 24, 8); \
        const uint8_t n4 = (uint8_t)p[4] ^ (uint8_t)POPBIT(state, 32, 8); \
        const uint8_t n5 = (uint8_t)p[5] ^ (uint8_t)POPBIT(state, 40, 8); \
        const uint8_t n6 = (uint8_t)p[6] ^ (uint8_t)POPBIT(state, 48, 8); \
        const uint8_t n7 = (uint8_t)p[7] ^ (uint8_t)POPBIT(state, 56, 8); \
        state = SHIFT(state, 64) ^ \
                t[15][POPBIT8(n0, 0, 4)] ^ \
                t[14][POPBIT8(n0, 4, 4)] ^ \
                t[13][POPBIT8(n1, 0, 4)] ^ \
                t[12][POPBIT8(n1, 4, 4)] ^ \
                t[11][POPBIT8(n2, 0, 4)] ^ \
                t[10][POPBIT8(n2, 4, 4)] ^ \
                t[ 9][POPBIT8(n3, 0, 4)] ^ \
                t[ 8][POPBIT8(n3, 4, 4)] ^ \
                t[ 7][POPBIT8(n4, 0, 4)] ^ \
                t[ 6][POPBIT8(n4, 4, 4)] ^ \
                t[ 5][POPBIT8(n5, 0, 4)] ^ \
                t[ 4][POPBIT8(n5, 4, 4)] ^ \
                t[ 3][POPBIT8(n6, 0, 4)] ^ \
                t[ 2][POPBIT8(n6, 4, 4)] ^ \
                t[ 1][POPBIT8(n7, 0, 4)] ^ \
                t[ 0][POPBIT8(n7, 4, 4)]; \
    }, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        const uint8_t n = POPBIT(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[1][POPBIT8(n, 0, 4)] ^ \
                t[0][POPBIT8(n, 4, 4)];                   \
    });                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY8_QUARTET_DECL);

    return state;
}

/*
 * Slicing by Duotriguple Quartet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY16_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[16] = (const CRCEA_TYPE (*)[16])table;

#define CRCEA_BY16_QUARTET_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 16, {                                             \
        const uint8_t n0  = (uint8_t)p[ 0] ^ (uint8_t)POPBIT(state,   0, 8); \
        const uint8_t n1  = (uint8_t)p[ 1] ^ (uint8_t)POPBIT(state,   8, 8); \
        const uint8_t n2  = (uint8_t)p[ 2] ^ (uint8_t)POPBIT(state,  16, 8); \
        const uint8_t n3  = (uint8_t)p[ 3] ^ (uint8_t)POPBIT(state,  24, 8); \
        const uint8_t n4  = (uint8_t)p[ 4] ^ (uint8_t)POPBIT(state,  32, 8); \
        const uint8_t n5  = (uint8_t)p[ 5] ^ (uint8_t)POPBIT(state,  40, 8); \
        const uint8_t n6  = (uint8_t)p[ 6] ^ (uint8_t)POPBIT(state,  48, 8); \
        const uint8_t n7  = (uint8_t)p[ 7] ^ (uint8_t)POPBIT(state,  56, 8); \
        const uint8_t n8  = (uint8_t)p[ 8] ^ (uint8_t)POPBIT(state,  64, 8); \
        const uint8_t n9  = (uint8_t)p[ 9] ^ (uint8_t)POPBIT(state,  72, 8); \
        const uint8_t n10 = (uint8_t)p[10] ^ (uint8_t)POPBIT(state,  80, 8); \
        const uint8_t n11 = (uint8_t)p[11] ^ (uint8_t)POPBIT(state,  88, 8); \
        const uint8_t n12 = (uint8_t)p[12] ^ (uint8_t)POPBIT(state,  96, 8); \
        const uint8_t n13 = (uint8_t)p[13] ^ (uint8_t)POPBIT(state, 104, 8); \
        const uint8_t n14 = (uint8_t)p[14] ^ (uint8_t)POPBIT(state, 112, 8); \
        const uint8_t n15 = (uint8_t)p[15] ^ (uint8_t)POPBIT(state, 120, 8); \
        state = SHIFT(state, 128) ^ \
                t[31][POPBIT8(n0,  0, 4)] ^ \
                t[30][POPBIT8(n0,  4, 4)] ^ \
                t[29][POPBIT8(n1,  0, 4)] ^ \
                t[28][POPBIT8(n1,  4, 4)] ^ \
                t[27][POPBIT8(n2,  0, 4)] ^ \
                t[26][POPBIT8(n2,  4, 4)] ^ \
                t[25][POPBIT8(n3,  0, 4)] ^ \
                t[24][POPBIT8(n3,  4, 4)] ^ \
                t[23][POPBIT8(n4,  0, 4)] ^ \
                t[22][POPBIT8(n4,  4, 4)] ^ \
                t[21][POPBIT8(n5,  0, 4)] ^ \
                t[20][POPBIT8(n5,  4, 4)] ^ \
                t[19][POPBIT8(n6,  0, 4)] ^ \
                t[18][POPBIT8(n6,  4, 4)] ^ \
                t[17][POPBIT8(n7,  0, 4)] ^ \
                t[16][POPBIT8(n7,  4, 4)] ^ \
                t[15][POPBIT8(n8,  0, 4)] ^ \
                t[14][POPBIT8(n8,  4, 4)] ^ \
                t[13][POPBIT8(n9,  0, 4)] ^ \
                t[12][POPBIT8(n9,  4, 4)] ^ \
                t[11][POPBIT8(n10, 0, 4)] ^ \
                t[10][POPBIT8(n10, 4, 4)] ^ \
                t[ 9][POPBIT8(n11, 0, 4)] ^ \
                t[ 8][POPBIT8(n11, 4, 4)] ^ \
                t[ 7][POPBIT8(n12, 0, 4)] ^ \
                t[ 6][POPBIT8(n12, 4, 4)] ^ \
                t[ 5][POPBIT8(n13, 0, 4)] ^ \
                t[ 4][POPBIT8(n13, 4, 4)] ^ \
                t[ 3][POPBIT8(n14, 0, 4)] ^ \
                t[ 2][POPBIT8(n14, 4, 4)] ^ \
                t[ 1][POPBIT8(n15, 0, 4)] ^ \
                t[ 0][POPBIT8(n15, 4, 4)]; \
    }, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        const uint8_t n = POPBIT(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[1][POPBIT8(n, 0, 4)] ^ \
                t[0][POPBIT8(n, 4, 4)];                   \
    });                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY16_QUARTET_DECL);

    return state;
}

/*
 * Slicing by Quattuorsexaguple Quartet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY32_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[16] = (const CRCEA_TYPE (*)[16])table;

#define CRCEA_BY32_QUARTET_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 32, {                                             \
        const uint8_t n0  = (uint8_t)p[ 0] ^ (uint8_t)POPBIT(state,   0, 8); \
        const uint8_t n1  = (uint8_t)p[ 1] ^ (uint8_t)POPBIT(state,   8, 8); \
        const uint8_t n2  = (uint8_t)p[ 2] ^ (uint8_t)POPBIT(state,  16, 8); \
        const uint8_t n3  = (uint8_t)p[ 3] ^ (uint8_t)POPBIT(state,  24, 8); \
        const uint8_t n4  = (uint8_t)p[ 4] ^ (uint8_t)POPBIT(state,  32, 8); \
        const uint8_t n5  = (uint8_t)p[ 5] ^ (uint8_t)POPBIT(state,  40, 8); \
        const uint8_t n6  = (uint8_t)p[ 6] ^ (uint8_t)POPBIT(state,  48, 8); \
        const uint8_t n7  = (uint8_t)p[ 7] ^ (uint8_t)POPBIT(state,  56, 8); \
        const uint8_t n8  = (uint8_t)p[ 8] ^ (uint8_t)POPBIT(state,  64, 8); \
        const uint8_t n9  = (uint8_t)p[ 9] ^ (uint8_t)POPBIT(state,  72, 8); \
        const uint8_t n10 = (uint8_t)p[10] ^ (uint8_t)POPBIT(state,  80, 8); \
        const uint8_t n11 = (uint8_t)p[11] ^ (uint8_t)POPBIT(state,  88, 8); \
        const uint8_t n12 = (uint8_t)p[12] ^ (uint8_t)POPBIT(state,  96, 8); \
        const uint8_t n13 = (uint8_t)p[13] ^ (uint8_t)POPBIT(state, 104, 8); \
        const uint8_t n14 = (uint8_t)p[14] ^ (uint8_t)POPBIT(state, 112, 8); \
        const uint8_t n15 = (uint8_t)p[15] ^ (uint8_t)POPBIT(state, 120, 8); \
        const uint8_t n16 = (uint8_t)p[16] ^ (uint8_t)POPBIT(state, 128, 8); \
        const uint8_t n17 = (uint8_t)p[17] ^ (uint8_t)POPBIT(state, 136, 8); \
        const uint8_t n18 = (uint8_t)p[18] ^ (uint8_t)POPBIT(state, 144, 8); \
        const uint8_t n19 = (uint8_t)p[19] ^ (uint8_t)POPBIT(state, 152, 8); \
        const uint8_t n20 = (uint8_t)p[20] ^ (uint8_t)POPBIT(state, 160, 8); \
        const uint8_t n21 = (uint8_t)p[21] ^ (uint8_t)POPBIT(state, 168, 8); \
        const uint8_t n22 = (uint8_t)p[22] ^ (uint8_t)POPBIT(state, 176, 8); \
        const uint8_t n23 = (uint8_t)p[23] ^ (uint8_t)POPBIT(state, 184, 8); \
        const uint8_t n24 = (uint8_t)p[24] ^ (uint8_t)POPBIT(state, 192, 8); \
        const uint8_t n25 = (uint8_t)p[25] ^ (uint8_t)POPBIT(state, 200, 8); \
        const uint8_t n26 = (uint8_t)p[26] ^ (uint8_t)POPBIT(state, 208, 8); \
        const uint8_t n27 = (uint8_t)p[27] ^ (uint8_t)POPBIT(state, 216, 8); \
        const uint8_t n28 = (uint8_t)p[28] ^ (uint8_t)POPBIT(state, 224, 8); \
        const uint8_t n29 = (uint8_t)p[29] ^ (uint8_t)POPBIT(state, 232, 8); \
        const uint8_t n30 = (uint8_t)p[30] ^ (uint8_t)POPBIT(state, 240, 8); \
        const uint8_t n31 = (uint8_t)p[31] ^ (uint8_t)POPBIT(state, 248, 8); \
        state = SHIFT(state, 256) ^ \
                t[63][POPBIT8(n0,  0, 4)] ^ \
                t[62][POPBIT8(n0,  4, 4)] ^ \
                t[61][POPBIT8(n1,  0, 4)] ^ \
                t[60][POPBIT8(n1,  4, 4)] ^ \
                t[59][POPBIT8(n2,  0, 4)] ^ \
                t[58][POPBIT8(n2,  4, 4)] ^ \
                t[57][POPBIT8(n3,  0, 4)] ^ \
                t[56][POPBIT8(n3,  4, 4)] ^ \
                t[55][POPBIT8(n4,  0, 4)] ^ \
                t[54][POPBIT8(n4,  4, 4)] ^ \
                t[53][POPBIT8(n5,  0, 4)] ^ \
                t[52][POPBIT8(n5,  4, 4)] ^ \
                t[51][POPBIT8(n6,  0, 4)] ^ \
                t[50][POPBIT8(n6,  4, 4)] ^ \
                t[49][POPBIT8(n7,  0, 4)] ^ \
                t[48][POPBIT8(n7,  4, 4)] ^ \
                t[47][POPBIT8(n8,  0, 4)] ^ \
                t[46][POPBIT8(n8,  4, 4)] ^ \
                t[45][POPBIT8(n9,  0, 4)] ^ \
                t[44][POPBIT8(n9,  4, 4)] ^ \
                t[43][POPBIT8(n10, 0, 4)] ^ \
                t[42][POPBIT8(n10, 4, 4)] ^ \
                t[41][POPBIT8(n11, 0, 4)] ^ \
                t[40][POPBIT8(n11, 4, 4)] ^ \
                t[39][POPBIT8(n12, 0, 4)] ^ \
                t[38][POPBIT8(n12, 4, 4)] ^ \
                t[37][POPBIT8(n13, 0, 4)] ^ \
                t[36][POPBIT8(n13, 4, 4)] ^ \
                t[35][POPBIT8(n14, 0, 4)] ^ \
                t[34][POPBIT8(n14, 4, 4)] ^ \
                t[33][POPBIT8(n15, 0, 4)] ^ \
                t[32][POPBIT8(n15, 4, 4)] ^ \
                t[31][POPBIT8(n16, 0, 4)] ^ \
                t[30][POPBIT8(n16, 4, 4)] ^ \
                t[29][POPBIT8(n17, 0, 4)] ^ \
                t[28][POPBIT8(n17, 4, 4)] ^ \
                t[27][POPBIT8(n18, 0, 4)] ^ \
                t[26][POPBIT8(n18, 4, 4)] ^ \
                t[25][POPBIT8(n19, 0, 4)] ^ \
                t[24][POPBIT8(n19, 4, 4)] ^ \
                t[23][POPBIT8(n20, 0, 4)] ^ \
                t[22][POPBIT8(n20, 4, 4)] ^ \
                t[21][POPBIT8(n21, 0, 4)] ^ \
                t[20][POPBIT8(n21, 4, 4)] ^ \
                t[19][POPBIT8(n22, 0, 4)] ^ \
                t[18][POPBIT8(n22, 4, 4)] ^ \
                t[17][POPBIT8(n23, 0, 4)] ^ \
                t[16][POPBIT8(n23, 4, 4)] ^ \
                t[15][POPBIT8(n24, 0, 4)] ^ \
                t[14][POPBIT8(n24, 4, 4)] ^ \
                t[13][POPBIT8(n25, 0, 4)] ^ \
                t[12][POPBIT8(n25, 4, 4)] ^ \
                t[11][POPBIT8(n26, 0, 4)] ^ \
                t[10][POPBIT8(n26, 4, 4)] ^ \
                t[ 9][POPBIT8(n27, 0, 4)] ^ \
                t[ 8][POPBIT8(n27, 4, 4)] ^ \
                t[ 7][POPBIT8(n28, 0, 4)] ^ \
                t[ 6][POPBIT8(n28, 4, 4)] ^ \
                t[ 5][POPBIT8(n29, 0, 4)] ^ \
                t[ 4][POPBIT8(n29, 4, 4)] ^ \
                t[ 3][POPBIT8(n30, 0, 4)] ^ \
                t[ 2][POPBIT8(n30, 4, 4)] ^ \
                t[ 1][POPBIT8(n31, 0, 4)] ^ \
                t[ 0][POPBIT8(n31, 4, 4)]; \
    }, {                                             \
        state ^= SHIFT_INPUT(*p);                                           \
        const uint8_t n = POPBIT(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[1][POPBIT8(n, 0, 4)] ^ \
                t[0][POPBIT8(n, 4, 4)];                   \
    });                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY32_QUARTET_DECL);

    return state;
}

/*
 * Slicing by Single Octet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY1_OCTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE *t = (const CRCEA_TYPE *)table;

#define CRCEA_BY1_OCTET_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 1, {                                             \
        state = SHIFT(state, 8) ^ t[(uint8_t)*p ^ POPBIT(state, 0, 8)];     \
    }, );                                                                   \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY1_OCTET_DECL);

    return state;
}

/*
 * Slicing by Double Octet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY2_OCTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[256] = (const CRCEA_TYPE (*)[256])table;

#define CRCEA_BY2_OCTET_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 2, {                                             \
        state = SHIFT(state, 16) ^                                          \
                t[1][(uint8_t)p[0] ^ POPBIT(state,  0, 8)] ^                \
                t[0][(uint8_t)p[1] ^ POPBIT(state,  8, 8)];                 \
    }, {                                                                    \
        state = SHIFT(state, 8) ^ t[0][(uint8_t)*p ^ POPBIT(state, 0, 8)];  \
    });                                                                     \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY2_OCTET_DECL);

    return state;
}

/*
 * Slicing by Quadruple Octet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY4_OCTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[256] = (const CRCEA_TYPE (*)[256])table;

#define CRCEA_BY4_OCTET_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 4, {                                             \
        state = SHIFT(state, 32) ^                                          \
                t[3][(uint8_t)p[0] ^ POPBIT(state,  0, 8)] ^                \
                t[2][(uint8_t)p[1] ^ POPBIT(state,  8, 8)] ^                \
                t[1][(uint8_t)p[2] ^ POPBIT(state, 16, 8)] ^                \
                t[0][(uint8_t)p[3] ^ POPBIT(state, 24, 8)];                 \
    }, {                                                                    \
        state = SHIFT(state, 8) ^ t[0][(uint8_t)*p ^ POPBIT(state, 0, 8)];  \
    });                                                                     \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY4_OCTET_DECL);

    return state;
}

/*
 * Slicing by Quadruple Octet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY8_OCTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[256] = (const CRCEA_TYPE (*)[256])table;

#define CRCEA_BY8_OCTET_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
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

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY8_OCTET_DECL);

    return state;
}

/*
 * Slicing by Sexdecuple Octet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY16_OCTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[256] = (const CRCEA_TYPE (*)[256])table;

#define CRCEA_BY16_OCTET_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
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

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY16_OCTET_DECL);

    return state;
}

/*
 * Slicing by Slicing by Duotriguple Octet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY32_OCTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[256] = (const CRCEA_TYPE (*)[256])table;

#define CRCEA_BY32_OCTET_DECL(SETUP_POLYNOMIAL, SHIFT_INPUT, SHIFT, POPBIT, POPBIT8, LOAD16, INDEX16) \
    CRCEA_UPDATE_CORE(p, pp, 32, {                                            \
        state = SHIFT(state, 256) ^                                         \
                t[31][(uint8_t)p[ 0] ^ POPBIT(state,   0, 8)] ^             \
                t[30][(uint8_t)p[ 1] ^ POPBIT(state,   8, 8)] ^             \
                t[29][(uint8_t)p[ 2] ^ POPBIT(state,  16, 8)] ^             \
                t[28][(uint8_t)p[ 3] ^ POPBIT(state,  24, 8)] ^             \
                t[27][(uint8_t)p[ 4] ^ POPBIT(state,  32, 8)] ^             \
                t[26][(uint8_t)p[ 5] ^ POPBIT(state,  40, 8)] ^             \
                t[25][(uint8_t)p[ 6] ^ POPBIT(state,  48, 8)] ^             \
                t[24][(uint8_t)p[ 7] ^ POPBIT(state,  56, 8)] ^             \
                t[23][(uint8_t)p[ 8] ^ POPBIT(state,  64, 8)] ^             \
                t[22][(uint8_t)p[ 9] ^ POPBIT(state,  72, 8)] ^             \
                t[21][(uint8_t)p[10] ^ POPBIT(state,  80, 8)] ^             \
                t[20][(uint8_t)p[11] ^ POPBIT(state,  88, 8)] ^             \
                t[19][(uint8_t)p[12] ^ POPBIT(state,  96, 8)] ^             \
                t[18][(uint8_t)p[13] ^ POPBIT(state, 104, 8)] ^             \
                t[17][(uint8_t)p[14] ^ POPBIT(state, 112, 8)] ^             \
                t[16][(uint8_t)p[15] ^ POPBIT(state, 120, 8)] ^             \
                t[15][(uint8_t)p[16] ^ POPBIT(state, 128, 8)] ^             \
                t[14][(uint8_t)p[17] ^ POPBIT(state, 136, 8)] ^             \
                t[13][(uint8_t)p[18] ^ POPBIT(state, 144, 8)] ^             \
                t[12][(uint8_t)p[19] ^ POPBIT(state, 152, 8)] ^             \
                t[11][(uint8_t)p[20] ^ POPBIT(state, 160, 8)] ^             \
                t[10][(uint8_t)p[21] ^ POPBIT(state, 168, 8)] ^             \
                t[ 9][(uint8_t)p[22] ^ POPBIT(state, 176, 8)] ^             \
                t[ 8][(uint8_t)p[23] ^ POPBIT(state, 184, 8)] ^             \
                t[ 7][(uint8_t)p[24] ^ POPBIT(state, 192, 8)] ^             \
                t[ 6][(uint8_t)p[25] ^ POPBIT(state, 200, 8)] ^             \
                t[ 5][(uint8_t)p[26] ^ POPBIT(state, 208, 8)] ^             \
                t[ 4][(uint8_t)p[27] ^ POPBIT(state, 216, 8)] ^             \
                t[ 3][(uint8_t)p[28] ^ POPBIT(state, 224, 8)] ^             \
                t[ 2][(uint8_t)p[29] ^ POPBIT(state, 232, 8)] ^             \
                t[ 1][(uint8_t)p[30] ^ POPBIT(state, 240, 8)] ^             \
                t[ 0][(uint8_t)p[31] ^ POPBIT(state, 248, 8)];              \
    }, {                                                                    \
        state = SHIFT(state, 8) ^ t[0][(uint8_t)*p ^ POPBIT(state, 0, 8)];  \
    });                                                                     \

    CRCEA_UPDATE_DECL(model, state, CRCEA_BY32_OCTET_DECL);

    return state;
}

CRCEA_VISIBILITY CRCEA_INLINE int
CRCEA_PREPARE_TABLE(crcea_context *cc)
{
    int algo = cc->algorithm;

    if (!cc->table && algo >= CRCEA_TABLE_ALGORITHM) {
        crcea_alloc_f *alloc = cc->alloc;
        if (!alloc) {
#ifdef CRCEA_DEFAULT_MALLOC
            alloc = CRCEA_DEFAULT_MALLOC;
#else
            return CRCEA_BITBYBIT_FAST;
#endif
        }

        if (alloc) {
            void *bufp = alloc(cc, CRCEA_TABLESIZE(algo));
            CRCEA_BUILD_TABLE(cc->model, cc->algorithm, bufp);
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
    case CRCEA_BITBYBIT:
        return CRCEA_UPDATE_BITBYBIT(cc->model, p, pp, state);
    case CRCEA_BITBYBIT_FAST:
        return CRCEA_UPDATE_BITBYBIT_FAST(cc->model, p, pp, state);
    case CRCEA_BY_DUO:
        return CRCEA_UPDATE_BY_DUO(cc->model, p, pp, state, cc->table);
    case CRCEA_BY1_DUO:
        return CRCEA_UPDATE_BY1_DUO(cc->model, p, pp, state, cc->table);
    case CRCEA_BY2_DUO:
        return CRCEA_UPDATE_BY2_DUO(cc->model, p, pp, state, cc->table);
    case CRCEA_BY4_DUO:
        return CRCEA_UPDATE_BY4_DUO(cc->model, p, pp, state, cc->table);
    case CRCEA_BY8_DUO:
        return CRCEA_UPDATE_BY8_DUO(cc->model, p, pp, state, cc->table);
    case CRCEA_BY16_DUO:
        return CRCEA_UPDATE_BY16_DUO(cc->model, p, pp, state, cc->table);
    case CRCEA_BY32_DUO:
        return CRCEA_UPDATE_BY32_DUO(cc->model, p, pp, state, cc->table);
    case CRCEA_BY_QUARTET:
        return CRCEA_UPDATE_BY_QUARTET(cc->model, p, pp, state, cc->table);
    case CRCEA_BY1_QUARTET:
        return CRCEA_UPDATE_BY1_QUARTET(cc->model, p, pp, state, cc->table);
    case CRCEA_BY2_QUARTET:
        return CRCEA_UPDATE_BY2_QUARTET(cc->model, p, pp, state, cc->table);
    case CRCEA_BY4_QUARTET:
        return CRCEA_UPDATE_BY4_QUARTET(cc->model, p, pp, state, cc->table);
    case CRCEA_BY8_QUARTET:
        return CRCEA_UPDATE_BY8_QUARTET(cc->model, p, pp, state, cc->table);
    case CRCEA_BY16_QUARTET:
        return CRCEA_UPDATE_BY16_QUARTET(cc->model, p, pp, state, cc->table);
    case CRCEA_BY32_QUARTET:
        return CRCEA_UPDATE_BY32_QUARTET(cc->model, p, pp, state, cc->table);
    case CRCEA_BY1_OCTET:
        return CRCEA_UPDATE_BY1_OCTET(cc->model, p, pp, state, cc->table);
    case CRCEA_BY2_OCTET:
        return CRCEA_UPDATE_BY2_OCTET(cc->model, p, pp, state, cc->table);
    case CRCEA_BY4_OCTET:
        return CRCEA_UPDATE_BY4_OCTET(cc->model, p, pp, state, cc->table);
    case CRCEA_BY8_OCTET:
        return CRCEA_UPDATE_BY8_OCTET(cc->model, p, pp, state, cc->table);
    case CRCEA_BY16_OCTET:
        return CRCEA_UPDATE_BY16_OCTET(cc->model, p, pp, state, cc->table);
    case CRCEA_BY32_OCTET:
        return CRCEA_UPDATE_BY32_OCTET(cc->model, p, pp, state, cc->table);
    default:
        return CRCEA_UPDATE_BITBYBIT(cc->model, p, pp, state);
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
#undef CRCEA_UPDATE_BY_SOLO
#undef CRCEA_UPDATE_BY1_SOLO
#undef CRCEA_UPDATE_BY2_SOLO
#undef CRCEA_UPDATE_BY4_SOLO
#undef CRCEA_UPDATE_BY8_SOLO
#undef CRCEA_UPDATE_BY16_SOLO
#undef CRCEA_UPDATE_BY32_SOLO
#undef CRCEA_UPDATE_BY_DUO
#undef CRCEA_UPDATE_BY1_DUO
#undef CRCEA_UPDATE_BY2_DUO
#undef CRCEA_UPDATE_BY4_DUO
#undef CRCEA_UPDATE_BY8_DUO
#undef CRCEA_UPDATE_BY16_DUO
#undef CRCEA_UPDATE_BY32_DUO
#undef CRCEA_UPDATE_BY_QUARTET
#undef CRCEA_UPDATE_BY1_QUARTET
#undef CRCEA_UPDATE_BY2_QUARTET
#undef CRCEA_UPDATE_BY4_QUARTET
#undef CRCEA_UPDATE_BY8_QUARTET
#undef CRCEA_UPDATE_BY16_QUARTET
#undef CRCEA_UPDATE_BY32_QUARTET
#undef CRCEA_UPDATE_BY1_OCTET
#undef CRCEA_UPDATE_BY2_OCTET
#undef CRCEA_UPDATE_BY4_OCTET
#undef CRCEA_UPDATE_BY8_OCTET
#undef CRCEA_UPDATE_BY16_OCTET
#undef CRCEA_UPDATE_BY32_OCTET
#undef CRCEA_UPDATE_BY2_SEXDECTET
#undef CRCEA_UPDATE_BY4_SEXDECTET
#undef CRCEA_UPDATE_BY8_SEXDECTET
#undef CRCEA_UPDATE_BY16_SEXDECTET
#undef CRCEA_UPDATE_BY32_SEXDECTET
#undef CRCEA_BITSIZE
#undef CRCEA_LSH
#undef CRCEA_RSH
#undef CRCEA_BITMASK

#undef CRCEA_SETUP_POLYNOMIAL
#undef CRCEA_SHIFT_INPUT
#undef CRCEA_SHIFT
#undef CRCEA_POPBIT
#undef CRCEA_POPBIT8
#undef CRCEA_LOAD16
#undef CRCEA_INDEX16
#undef CRCEA_SETUP_POLYNOMIAL_R
#undef CRCEA_SHIFT_INPUT_R
#undef CRCEA_SHIFT_R
#undef CRCEA_POPBIT_R
#undef CRCEA_POPBIT8_R
#undef CRCEA_LOAD16_R
#undef CRCEA_INDEX16_R
#undef CRCEA_UPDATE_CORE
#undef CRCEA_UPDATE_DECL
#undef CRCEA_BITBYBIT_DECL
#undef CRCEA_BITBYBIT_FAST_DECL
#undef CRCEA_BY_SOLO_DECL
#undef CRCEA_BY1_SOLO_DECL
#undef CRCEA_BY2_SOLO_DECL
#undef CRCEA_BY4_SOLO_DECL
#undef CRCEA_BY8_SOLO_DECL
#undef CRCEA_BY16_SOLO_DECL
#undef CRCEA_BY32_SOLO_DECL
#undef CRCEA_BY_DUO_DECL
#undef CRCEA_BY1_DUO_DECL
#undef CRCEA_BY2_DUO_DECL
#undef CRCEA_BY4_DUO_DECL
#undef CRCEA_BY8_DUO_DECL
#undef CRCEA_BY16_DUO_DECL
#undef CRCEA_BY32_DUO_DECL
#undef CRCEA_BY_QUARTET_DECL
#undef CRCEA_BY1_QUARTET_DECL
#undef CRCEA_BY2_QUARTET_DECL
#undef CRCEA_BY4_QUARTET_DECL
#undef CRCEA_BY8_QUARTET_DECL
#undef CRCEA_BY16_QUARTET_DECL
#undef CRCEA_BY32_QUARTET_DECL
#undef CRCEA_BY1_OCTET_DECL
#undef CRCEA_BY2_OCTET_DECL
#undef CRCEA_BY4_OCTET_DECL
#undef CRCEA_BY8_OCTET_DECL
#undef CRCEA_BY16_OCTET_DECL
#undef CRCEA_BY32_OCTET_DECL
#undef CRCEA_BY2_SEXDECTET_DECL
#undef CRCEA_BY4_SEXDECTET_DECL
#undef CRCEA_BY8_SEXDECTET_DECL
#undef CRCEA_BY16_SEXDECTET_DECL
#undef CRCEA_BY32_SEXDECTET_DECL
