/**
 * @file defs.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 */

#ifndef CRCEA_DEFS_H__
#define CRCEA_DEFS_H__ 1

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
# define CRCEA_BEGIN_C_DECL extern "C" {
# define CRCEA_END_C_DECL   }
#else
# define CRCEA_BEGIN_C_DECL
# define CRCEA_END_C_DECL
#endif


#if defined(CRCEA_MINIMAL)
# ifndef CRCEA_DEFAULT_ALGORYTHM
#  define CRCEA_DEFAULT_ALGORYTHM CRCEA_BITWISE_BRANCHLESS
# endif
# define CRCEA_ENABLE_BITWISE_BRANCHLESS

#elif defined(CRCEA_TINY)
# ifndef CRCEA_DEFAULT_ALGORYTHM
#  define CRCEA_DEFAULT_ALGORYTHM CRCEA_FALLBACK
# endif
    /* 追加されるアルゴリズムはなし */

#elif defined(CRCEA_SMALL)
# ifndef CRCEA_DEFAULT_ALGORYTHM
#  define CRCEA_DEFAULT_ALGORYTHM CRCEA_BY4_QUARTET
# endif
# define CRCEA_ENABLE_BY4_QUARTET

#elif defined(CRCEA_LARGE)
# ifndef CRCEA_DEFAULT_ALGORYTHM
#  define CRCEA_DEFAULT_ALGORYTHM CRCEA_BY16_OCTET
# endif
# define CRCEA_ENABLE_BY16_OCTET

#elif defined(CRCEA_DEFAULT) || !defined(CRCEA_ACADEMIC) /* もしくはプロファイルが未定義 */
# ifndef CRCEA_DEFAULT_ALGORYTHM
#  define CRCEA_DEFAULT_ALGORYTHM CRCEA_BY4_OCTET
# endif
# define CRCEA_ENABLE_BY4_OCTET

#else /* defined CRCEA_ACADEMIC */
# ifndef CRCEA_DEFAULT_ALGORYTHM
#  define CRCEA_DEFAULT_ALGORYTHM CRCEA_BY4_OCTET
# endif
# define CRCEA_ENABLE_REFERENCE
# define CRCEA_ENABLE_FALLBACK
# define CRCEA_ENABLE_BITWISE_CONDXOR
# define CRCEA_ENABLE_BITWISE_BRANCHASSIGN
# define CRCEA_ENABLE_BITWISE_BRANCHMIX
# define CRCEA_ENABLE_BITWISE_BRANCHLESS
# define CRCEA_ENABLE_BITCOMBINE2
# define CRCEA_ENABLE_BITCOMBINE4
# define CRCEA_ENABLE_BITCOMBINE8
# define CRCEA_ENABLE_BITCOMBINE16
# define CRCEA_ENABLE_BITCOMBINE32
# define CRCEA_ENABLE_BY_SOLO
# define CRCEA_ENABLE_BY1_SOLO
# define CRCEA_ENABLE_BY2_SOLO
# define CRCEA_ENABLE_BY4_SOLO
# define CRCEA_ENABLE_BY8_SOLO
# define CRCEA_ENABLE_BY16_SOLO
# define CRCEA_ENABLE_BY32_SOLO
# define CRCEA_ENABLE_BY_DUO
# define CRCEA_ENABLE_BY1_DUO
# define CRCEA_ENABLE_BY2_DUO
# define CRCEA_ENABLE_BY4_DUO
# define CRCEA_ENABLE_BY8_DUO
# define CRCEA_ENABLE_BY16_DUO
# define CRCEA_ENABLE_BY32_DUO
# define CRCEA_ENABLE_BY_QUARTET
# define CRCEA_ENABLE_BY1_QUARTET
# define CRCEA_ENABLE_BY2_QUARTET
# define CRCEA_ENABLE_BY4_QUARTET
# define CRCEA_ENABLE_BY8_QUARTET
# define CRCEA_ENABLE_BY16_QUARTET
# define CRCEA_ENABLE_BY32_QUARTET
# define CRCEA_ENABLE_BY1_OCTET
# define CRCEA_ENABLE_BY2_OCTET
# define CRCEA_ENABLE_BY4_OCTET
# define CRCEA_ENABLE_BY8_OCTET
# define CRCEA_ENABLE_BY16_OCTET
# define CRCEA_ENABLE_BY32_OCTET
# define CRCEA_ENABLE_BY2_SEXDECTET
# define CRCEA_ENABLE_BY4_SEXDECTET
# define CRCEA_ENABLE_BY8_SEXDECTET
# define CRCEA_ENABLE_BY16_SEXDECTET
# define CRCEA_ENABLE_BY32_SEXDECTET

#endif


enum crcea_algorithms
{
    CRCEA_REFERENCE             = 0,
    CRCEA_FALLBACK              = 1,

    CRCEA_BITWISE_CONDXOR       = 0x10,
    CRCEA_BITWISE_BRANCHASSIGN  = 0x11,
    CRCEA_BITWISE_BRANCHMIX     = 0x12,
    CRCEA_BITWISE_BRANCHLESS    = 0x13,
    CRCEA_BITCOMBINE2           = 0x20,
    CRCEA_BITCOMBINE4           = 0x21,
    CRCEA_BITCOMBINE8           = 0x22,
    CRCEA_BITCOMBINE16          = 0x23,
    CRCEA_BITCOMBINE32          = 0x24,

    CRCEA_TABLE_ALGORITHM       = 0x0100,

    CRCEA_BY_SOLO_GROUP         = 0x0100,
    CRCEA_BY_SOLO               = 0 | CRCEA_BY_SOLO_GROUP,
    CRCEA_BY1_SOLO              = 1 | CRCEA_BY_SOLO_GROUP,
    CRCEA_BY2_SOLO              = 2 | CRCEA_BY_SOLO_GROUP,
    CRCEA_BY4_SOLO              = 3 | CRCEA_BY_SOLO_GROUP,
    CRCEA_BY8_SOLO              = 4 | CRCEA_BY_SOLO_GROUP,
    CRCEA_BY16_SOLO             = 5 | CRCEA_BY_SOLO_GROUP,
    CRCEA_BY32_SOLO             = 6 | CRCEA_BY_SOLO_GROUP,

    CRCEA_BY_DUO_GROUP          = 0x0200,
    CRCEA_BY_DUO                = 0 | CRCEA_BY_DUO_GROUP,
    CRCEA_BY1_DUO               = 1 | CRCEA_BY_DUO_GROUP,
    CRCEA_BY2_DUO               = 2 | CRCEA_BY_DUO_GROUP,
    CRCEA_BY4_DUO               = 3 | CRCEA_BY_DUO_GROUP,
    CRCEA_BY8_DUO               = 4 | CRCEA_BY_DUO_GROUP,
    CRCEA_BY16_DUO              = 5 | CRCEA_BY_DUO_GROUP,
    CRCEA_BY32_DUO              = 6 | CRCEA_BY_DUO_GROUP,

    CRCEA_BY_QUARTET_GROUP      = 0x0300,
    CRCEA_BY_QUARTET            = 0 | CRCEA_BY_QUARTET_GROUP,
    CRCEA_BY1_QUARTET           = 1 | CRCEA_BY_QUARTET_GROUP,
    CRCEA_BY2_QUARTET           = 2 | CRCEA_BY_QUARTET_GROUP,
    CRCEA_BY4_QUARTET           = 3 | CRCEA_BY_QUARTET_GROUP,
    CRCEA_BY8_QUARTET           = 4 | CRCEA_BY_QUARTET_GROUP,
    CRCEA_BY16_QUARTET          = 5 | CRCEA_BY_QUARTET_GROUP,
    CRCEA_BY32_QUARTET          = 6 | CRCEA_BY_QUARTET_GROUP,

    CRCEA_BY_OCTET_GROUP        = 0x0400,
    CRCEA_BY_OCTET              = 1 | CRCEA_BY_OCTET_GROUP,
    CRCEA_BY1_OCTET             = CRCEA_BY_OCTET,
    CRCEA_BY2_OCTET             = 2 | CRCEA_BY_OCTET_GROUP,
    CRCEA_BY4_OCTET             = 3 | CRCEA_BY_OCTET_GROUP,
    CRCEA_BY8_OCTET             = 4 | CRCEA_BY_OCTET_GROUP,
    CRCEA_BY16_OCTET            = 5 | CRCEA_BY_OCTET_GROUP,
    CRCEA_BY32_OCTET            = 6 | CRCEA_BY_OCTET_GROUP,

    CRCEA_BY_SEXDECTET_GROUP    = 0x0500,
    CRCEA_BY_SEXDECTET          = 2 | CRCEA_BY_SEXDECTET_GROUP,
    CRCEA_BY2_SEXDECTET         = CRCEA_BY_SEXDECTET,
    CRCEA_BY4_SEXDECTET         = 3 | CRCEA_BY_SEXDECTET_GROUP,
    CRCEA_BY8_SEXDECTET         = 4 | CRCEA_BY_SEXDECTET_GROUP,
    CRCEA_BY16_SEXDECTET        = 5 | CRCEA_BY_SEXDECTET_GROUP,
    CRCEA_BY32_SEXDECTET        = 6 | CRCEA_BY_SEXDECTET_GROUP,

    CRCEA_HALFBYTE_TABLE        = CRCEA_BY_QUARTET,
    CRCEA_STANDARD_TABLE        = CRCEA_BY_OCTET,
    CRCEA_SLICING_BY_4          = CRCEA_BY4_OCTET,
    CRCEA_SLICING_BY_8          = CRCEA_BY8_OCTET,
    CRCEA_SLICING_BY_16         = CRCEA_BY16_OCTET,
};

typedef struct crcea_design crcea_design;
typedef struct crcea_context crcea_context;
typedef void *(crcea_alloc_f)(void *opaque, size_t size);

#if defined(CRCEA_ONLY_INT32)
typedef uint32_t crcea_int;
#elif defined(CRCEA_ONLY_INT16)
typedef uint16_t crcea_int;
#elif defined(CRCEA_ONLY_INT8)
typedef uint8_t crcea_int;
#else /* defined(CRCEA_ONLY_INT64) */
typedef uint64_t crcea_int;
#endif

struct crcea_design
{
    uint32_t bitsize:8;
    uint32_t reflectin:1;
    uint32_t reflectout:1;
    uint32_t appendzero:1;
    crcea_int polynomial;
    crcea_int xoroutput;
};

struct crcea_context
{
    const crcea_design *design;
    int16_t algorithm;  /*< enum crcea_algorithms */
    const void *table;
    crcea_alloc_f *alloc;
    void *opaque;       /*< for custom memory allocator */
};

#endif /* CRCEA_DEFS_H__ */
