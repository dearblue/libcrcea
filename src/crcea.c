/**
 * @file crcea.c
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 *
 * Preprocessor definisions before included this file:
 *
 * [CRCEA_NO_MALLOC]
 *      Optional, not defined by default.
 *
 * [CRCEA_DEFAULT_MALLOC]
 *      Optional.
 */

#ifndef CRCEA_DEFAULT
#   define CRCEA_ACADEMIC
#endif

#include "../include/crcea.h"

#if !defined(CRCEA_NO_MALLOC) && !defined(CRCEA_DEFAULT_MALLOC)
#   define CRCEA_DEFAULT_MALLOC crcea_default_malloc
static void *CRCEA_DEFAULT_MALLOC(void *opaque, size_t size);
#elif defined(CRCEA_DEFAULT_MALLOC)
void *CRCEA_DEFAULT_MALLOC(void *opaque, size_t size);
#endif

#if defined(CRCEA_ONLY_INT32)
#   define CRCEA_PREFIX      crcea32
#   define CRCEA_TYPE        uint32_t
#   include "../include/crcea/core.h"

#   define CRCEA_SWITCH_BY_TYPE(C, F)     \
        F(uint32_t, crcea32);             \

#elif defined(CRCEA_ONLY_INT16)
#   define CRCEA_PREFIX      crcea16
#   define CRCEA_TYPE        uint16_t
#   include "../include/crcea/core.h"

#   define CRCEA_SWITCH_BY_TYPE(C, F)     \
        F(uint16_t, crcea16);             \

#elif defined(CRCEA_ONLY_INT8)
#   define CRCEA_PREFIX      crcea8
#   define CRCEA_TYPE        uint8_t
#   include "../include/crcea/core.h"

#   define CRCEA_SWITCH_BY_TYPE(C, F)     \
        F(uint8_t, crcea8);               \

#elif defined(CRCEA_ONLY_INT64)
#   define CRCEA_PREFIX      crcea64
#   define CRCEA_TYPE        uint64_t
#   include "../include/crcea/core.h"

#   define CRCEA_SWITCH_BY_TYPE(C, F)     \
        F(uint64_t, crcea64);             \

#else
#   define CRCEA_PREFIX      crcea8
#   define CRCEA_TYPE        uint8_t
#   include "../include/crcea/core.h"

#   define CRCEA_PREFIX      crcea16
#   define CRCEA_TYPE        uint16_t
#   include "../include/crcea/core.h"

#   define CRCEA_PREFIX      crcea32
#   define CRCEA_TYPE        uint32_t
#   include "../include/crcea/core.h"

#   define CRCEA_PREFIX      crcea64
#   define CRCEA_TYPE        uint64_t
#   include "../include/crcea/core.h"

#   if 0 && HAVE_TYPE_UINT128_T
#      define CRCEA_PREFIX  crcea128
#      define CRCEA_TYPE    uint128_t
#      include "../include/crcea/core.h"

#      define CRCEA_HAVE_TYPE_UINT128_T_CASE \
       case CRCEA_INT128:               \
           F(uint128_t, crcea128);           \
           break;                          \

#   else
#      define CRCEA_HAVE_TYPE_UINT128_T_CASE
#   endif

#   define CRCEA_SWITCH_BY_TYPE(C, F)         \
        if ((C)->model->bitsize <= 8) { \
            F(uint8_t, crcea8); \
        } else if ((C)->model->bitsize <= 16) { \
            F(uint16_t, crcea16); \
        } else if ((C)->model->bitsize <= 32) { \
            F(uint32_t, crcea32); \
        } else { \
            F(uint64_t, crcea64); \
        } \

#endif /* CRCEA_ONLY_UINT*** */

size_t
crcea_tablesize(const crcea_context *cc)
{
#define CRCEA_TABLE_SIZE_DECL(T, P) return P ## _tablesize(cc->algorithm)

    CRCEA_SWITCH_BY_TYPE(cc, CRCEA_TABLE_SIZE_DECL);

    return ~(size_t)0;
}

int
crcea_prepare_table(crcea_context *cc)
{
    int algo = cc->algorithm;

    if (!cc->table && algo >= CRCEA_TABLE_ALGORITHM) {
        crcea_alloc_f *alloc = cc->alloc;
        if (!alloc) {
#ifdef CRCEA_DEFAULT_MALLOC
            alloc = CRCEA_DEFAULT_MALLOC;
#else
            return CRCEA_BITBYBIT;
#endif
        }

        if (alloc) {
            void *table = alloc(cc->opaque, crcea_tablesize(cc));
            if (!table) {
                return CRCEA_BITBYBIT;
            }

#define CRCEA_BUILD_TABLE_DECL(T, P) P ## _build_table(cc->model, cc->algorithm, table);

            CRCEA_SWITCH_BY_TYPE(cc, CRCEA_BUILD_TABLE_DECL);

            cc->table = table;
        }
    }

    return algo;
}

crcea_int
crcea_setup(crcea_context *cc, crcea_int crc)
{
#define CRCEA_SETUP(T, P) return P ## _setup(cc->model, crc)

    CRCEA_SWITCH_BY_TYPE(cc, CRCEA_SETUP);

    return ~(crcea_int)0;
}

crcea_int
crcea_update(crcea_context *cc, const void *p, const void *pp, crcea_int state)
{
#define CRCEA_UPDATE(T, P) \
    do { \
        int algo = crcea_prepare_table(cc); \
        return P ## _update(cc->model, p, pp, state, algo, cc->table); \
    } while (0); \

    CRCEA_SWITCH_BY_TYPE(cc, CRCEA_UPDATE);

    return state;
}

crcea_int
crcea_finish(crcea_context *cc, crcea_int state)
{
#define CRCEA_FINISH(T, P) return P ## _finish(cc->model, state)

    CRCEA_SWITCH_BY_TYPE(cc, CRCEA_FINISH);

    return ~(crcea_int)0;
}

crcea_int
crcea(crcea_context *cc, const void *src, const void *srcend, crcea_int crc)
{
    crcea_int s = crcea_setup(cc, crc);
    s = crcea_update(cc, src, srcend, s);
    return crcea_finish(cc, s);
}

#ifndef CRCEA_NO_MALLOC
static void *
CRCEA_DEFAULT_MALLOC(void *opaque, size_t size)
{
    return CRCEA_MALLOC(size);
}
#endif
