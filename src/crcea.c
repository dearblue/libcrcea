/**
 * @file crcea.c
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons License Zero (CC0 / Public Domain)
 *
 * Preprocessor definisions before included this file:
 *
 * [CRCEA_NO_MALLOC]
 *      Optional, not defined by default.
 *
 * [CRCEA_DEFAULT_MALLOC]
 *      Optional.
 */

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
       case CRCEA_TYPE_INT128:               \
           F(uint128_t, crcea128);           \
           break;                          \

#   else
#      define CRCEA_HAVE_TYPE_UINT128_T_CASE
#   endif

#   define CRCEA_SWITCH_BY_TYPE(C, F)         \
        switch (C->inttype) {               \
        case CRCEA_TYPE_INT8:                 \
            F(uint8_t, crcea8);               \
            break;                          \
        case CRCEA_TYPE_INT16:                \
            F(uint16_t, crcea16);             \
            break;                          \
        case CRCEA_TYPE_INT32:                \
            F(uint32_t, crcea32);             \
            break;                          \
        case CRCEA_TYPE_INT64:                \
            F(uint64_t, crcea64);             \
            break;                          \
        CRCEA_HAVE_TYPE_UINT128_T_CASE        \
        default:                            \
            break;                          \
        }                                   \

#endif /* CRCEA_ONLY_UINT*** */

crcea_int
crcea_setup(crcea_context *cc, crcea_int crc)
{
#define CRCEA_SETUP(T, P) return P ## _setup(cc, crc)

    CRCEA_SWITCH_BY_TYPE(cc, CRCEA_SETUP);

    return ~(crcea_int)0;
}

crcea_int
crcea_update(crcea_context *cc, const void *p, const void *pp, crcea_int state)
{
#define CRCEA_UPDATE(T, P) return P ## _update(cc, p, pp, state)

    CRCEA_SWITCH_BY_TYPE(cc, CRCEA_UPDATE);

    return state;
}

crcea_int
crcea_finish(crcea_context *cc, crcea_int state)
{
#define CRCEA_FINISH(T, P) return P ## _finish(cc, state)

    CRCEA_SWITCH_BY_TYPE(cc, CRCEA_FINISH);

    return ~(crcea_int)0;
}

#ifndef CRCEA_NO_MALLOC
static void *
CRCEA_DEFAULT_MALLOC(void *opaque, size_t size)
{
    return CRCEA_MALLOC(size);
}
#endif
