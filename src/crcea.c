/**
 * @file crcea.c
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.osdn.me>
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

#if defined(CRCEA_ONLY_INT64)
#   define CRCEA_PREFIX      crcea64
#   define CRCEA_TYPE        uint64_t
#   include "../include/crcea/core.h"

#   define CRCEA_SWITCH_BY_TYPE(D, F)                                       \
        F(uint64_t, crcea64);                                               \

#elif defined(CRCEA_ONLY_INT32)
#   define CRCEA_PREFIX      crcea32
#   define CRCEA_TYPE        uint32_t
#   include "../include/crcea/core.h"

#   define CRCEA_SWITCH_BY_TYPE(D, F)                                       \
        F(uint32_t, crcea32);                                               \

#elif defined(CRCEA_ONLY_INT16)
#   define CRCEA_PREFIX      crcea16
#   define CRCEA_TYPE        uint16_t
#   include "../include/crcea/core.h"

#   define CRCEA_SWITCH_BY_TYPE(D, F)                                       \
        F(uint16_t, crcea16);                                               \

#elif defined(CRCEA_ONLY_INT8)
#   define CRCEA_PREFIX      crcea8
#   define CRCEA_TYPE        uint8_t
#   include "../include/crcea/core.h"

#   define CRCEA_SWITCH_BY_TYPE(D, F)                                       \
        F(uint8_t, crcea8);                                                 \

#elif defined(CRCEA_ONLY_INT32_INT64)
#   define CRCEA_PREFIX      crcea32
#   define CRCEA_TYPE        uint32_t
#   include "../include/crcea/core.h"

#   define CRCEA_PREFIX      crcea64
#   define CRCEA_TYPE        uint64_t
#   include "../include/crcea/core.h"

#   define CRCEA_SWITCH_BY_TYPE(D, F)                                       \
        do {                                                                \
            if ((D)->bitsize > 32) {                                        \
                F(uint64_t, crcea64);                                       \
            } else {                                                        \
                F(uint32_t, crcea32);                                       \
            }                                                               \
        } while (0)                                                         \

#else
#   define CRCEA_PREFIX      crcea16
#   define CRCEA_TYPE        uint16_t
#   include "../include/crcea/core.h"

#   define CRCEA_PREFIX      crcea32
#   define CRCEA_TYPE        uint32_t
#   include "../include/crcea/core.h"

#   define CRCEA_PREFIX      crcea64
#   define CRCEA_TYPE        uint64_t
#   include "../include/crcea/core.h"

#   define CRCEA_SWITCH_BY_TYPE(D, F)                                       \
        do {                                                                \
            if ((D)->bitsize > 32) {                                        \
                F(uint64_t, crcea64);                                       \
            } else if ((D)->bitsize > 16) {                                 \
                F(uint32_t, crcea32);                                       \
            } else {                                                        \
                F(uint16_t, crcea16);                                       \
            }                                                               \
        } while (0)                                                         \

#endif /* CRCEA_ONLY_UINT*** */

size_t
crcea_tablesize(const crcea_context *cc)
{
#define CRCEA_TABLE_SIZE_DECL(T, P) return P ## _tablesize(cc->algorithm)

    CRCEA_SWITCH_BY_TYPE(cc->design, CRCEA_TABLE_SIZE_DECL);

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
            return CRCEA_FALLBACK;
#endif
        }

        if (alloc) {
            void *table = alloc(cc->opaque, crcea_tablesize(cc));
            if (!table) {
                return CRCEA_FALLBACK;
            }

            crcea_build_table(cc->design, algo, table);
            cc->table = table;
        }
    }

    return algo;
}

void
crcea_build_table(const crcea_design *design, int algo, void *table)
{
#define CRCEA_BUILD_TABLE_DECL(T, P) P ## _build_table(design, algo, table);

    CRCEA_SWITCH_BY_TYPE(design, CRCEA_BUILD_TABLE_DECL);
}

crcea_int
crcea_setup(const crcea_context *cc, crcea_int crc)
{
#define CRCEA_SETUP(T, P) return P ## _setup(cc->design, crc)

    CRCEA_SWITCH_BY_TYPE(cc->design, CRCEA_SETUP);

    return ~(crcea_int)0;
}

crcea_int
crcea_update(const crcea_context *cc, const void *p, const void *pp, crcea_int state)
{
#define CRCEA_UPDATE(T, P)                                                  \
    do {                                                                    \
        return P ## _update(cc->design, p, pp, state, cc->algorithm, cc->table); \
    } while (0);                                                            \

    CRCEA_SWITCH_BY_TYPE(cc->design, CRCEA_UPDATE);

    return state;
}

crcea_int
crcea_finish(const crcea_context *cc, crcea_int state)
{
#define CRCEA_FINISH(T, P) return P ## _finish(cc->design, state)

    CRCEA_SWITCH_BY_TYPE(cc->design, CRCEA_FINISH);

    return ~(crcea_int)0;
}

crcea_int
crcea(const crcea_context *cc, const void *src, const void *srcend, crcea_int crc)
{
    crcea_int s = crcea_setup(cc, crc);
    s = crcea_update(cc, src, srcend, s);
    return crcea_finish(cc, s);
}

#ifndef CRCEA_NO_MALLOC
static void *
CRCEA_DEFAULT_MALLOC(void *opaque, size_t size)
{
    (void)opaque;

    return CRCEA_MALLOC(size);
}
#endif
