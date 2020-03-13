/**
 * @file _by_octet.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 */

#ifdef CRCEA_ENABLE_BY1_OCTET

/*
 * Slicing by Single Octet
 *
 * This algorithm is table algorithm by Dilip V. Sarwate in 1988.
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY1_OCTET(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE *t = (const CRCEA_TYPE *)table;

#define CRCEA_BY1_OCTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                        \
        state = SHIFT(state, 8) ^ t[(uint8_t)*IN ^ SLICE(state, 0, 8)];     \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state = SHIFT(state, 8) ^ t[(uint8_t)*IN ^ SLICE(state, 0, 8)];     \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BY1_OCTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY1_OCTET */

#ifdef CRCEA_ENABLE_BY2_OCTET

/*
 * Slicing by Double Octet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY2_OCTET(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[256] = (const CRCEA_TYPE (*)[256])table;

#define CRCEA_BY2_OCTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 2);                                        \
        state = SHIFT(state, 16) ^                                          \
                t[1][(uint8_t)IN[0] ^ SLICE(state,  0, 8)] ^                \
                t[0][(uint8_t)IN[1] ^ SLICE(state,  8, 8)];                 \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state = SHIFT(state, 8) ^ t[0][(uint8_t)*IN ^ SLICE(state, 0, 8)];  \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BY2_OCTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY2_OCTET */

#ifdef CRCEA_ENABLE_BY4_OCTET

/*
 * Slicing by Quadruple Octet
 *
 * This algorithm is based on Slicing-by-4 by Intel corp. in 2006.
 *
 * Modified points are byte order free and byte alignment free.
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY4_OCTET(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[256] = (const CRCEA_TYPE (*)[256])table;

#define CRCEA_BY4_OCTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 4);                                        \
        state = SHIFT(state, 32) ^                                          \
                t[3][(uint8_t)IN[0] ^ SLICE(state,  0, 8)] ^                \
                t[2][(uint8_t)IN[1] ^ SLICE(state,  8, 8)] ^                \
                t[1][(uint8_t)IN[2] ^ SLICE(state, 16, 8)] ^                \
                t[0][(uint8_t)IN[3] ^ SLICE(state, 24, 8)];                 \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state = SHIFT(state, 8) ^ t[0][(uint8_t)*IN ^ SLICE(state, 0, 8)];  \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BY4_OCTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY4_OCTET */

#ifdef CRCEA_ENABLE_BY8_OCTET

/*
 * Slicing by Quadruple Octet
 *
 * This algorithm is based on Slicing-by-8 by Intel corp. in 2006.
 *
 * Modified points are byte order free and byte alignment free.
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY8_OCTET(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[256] = (const CRCEA_TYPE (*)[256])table;

#define CRCEA_BY8_OCTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 8);                                        \
        state = SHIFT(state, 64) ^                                          \
                t[7][(uint8_t)IN[0] ^ SLICE(state,  0, 8)] ^                \
                t[6][(uint8_t)IN[1] ^ SLICE(state,  8, 8)] ^                \
                t[5][(uint8_t)IN[2] ^ SLICE(state, 16, 8)] ^                \
                t[4][(uint8_t)IN[3] ^ SLICE(state, 24, 8)] ^                \
                t[3][(uint8_t)IN[4] ^ SLICE(state, 32, 8)] ^                \
                t[2][(uint8_t)IN[5] ^ SLICE(state, 40, 8)] ^                \
                t[1][(uint8_t)IN[6] ^ SLICE(state, 48, 8)] ^                \
                t[0][(uint8_t)IN[7] ^ SLICE(state, 56, 8)];                 \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state = SHIFT(state, 8) ^ t[0][(uint8_t)*IN ^ SLICE(state, 0, 8)];  \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BY8_OCTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY8_OCTET */

#ifdef CRCEA_ENABLE_BY16_OCTET

/*
 * Slicing by Sexdecuple Octet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY16_OCTET(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[256] = (const CRCEA_TYPE (*)[256])table;

#define CRCEA_BY16_OCTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 16);                                       \
        state = SHIFT(state, 128) ^                                         \
                t[15][(uint8_t)IN[ 0] ^ SLICE(state,   0, 8)] ^             \
                t[14][(uint8_t)IN[ 1] ^ SLICE(state,   8, 8)] ^             \
                t[13][(uint8_t)IN[ 2] ^ SLICE(state,  16, 8)] ^             \
                t[12][(uint8_t)IN[ 3] ^ SLICE(state,  24, 8)] ^             \
                t[11][(uint8_t)IN[ 4] ^ SLICE(state,  32, 8)] ^             \
                t[10][(uint8_t)IN[ 5] ^ SLICE(state,  40, 8)] ^             \
                t[ 9][(uint8_t)IN[ 6] ^ SLICE(state,  48, 8)] ^             \
                t[ 8][(uint8_t)IN[ 7] ^ SLICE(state,  56, 8)] ^             \
                t[ 7][(uint8_t)IN[ 8] ^ SLICE(state,  64, 8)] ^             \
                t[ 6][(uint8_t)IN[ 9] ^ SLICE(state,  72, 8)] ^             \
                t[ 5][(uint8_t)IN[10] ^ SLICE(state,  80, 8)] ^             \
                t[ 4][(uint8_t)IN[11] ^ SLICE(state,  88, 8)] ^             \
                t[ 3][(uint8_t)IN[12] ^ SLICE(state,  96, 8)] ^             \
                t[ 2][(uint8_t)IN[13] ^ SLICE(state, 104, 8)] ^             \
                t[ 1][(uint8_t)IN[14] ^ SLICE(state, 112, 8)] ^             \
                t[ 0][(uint8_t)IN[15] ^ SLICE(state, 120, 8)];              \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state = SHIFT(state, 8) ^ t[0][(uint8_t)*IN ^ SLICE(state, 0, 8)];  \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BY16_OCTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY16_OCTET */

#ifdef CRCEA_ENABLE_BY32_OCTET

/*
 * Slicing by Slicing by Duotriguple Octet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY32_OCTET(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[256] = (const CRCEA_TYPE (*)[256])table;

#define CRCEA_BY32_OCTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 32);                                       \
        state = SHIFT(state, 256) ^                                         \
                t[31][(uint8_t)IN[ 0] ^ SLICE(state,   0, 8)] ^             \
                t[30][(uint8_t)IN[ 1] ^ SLICE(state,   8, 8)] ^             \
                t[29][(uint8_t)IN[ 2] ^ SLICE(state,  16, 8)] ^             \
                t[28][(uint8_t)IN[ 3] ^ SLICE(state,  24, 8)] ^             \
                t[27][(uint8_t)IN[ 4] ^ SLICE(state,  32, 8)] ^             \
                t[26][(uint8_t)IN[ 5] ^ SLICE(state,  40, 8)] ^             \
                t[25][(uint8_t)IN[ 6] ^ SLICE(state,  48, 8)] ^             \
                t[24][(uint8_t)IN[ 7] ^ SLICE(state,  56, 8)] ^             \
                t[23][(uint8_t)IN[ 8] ^ SLICE(state,  64, 8)] ^             \
                t[22][(uint8_t)IN[ 9] ^ SLICE(state,  72, 8)] ^             \
                t[21][(uint8_t)IN[10] ^ SLICE(state,  80, 8)] ^             \
                t[20][(uint8_t)IN[11] ^ SLICE(state,  88, 8)] ^             \
                t[19][(uint8_t)IN[12] ^ SLICE(state,  96, 8)] ^             \
                t[18][(uint8_t)IN[13] ^ SLICE(state, 104, 8)] ^             \
                t[17][(uint8_t)IN[14] ^ SLICE(state, 112, 8)] ^             \
                t[16][(uint8_t)IN[15] ^ SLICE(state, 120, 8)] ^             \
                t[15][(uint8_t)IN[16] ^ SLICE(state, 128, 8)] ^             \
                t[14][(uint8_t)IN[17] ^ SLICE(state, 136, 8)] ^             \
                t[13][(uint8_t)IN[18] ^ SLICE(state, 144, 8)] ^             \
                t[12][(uint8_t)IN[19] ^ SLICE(state, 152, 8)] ^             \
                t[11][(uint8_t)IN[20] ^ SLICE(state, 160, 8)] ^             \
                t[10][(uint8_t)IN[21] ^ SLICE(state, 168, 8)] ^             \
                t[ 9][(uint8_t)IN[22] ^ SLICE(state, 176, 8)] ^             \
                t[ 8][(uint8_t)IN[23] ^ SLICE(state, 184, 8)] ^             \
                t[ 7][(uint8_t)IN[24] ^ SLICE(state, 192, 8)] ^             \
                t[ 6][(uint8_t)IN[25] ^ SLICE(state, 200, 8)] ^             \
                t[ 5][(uint8_t)IN[26] ^ SLICE(state, 208, 8)] ^             \
                t[ 4][(uint8_t)IN[27] ^ SLICE(state, 216, 8)] ^             \
                t[ 3][(uint8_t)IN[28] ^ SLICE(state, 224, 8)] ^             \
                t[ 2][(uint8_t)IN[29] ^ SLICE(state, 232, 8)] ^             \
                t[ 1][(uint8_t)IN[30] ^ SLICE(state, 240, 8)] ^             \
                t[ 0][(uint8_t)IN[31] ^ SLICE(state, 248, 8)];              \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state = SHIFT(state, 8) ^ t[0][(uint8_t)*IN ^ SLICE(state, 0, 8)];  \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BY32_OCTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY32_OCTET */
