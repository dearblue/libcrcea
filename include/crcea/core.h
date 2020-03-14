/**
 * @file core.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.osdn.me>
 * @license Creative Commons Zero License (CC0 / Public Domain)
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
 *          static const crcea_design design = {
 *              .bitsize = 32,
 *              .polynomial = 0x04c11db7ul,
 *              .reflectin = 1,
 *              .reflectout = 1,
 *              .xoroutput = ~0ul,
 *          };
 *
 *          static crcea_context cc = {
 *              .design = &design,
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
 *          static const crcea_design design = {
 *              .bitsize = 32,
 *              .polynomial = 0x1edc6f41ul,
 *              .reflectin = 1,
 *              .reflectout = 1,
 *              .xoroutput = ~0ul,
 *          };
 *          static crcea_context cc = {
 *              .design = &design,
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
 *          static const crcea_design design = {
 *              .bitsize = 32,
 *              .polynomial = 0x04c11db7ul,
 *              .reflectin = 0,
 *              .reflectout = 0,
 *              .xoroutput = ~0ul,
 *          };
 *          static crcea_context cc = {
 *              .design = &design,
 *              .inttype = CRCEA_INT32,
 *              .algorithm = CRCEA_BITCOMBINE8,
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
# error Please definision both CRCEA_PREFIX and CRCEA_TYPE macros in before include this file.
#endif

#include "defs.h"

#ifndef CHAR_BIT
# include <limits.h>
#endif


#ifndef CRCEA_VISIBILITY
# define CRCEA_VISIBILITY static
#endif

#ifndef CRCEA_INLINE
# define CRCEA_INLINE inline
#endif

#ifndef CRCEA_NO_MALLOC
# ifndef CRCEA_MALLOC
#  include <stdlib.h>
#  define CRCEA_MALLOC malloc
# endif
#endif

#ifndef CRCEA_STRIPE_SIZE
# define CRCEA_STRIPE_SIZE 1
#endif

#define CRCEA_TOKEN__2(PREFIX, NAME)    PREFIX ## NAME
#define CRCEA_TOKEN__1(PREFIX, NAME)    CRCEA_TOKEN__2(PREFIX, NAME)
#define CRCEA_TOKEN(NAME)               CRCEA_TOKEN__1(CRCEA_PREFIX, NAME)
#define CRCEA_BITREFLECT                CRCEA_TOKEN(_bitreflect)
#define CRCEA_SETUP                     CRCEA_TOKEN(_setup)
#define CRCEA_FINISH                    CRCEA_TOKEN(_finish)
#define CRCEA_UPDATE                    CRCEA_TOKEN(_update)
#define CRCEA_PREPARE_TABLE             CRCEA_TOKEN(_prepare_table)
#define CRCEA_TABLESIZE                 CRCEA_TOKEN(_tablesize)
#define CRCEA_BUILD_TABLE               CRCEA_TOKEN(_build_table)
#define CRCEA_INPUT_TO_STATE            CRCEA_TOKEN(_input_to_state)
#define CRCEA_UPDATE_SHIFT              CRCEA_TOKEN(_update_shift)
#define CRCEA_UPDATE_UNIFIED            CRCEA_TOKEN(_update_unified)
#define CRCEA_UPDATE_REFERENCE          CRCEA_TOKEN(_update_reference)
#define CRCEA_UPDATE_BITWISE_CONDXOR    CRCEA_TOKEN(_update_bitwise_condxor)
#define CRCEA_UPDATE_BITWISE_BRANCHASSIGN CRCEA_TOKEN(_update_bitwise_branchassign)
#define CRCEA_UPDATE_BITWISE_BRANCHMIX  CRCEA_TOKEN(_update_bitwise_branchmix)
#define CRCEA_UPDATE_BITWISE_BRANCHLESS CRCEA_TOKEN(_update_bitwise_branchless)
#define CRCEA_UPDATE_BITCOMBINE2        CRCEA_TOKEN(_update_bitcombine2)
#define CRCEA_UPDATE_BITCOMBINE4        CRCEA_TOKEN(_update_bitcombine4)
#define CRCEA_UPDATE_BITCOMBINE8        CRCEA_TOKEN(_update_bitcombine8)
#define CRCEA_UPDATE_BITCOMBINE16       CRCEA_TOKEN(_update_bitcombine16)
#define CRCEA_UPDATE_BITCOMBINE32       CRCEA_TOKEN(_update_bitcombine32)
#define CRCEA_UPDATE_BY_SOLO            CRCEA_TOKEN(_update_by_solo)
#define CRCEA_UPDATE_BY1_SOLO           CRCEA_TOKEN(_update_by1_solo)
#define CRCEA_UPDATE_BY2_SOLO           CRCEA_TOKEN(_update_by2_solo)
#define CRCEA_UPDATE_BY4_SOLO           CRCEA_TOKEN(_update_by4_solo)
#define CRCEA_UPDATE_BY8_SOLO           CRCEA_TOKEN(_update_by8_solo)
#define CRCEA_UPDATE_BY16_SOLO          CRCEA_TOKEN(_update_by16_solo)
#define CRCEA_UPDATE_BY32_SOLO          CRCEA_TOKEN(_update_by32_solo)
#define CRCEA_UPDATE_BY_DUO             CRCEA_TOKEN(_update_by_duo)
#define CRCEA_UPDATE_BY1_DUO            CRCEA_TOKEN(_update_by1_duo)
#define CRCEA_UPDATE_BY2_DUO            CRCEA_TOKEN(_update_by2_duo)
#define CRCEA_UPDATE_BY4_DUO            CRCEA_TOKEN(_update_by4_duo)
#define CRCEA_UPDATE_BY8_DUO            CRCEA_TOKEN(_update_by8_duo)
#define CRCEA_UPDATE_BY16_DUO           CRCEA_TOKEN(_update_by16_duo)
#define CRCEA_UPDATE_BY32_DUO           CRCEA_TOKEN(_update_by32_duo)
#define CRCEA_UPDATE_BY_QUARTET         CRCEA_TOKEN(_update_by_quartet)
#define CRCEA_UPDATE_BY1_QUARTET        CRCEA_TOKEN(_update_by1_quartet)
#define CRCEA_UPDATE_BY2_QUARTET        CRCEA_TOKEN(_update_by2_quartet)
#define CRCEA_UPDATE_BY4_QUARTET        CRCEA_TOKEN(_update_by4_quartet)
#define CRCEA_UPDATE_BY8_QUARTET        CRCEA_TOKEN(_update_by8_quartet)
#define CRCEA_UPDATE_BY16_QUARTET       CRCEA_TOKEN(_update_by16_quartet)
#define CRCEA_UPDATE_BY32_QUARTET       CRCEA_TOKEN(_update_by32_quartet)
#define CRCEA_UPDATE_BY1_OCTET          CRCEA_TOKEN(_update_by1_octet)
#define CRCEA_UPDATE_BY2_OCTET          CRCEA_TOKEN(_update_by2_octet)
#define CRCEA_UPDATE_BY4_OCTET          CRCEA_TOKEN(_update_by4_octet)
#define CRCEA_UPDATE_BY8_OCTET          CRCEA_TOKEN(_update_by8_octet)
#define CRCEA_UPDATE_BY16_OCTET         CRCEA_TOKEN(_update_by16_octet)
#define CRCEA_UPDATE_BY32_OCTET         CRCEA_TOKEN(_update_by32_octet)
#define CRCEA_UPDATE_BY2_SEXDECTET      CRCEA_TOKEN(_update_by2_sexdectet)
#define CRCEA_UPDATE_BY4_SEXDECTET      CRCEA_TOKEN(_update_by4_sexdectet)
#define CRCEA_UPDATE_BY8_SEXDECTET      CRCEA_TOKEN(_update_by8_sexdectet)
#define CRCEA_UPDATE_BY16_SEXDECTET     CRCEA_TOKEN(_update_by16_sexdectet)
#define CRCEA_UPDATE_BY32_SEXDECTET     CRCEA_TOKEN(_update_by32_sexdectet)

#define CRCEA_BITSIZE                   (sizeof(CRCEA_TYPE) * CHAR_BIT)
#define CRCEA_LSH(N, OFF)               ((OFF) < CRCEA_BITSIZE ? (N) << (OFF) : 0)
#define CRCEA_RSH(N, OFF)               ((OFF) < CRCEA_BITSIZE ? (N) >> (OFF) : 0)
#define CRCEA_BITMASK(WID)              (~CRCEA_LSH(~(CRCEA_TYPE)0, WID))
#define CRCEA_LSH16(N, OFF)             ((OFF) < 16 ? (N) << (OFF) : 0)
#define CRCEA_RSH16(N, OFF)             ((OFF) < 16 ? (N) >> (OFF) : 0)
#define CRCEA_BITMASK16(WID)            (~CRCEA_LSH16(~(uint16_t)0, WID))


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
CRCEA_SETUP(const crcea_design *design, CRCEA_TYPE crc)
{
    CRCEA_TYPE state = (crc ^ design->xoroutput) & CRCEA_BITMASK(design->bitsize);
    if (design->reflectin ^ design->reflectout) {
        state = CRCEA_BITREFLECT(state << (CRCEA_BITSIZE - design->bitsize));
    }

    if (!design->reflectin) {
        state <<= (CRCEA_BITSIZE - design->bitsize);
    }

    return state;
}

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_FINISH(const crcea_design *design, CRCEA_TYPE state)
{
    if (!design->reflectin) {
        state >>= (CRCEA_BITSIZE - design->bitsize);
    }

    if (design->reflectin ^ design->reflectout) {
        state = CRCEA_BITREFLECT(state << (CRCEA_BITSIZE - design->bitsize));
    }

    state ^= design->xoroutput;

    return state & CRCEA_BITMASK(design->bitsize);
}

#define CRCEA_ADAPT_POLYNOMIAL(POLY, BS)    ((POLY) << (CRCEA_BITSIZE - (BS)))
#define CRCEA_ADAPT_POLYNOMIALW(POLY, BS)   ((uint16_t)(POLY) << (CRCEA_BITSIZE - (BS) + 8))
#define CRCEA_INPUT(B)                      ((CRCEA_TYPE)(uint8_t)(B) << (CRCEA_BITSIZE - 8))
#define CRCEA_INPUT16(B, N)                 ((uint16_t)(B) << (16 - (N)))
#define CRCEA_INPUTW(B, N)                  ((B) << (CRCEA_BITSIZE - (N)))
#define CRCEA_STORE(X)                      (X)
#define CRCEA_STORE16(X)                    ((X) >> 8)
#define CRCEA_SLICE(ST, N, L)               ((CRCEA_BITSIZE < 16 && (L) > 8) ? \
                                                (CRCEA_RSH((ST) << 8, (CRCEA_BITSIZE + 8) - ((N) + (L))) & CRCEA_BITMASK(L)) : \
                                                (CRCEA_RSH(ST, CRCEA_BITSIZE - ((N) + (L))) & CRCEA_BITMASK(L)))
#define CRCEA_SLICE8(X, N, L)               (CRCEA_RSH(X, 8 - ((N) + (L))) & CRCEA_BITMASK(L))
#define CRCEA_SLICE16(X, N, L)              (CRCEA_RSH16(X, 16 - ((N) + (L))) & CRCEA_BITMASK16(L))
#define CRCEA_LOAD16(P)                     (((uint16_t)*((uint8_t *)P + 0) << 8) | ((uint16_t)*((uint8_t *)P + 1) << 0))
#define CRCEA_INDEX16(A)                    ((uint16_t)(uint16_t)(A))
#define CRCEA_ADAPT_POLYNOMIAL_R(POLY, BS)  (CRCEA_BITREFLECT(POLY) >> (CRCEA_BITSIZE - (BS)))
#define CRCEA_INPUT_R(B)                    ((CRCEA_TYPE)(uint8_t)(B))
#define CRCEA_INPUTW_R(B, N)                (B)
#define CRCEA_STORE_R(X)                    (X)
#define CRCEA_SLICE_R(ST, N, L)             (CRCEA_RSH(ST, N) & CRCEA_BITMASK(L))
#define CRCEA_SLICE8_R(X, N, L)             (CRCEA_RSH(X, N) & CRCEA_BITMASK(L))
#define CRCEA_SLICE16_R(X, N, L)            (CRCEA_RSH16(X, N) & CRCEA_BITMASK16(L))
#define CRCEA_LOAD16_R(P)                   (((uint16_t)*((uint8_t *)P + 0) << 0) | ((uint16_t)*((uint8_t *)P + 1) << 8))
#define CRCEA_INDEX16_R(A)                  ((uint16_t)(uint16_t)(A) << 8)


#define CRCEA_UPDATE_STRIPE(P, PP, SLICESIZE)                               \
    do {                                                                    \
        if (SLICESIZE > 1 || CRCEA_STRIPE_SIZE > 1) {                       \
            const size_t stripesize__ = CRCEA_STRIPE_SIZE * (SLICESIZE);    \
            const char *pps__ = P + ((PP - P) & ~(stripesize__ - 1));       \
            while (P < pps__) {                                             \
                int i__;                                                    \
                for (i__ = (CRCEA_STRIPE_SIZE); i__ > 0; i__ --, P += (SLICESIZE)) { \

#define CRCEA_UPDATE_BYTE(P, PP)                                            \
                }                                                           \
            }                                                               \
        }                                                                   \
                                                                            \
        for (; P < PP; P ++) {                                              \

#define CRCEA_UPDATE_END()                                                  \
        }                                                                   \
    } while (0)                                                             \

#define CRCEA_UPDATE_SIMPLE_DECL(DESIGN, IN, END, STATE, F)                 \
    do {                                                                    \
        if ((DESIGN)->reflectin) {                                          \
            F(IN, END, CRCEA_ADAPT_POLYNOMIAL_R, CRCEA_INPUT_R, CRCEA_RSH,  \
                    CRCEA_SLICE_R, CRCEA_SLICE8_R,                          \
                    CRCEA_LOAD16_R, CRCEA_INDEX16_R);                       \
        } else {                                                            \
            F(IN, END, CRCEA_ADAPT_POLYNOMIAL, CRCEA_INPUT, CRCEA_LSH,      \
                    CRCEA_SLICE, CRCEA_SLICE8,                              \
                    CRCEA_LOAD16, CRCEA_INDEX16);                           \
        }                                                                   \
    } while (0)                                                             \

#define CRCEA_UPDATE_DECL(DESIGN, IN, END, STATE, F)                        \
            CRCEA_UPDATE_SIMPLE_DECL((DESIGN), IN, END, STATE, F);          \

#include "_reference.h"
#include "_fallback.h"
#include "_bitwise.h"
#include "_bitcombine.h"
#include "_by_solo.h"
#include "_by_duo.h"
#include "_by_quartet.h"
#include "_by_octet.h"
#include "_by_sexdectet.h"
#include "_table.h"

/*
 * 入力を伴わないガロア体の除算
 */
static inline CRCEA_TYPE
CRCEA_UPDATE_SHIFT(const crcea_design *design, size_t bits, CRCEA_TYPE state)
{
#define CRCEA_UPDATE_SHIFT_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_TYPE poly = ADAPT(design->polynomial, design->bitsize);           \
    for (; bits > 0; bits --) {                                             \
        int head = SLICE(state, 0, 1);                                      \
        state = SHIFT(state, 1);                                            \
        if (head) { state ^= poly; }                                        \
    }                                                                       \

    CRCEA_UPDATE_SIMPLE_DECL(design, p, pp, state, CRCEA_UPDATE_SHIFT_DECL);

    return state;
}

/*
 * ガロア体の除算をせずに入力値を充填する
 */
static inline CRCEA_TYPE
CRCEA_INPUT_TO_STATE(const crcea_design *design, int off, const char *p, const char *const pp, CRCEA_TYPE state)
{
    if (design->reflectin) {
        for (; p < pp; p ++, off += 8) {
            state ^= (CRCEA_TYPE)*(const uint8_t *)p << off;
        }
    } else {
        for (off = CRCEA_BITSIZE - off - 8; p < pp; p ++, off -= 8) {
            state ^= (CRCEA_TYPE)*(const uint8_t *)p << off;
        }
    }

    return state;
}

static CRCEA_TYPE
CRCEA_UPDATE_UNIFIED(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, int algo, const void *table)
{
    if (table == NULL && algo >= CRCEA_TABLE_ALGORITHM) {
        algo = CRCEA_FALLBACK;
    }

    switch (algo) {
#ifdef CRCEA_ENABLE_REFERENCE
    case CRCEA_REFERENCE:
        return CRCEA_UPDATE_REFERENCE(design, p, pp, state);
#endif

#ifdef CRCEA_ENABLE_BITWISE_CONDXOR
    case CRCEA_BITWISE_CONDXOR:
        return CRCEA_UPDATE_BITWISE_CONDXOR(design, p, pp, state);
#endif

#ifdef CRCEA_ENABLE_BITWISE_BRANCHASSIGN
    case CRCEA_BITWISE_BRANCHASSIGN:
        return CRCEA_UPDATE_BITWISE_BRANCHASSIGN(design, p, pp, state);
#endif

#ifdef CRCEA_ENABLE_BITWISE_BRANCHMIX
    case CRCEA_BITWISE_BRANCHMIX:
        return CRCEA_UPDATE_BITWISE_BRANCHMIX(design, p, pp, state);
#endif

#ifdef CRCEA_ENABLE_BITWISE_BRANCHLESS
    case CRCEA_BITWISE_BRANCHLESS:
        return CRCEA_UPDATE_BITWISE_BRANCHLESS(design, p, pp, state);
#endif

#ifdef CRCEA_ENABLE_BITCOMBINE2
    case CRCEA_BITCOMBINE2:
        return CRCEA_UPDATE_BITCOMBINE2(design, p, pp, state);
#endif

#ifdef CRCEA_ENABLE_BITCOMBINE4
    case CRCEA_BITCOMBINE4:
        return CRCEA_UPDATE_BITCOMBINE4(design, p, pp, state);
#endif

#ifdef CRCEA_ENABLE_BITCOMBINE8
    case CRCEA_BITCOMBINE8:
        return CRCEA_UPDATE_BITCOMBINE8(design, p, pp, state);
#endif

#ifdef CRCEA_ENABLE_BITCOMBINE16
    case CRCEA_BITCOMBINE16:
        return CRCEA_UPDATE_BITCOMBINE16(design, p, pp, state);
#endif

#ifdef CRCEA_ENABLE_BITCOMBINE32
    case CRCEA_BITCOMBINE32:
        return CRCEA_UPDATE_BITCOMBINE32(design, p, pp, state);
#endif

#ifdef CRCEA_ENABLE_BY_SOLO
    case CRCEA_BY_SOLO:
        return CRCEA_UPDATE_BY_SOLO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY1_SOLO
    case CRCEA_BY1_SOLO:
        return CRCEA_UPDATE_BY1_SOLO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY2_SOLO
    case CRCEA_BY2_SOLO:
        return CRCEA_UPDATE_BY2_SOLO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY4_SOLO
    case CRCEA_BY4_SOLO:
        return CRCEA_UPDATE_BY4_SOLO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY8_SOLO
    case CRCEA_BY8_SOLO:
        return CRCEA_UPDATE_BY8_SOLO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY16_SOLO
    case CRCEA_BY16_SOLO:
        return CRCEA_UPDATE_BY16_SOLO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY32_SOLO
    case CRCEA_BY32_SOLO:
        return CRCEA_UPDATE_BY32_SOLO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY_DUO
    case CRCEA_BY_DUO:
        return CRCEA_UPDATE_BY_DUO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY1_DUO
    case CRCEA_BY1_DUO:
        return CRCEA_UPDATE_BY1_DUO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY2_DUO
    case CRCEA_BY2_DUO:
        return CRCEA_UPDATE_BY2_DUO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY4_DUO
    case CRCEA_BY4_DUO:
        return CRCEA_UPDATE_BY4_DUO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY8_DUO
    case CRCEA_BY8_DUO:
        return CRCEA_UPDATE_BY8_DUO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY16_DUO
    case CRCEA_BY16_DUO:
        return CRCEA_UPDATE_BY16_DUO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY32_DUO
    case CRCEA_BY32_DUO:
        return CRCEA_UPDATE_BY32_DUO(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY_QUARTET
    case CRCEA_BY_QUARTET:
        return CRCEA_UPDATE_BY_QUARTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY1_QUARTET
    case CRCEA_BY1_QUARTET:
        return CRCEA_UPDATE_BY1_QUARTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY2_QUARTET
    case CRCEA_BY2_QUARTET:
        return CRCEA_UPDATE_BY2_QUARTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY4_QUARTET
    case CRCEA_BY4_QUARTET:
        return CRCEA_UPDATE_BY4_QUARTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY8_QUARTET
    case CRCEA_BY8_QUARTET:
        return CRCEA_UPDATE_BY8_QUARTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY16_QUARTET
    case CRCEA_BY16_QUARTET:
        return CRCEA_UPDATE_BY16_QUARTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY32_QUARTET
    case CRCEA_BY32_QUARTET:
        return CRCEA_UPDATE_BY32_QUARTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY1_OCTET
    case CRCEA_BY1_OCTET:
        return CRCEA_UPDATE_BY1_OCTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY2_OCTET
    case CRCEA_BY2_OCTET:
        return CRCEA_UPDATE_BY2_OCTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY4_OCTET
    case CRCEA_BY4_OCTET:
        return CRCEA_UPDATE_BY4_OCTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY8_OCTET
    case CRCEA_BY8_OCTET:
        return CRCEA_UPDATE_BY8_OCTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY16_OCTET
    case CRCEA_BY16_OCTET:
        return CRCEA_UPDATE_BY16_OCTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY32_OCTET
    case CRCEA_BY32_OCTET:
        return CRCEA_UPDATE_BY32_OCTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY2_SEXDECTET
    case CRCEA_BY2_SEXDECTET:
        return CRCEA_UPDATE_BY2_SEXDECTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY4_SEXDECTET
    case CRCEA_BY4_SEXDECTET:
        return CRCEA_UPDATE_BY4_SEXDECTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY8_SEXDECTET
    case CRCEA_BY8_SEXDECTET:
        return CRCEA_UPDATE_BY8_SEXDECTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY16_SEXDECTET
    case CRCEA_BY16_SEXDECTET:
        return CRCEA_UPDATE_BY16_SEXDECTET(design, p, pp, state, table);
#endif

#ifdef CRCEA_ENABLE_BY32_SEXDECTET
    case CRCEA_BY32_SEXDECTET:
        return CRCEA_UPDATE_BY32_SEXDECTET(design, p, pp, state, table);
#endif

    case CRCEA_FALLBACK:
    default:
        return CRCEA_UPDATE_FALLBACK(design, p, pp, state);
    }
}

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, int algo, const void *table)
{
    if (p >= pp) { return state; }

#ifdef CRCEA_ENABLE_REFERENCE
    if (algo == CRCEA_REFERENCE) {
        return CRCEA_UPDATE_REFERENCE(design, p, pp, state);
    }
#endif

    if (design->appendzero) {
        return CRCEA_UPDATE_UNIFIED(design, p, pp, state, algo, table);
    } else {
        if ((design->bitsize + 7) / 8 > (pp - p)) {
            if (design->bitsize < 8) {
                state = CRCEA_UPDATE_SHIFT(design, design->bitsize, state);
                state = CRCEA_INPUT_TO_STATE(design, 0, p, pp, state);
                state = CRCEA_UPDATE_SHIFT(design, 8 - design->bitsize, state);
            } else {
                state = CRCEA_UPDATE_SHIFT(design, (pp - p) * 8, state);
                state = CRCEA_INPUT_TO_STATE(design, design->bitsize - (pp - p) * 8, p, pp, state);
            }
        } else {
            const char *stop__ = (const char *)pp - (design->bitsize + 7) / 8;

            state = CRCEA_UPDATE_SHIFT(design, design->bitsize, state);
            state = CRCEA_UPDATE_UNIFIED(design, p, stop__, state, algo, table);
            state = CRCEA_INPUT_TO_STATE(design, 0, stop__, pp, state);
            if (design->bitsize % 8 > 0) {
                state = CRCEA_UPDATE_SHIFT(design, 8 - design->bitsize % 8, state);
            }
        }

        return state;
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
#undef CRCEA_UPDATE_FALLBACK
#undef CRCEA_UPDATE_BITWISE_CONDXOR
#undef CRCEA_UPDATE_BITWISE_BRANCHASSIGN
#undef CRCEA_UPDATE_BITWISE_BRANCHMIX
#undef CRCEA_UPDATE_BITWISE_BRANCHLESS
#undef CRCEA_UPDATE_BITCOMBINE2
#undef CRCEA_UPDATE_BITCOMBINE4
#undef CRCEA_UPDATE_BITCOMBINE8
#undef CRCEA_UPDATE_BITCOMBINE16
#undef CRCEA_UPDATE_BITCOMBINE32
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
#undef CRCEA_LSH16
#undef CRCEA_RSH16
#undef CRCEA_BITMASK16

#undef CRCEA_ADAPT_POLYNOMIAL
#undef CRCEA_ADAPT_POLYNOMIALW
#undef CRCEA_INPUT
#undef CRCEA_INPUT16
#undef CRCEA_INPUTW
#undef CRCEA_STORE
#undef CRCEA_STORE16
#undef CRCEA_SLICE
#undef CRCEA_SLICE8
#undef CRCEA_SLICE16
#undef CRCEA_LOAD16
#undef CRCEA_INDEX16
#undef CRCEA_ADAPT_POLYNOMIAL_R
#undef CRCEA_INPUT_R
#undef CRCEA_INPUTW_R
#undef CRCEA_STORE_R
#undef CRCEA_SLICE_R
#undef CRCEA_SLICE8_R
#undef CRCEA_SLICE16_R
#undef CRCEA_LOAD16_R
#undef CRCEA_INDEX16_R
#undef CRCEA_UPDATE_SIMPLE_DECL
#undef CRCEA_UPDATE_STRIPE
#undef CRCEA_UPDATE_BYTE
#undef CRCEA_UPDATE_END
#undef CRCEA_UPDATE_DECL
#undef CRCEA_INPUT_TO_STATE
#undef CRCEA_UPDATE_SHIFT_DECL
#undef CRCEA_UPDATE_SHIFT
#undef CRCEA_UPDATE_UNIFIED
#undef CRCEA_UPDATE_REFERENCE
#undef CRCEA_FALLBACK_DECL
#undef CRCEA_GALOIS_DIVISION2_FOWARDED
#undef CRCEA_GALOIS_DIVISION2_REFLECTED
#undef CRCEA_BITWISE_CONDXOR_DECL
#undef CRCEA_BITWISE_BRANCHASSIGN_DECL
#undef CRCEA_BITWISE_BRANCHMIX_DECL
#undef CRCEA_BITWISE_BRANCHLESS_DECL
#undef CRCEA_BITCOMBINE2_DECL
#undef CRCEA_BITCOMBINE4_DECL
#undef CRCEA_BITCOMBINE8_DECL
#undef CRCEA_BITCOMBINE16_DECL
#undef CRCEA_BITCOMBINE32_DECL
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
#undef CRCEA_BUILD_TABLE_DEFINE
#undef CRCEA_BUILD_TABLE_DECL
