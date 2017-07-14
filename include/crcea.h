/**
 * @file crcea.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons License Zero (CC0 / Public Domain)
 */

#ifndef CRCEA_H__
#define CRCEA_H__ 1

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#   define CRCEA_BEGIN_C_DECL extern "C" {
#   define CRCEA_END_C_DECL   }
#else
#   define CRCEA_BEGIN_C_DECL
#   define CRCEA_END_C_DECL
#endif

enum crcea_algorithms
{
    CRCEA_ALGORITHM_BITBYBIT          = -2,
    CRCEA_ALGORITHM_BITBYBIT_FAST     = -1,
    CRCEA_ALGORITHM_HALFBYTE_TABLE    =  0,
    CRCEA_ALGORITHM_STANDARD_TABLE    =  1,
    CRCEA_ALGORITHM_SLICING_BY_4      =  4,
    CRCEA_ALGORITHM_SLICING_BY_8      =  8,
    CRCEA_ALGORITHM_SLICING_BY_16     = 16,
};

enum crcea_int_types
{
    CRCEA_TYPE_INT8 = 1,
    CRCEA_TYPE_INT16 = 2,
    CRCEA_TYPE_INT32 = 4,
    CRCEA_TYPE_INT64 = 8,
#ifdef HAVE_TYPE_UINT128_T
    CRCEA_TYPE_INT128 = 16,
#endif
};

typedef struct crcea_t crcea_t;
typedef void *(crcea_alloc_f)(crcea_t *cc, size_t size);

#if defined(CRCEA_ONLY_INT32)
typedef uint32_t crcea_int;
#elif defined(CRCEA_ONLY_INT16)
typedef uint16_t crcea_int;
#elif defined(CRCEA_ONLY_INT8)
typedef uint8_t crcea_int;
#else /* defined(CRCEA_ONLY_INT64) */
typedef uint64_t crcea_int;
#endif

struct crcea_t
{
    int8_t inttype;    /*< enum crcea_int_types */
    int8_t algorithm;  /*< enum crcea_algorithms */
    int8_t bitsize;
    uint8_t reflect_input:1;
    uint8_t reflect_output:1;
    crcea_int polynomial;
    crcea_int xor_output;
    const void *table;
    crcea_alloc_f *alloc;

    /** ... user data field ... */
};

crcea_int crcea_setup(crcea_t *cc, crcea_int crc);
crcea_int crcea_update(crcea_t *cc, const void *src, const void *srcend, crcea_int state);
crcea_int crcea_finish(crcea_t *cc, crcea_int state);

#endif /* CRCEA_H__ */
