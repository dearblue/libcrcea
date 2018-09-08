/**
 * @file _by_sexdectet.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 */

#ifdef CRCEA_ENABLE_BY2_SEXDECTET

/*
 * Slicing by Single Sexdectet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY2_SEXDECTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE *t = (const CRCEA_TYPE *)table;

#define CRCEA_BY2_SEXDECTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 2);                                             \
        const uint16_t n = LOAD16(IN); \
        state = SHIFT(state, 16) ^                                          \
                t[n ^ SLICE(state,  0, 16)];                 \
    CRCEA_UPDATE_BYTE(IN, END);                                                                    \
        state = SHIFT(state, 8) ^ t[INDEX16((uint8_t)*IN ^ SLICE(state, 0, 8))];  \
    CRCEA_UPDATE_END();                                                                     \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY2_SEXDECTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY2_SEXDECTET */

#ifdef CRCEA_ENABLE_BY4_SEXDECTET

/*
 * Slicing by Double Sexdectet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY4_SEXDECTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[65536] = (const CRCEA_TYPE (*)[65536])table;

#define CRCEA_BY4_SEXDECTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 4);                                             \
        const uint16_t n0 = LOAD16(IN + 0); \
        const uint16_t n1 = LOAD16(IN + 2); \
        state = SHIFT(state, 32) ^                                          \
                t[1][n0 ^ SLICE(state,  0, 16)] ^                                          \
                t[0][n1 ^ SLICE(state, 16, 16)];                 \
    CRCEA_UPDATE_BYTE(IN, END);                                                                    \
        state = SHIFT(state, 8) ^ t[0][INDEX16((uint8_t)*IN ^ SLICE(state, 0, 8))];  \
    CRCEA_UPDATE_END();                                                                     \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY4_SEXDECTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY4_SEXDECTET */

#ifdef CRCEA_ENABLE_BY8_SEXDECTET

/*
 * Slicing by Quadruple Sexdectet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY8_SEXDECTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[65536] = (const CRCEA_TYPE (*)[65536])table;

#define CRCEA_BY8_SEXDECTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 8);                                             \
        const uint16_t n0 = LOAD16(IN + 0); \
        const uint16_t n1 = LOAD16(IN + 2); \
        const uint16_t n2 = LOAD16(IN + 4); \
        const uint16_t n3 = LOAD16(IN + 6); \
        state = SHIFT(state, 64) ^                                          \
                t[3][n0 ^ SLICE(state,  0, 16)] ^                                          \
                t[2][n1 ^ SLICE(state, 16, 16)] ^                                          \
                t[1][n2 ^ SLICE(state, 32, 16)] ^                                          \
                t[0][n3 ^ SLICE(state, 48, 16)];                 \
    CRCEA_UPDATE_BYTE(IN, END);                                                                    \
        state = SHIFT(state, 8) ^ t[0][INDEX16((uint8_t)*IN ^ SLICE(state, 0, 8))];  \
    CRCEA_UPDATE_END();                                                                     \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY8_SEXDECTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY8_SEXDECTET */

#ifdef CRCEA_ENABLE_BY16_SEXDECTET

/*
 * Slicing by Octuple Sexdectet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY16_SEXDECTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[65536] = (const CRCEA_TYPE (*)[65536])table;

#define CRCEA_BY16_SEXDECTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 16);                                             \
        const uint16_t n0 = LOAD16(IN +  0); \
        const uint16_t n1 = LOAD16(IN +  2); \
        const uint16_t n2 = LOAD16(IN +  4); \
        const uint16_t n3 = LOAD16(IN +  6); \
        const uint16_t n4 = LOAD16(IN +  8); \
        const uint16_t n5 = LOAD16(IN + 10); \
        const uint16_t n6 = LOAD16(IN + 12); \
        const uint16_t n7 = LOAD16(IN + 14); \
        state = SHIFT(state, 128) ^                                          \
                t[7][n0 ^ SLICE(state,   0, 16)] ^                                          \
                t[6][n1 ^ SLICE(state,  16, 16)] ^                                          \
                t[5][n2 ^ SLICE(state,  32, 16)] ^                                          \
                t[4][n3 ^ SLICE(state,  48, 16)] ^                                          \
                t[3][n4 ^ SLICE(state,  64, 16)] ^                                          \
                t[2][n5 ^ SLICE(state,  80, 16)] ^                                          \
                t[1][n6 ^ SLICE(state,  96, 16)] ^                                          \
                t[0][n7 ^ SLICE(state, 112, 16)];                 \
    CRCEA_UPDATE_BYTE(IN, END);                                                                    \
        state = SHIFT(state, 8) ^ t[0][INDEX16((uint8_t)*IN ^ SLICE(state, 0, 8))];  \
    CRCEA_UPDATE_END();                                                                     \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY16_SEXDECTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY16_SEXDECTET */

#ifdef CRCEA_ENABLE_BY32_SEXDECTET

/*
 * Slicing by Sexdecuple Sexdectet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY32_SEXDECTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[65536] = (const CRCEA_TYPE (*)[65536])table;

#define CRCEA_BY32_SEXDECTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 32);                                             \
        const uint16_t n0  = LOAD16(IN +  0); \
        const uint16_t n1  = LOAD16(IN +  2); \
        const uint16_t n2  = LOAD16(IN +  4); \
        const uint16_t n3  = LOAD16(IN +  6); \
        const uint16_t n4  = LOAD16(IN +  8); \
        const uint16_t n5  = LOAD16(IN + 10); \
        const uint16_t n6  = LOAD16(IN + 12); \
        const uint16_t n7  = LOAD16(IN + 14); \
        const uint16_t n8  = LOAD16(IN + 16); \
        const uint16_t n9  = LOAD16(IN + 18); \
        const uint16_t n10 = LOAD16(IN + 20); \
        const uint16_t n11 = LOAD16(IN + 22); \
        const uint16_t n12 = LOAD16(IN + 24); \
        const uint16_t n13 = LOAD16(IN + 26); \
        const uint16_t n14 = LOAD16(IN + 28); \
        const uint16_t n15 = LOAD16(IN + 30); \
        state = SHIFT(state, 256) ^                                          \
                t[15][n0  ^ SLICE(state,   0, 16)] ^                                          \
                t[14][n1  ^ SLICE(state,  16, 16)] ^                                          \
                t[13][n2  ^ SLICE(state,  32, 16)] ^                                          \
                t[12][n3  ^ SLICE(state,  48, 16)] ^                                          \
                t[11][n4  ^ SLICE(state,  64, 16)] ^                                          \
                t[10][n5  ^ SLICE(state,  80, 16)] ^                                          \
                t[ 9][n6  ^ SLICE(state,  96, 16)] ^                                          \
                t[ 8][n7  ^ SLICE(state, 112, 16)] ^                                          \
                t[ 7][n8  ^ SLICE(state, 128, 16)] ^                                          \
                t[ 6][n9  ^ SLICE(state, 144, 16)] ^                                          \
                t[ 5][n10 ^ SLICE(state, 160, 16)] ^                                          \
                t[ 4][n11 ^ SLICE(state, 176, 16)] ^                                          \
                t[ 3][n12 ^ SLICE(state, 192, 16)] ^                                          \
                t[ 2][n13 ^ SLICE(state, 208, 16)] ^                                          \
                t[ 1][n14 ^ SLICE(state, 224, 16)] ^                                          \
                t[ 0][n15 ^ SLICE(state, 240, 16)];                 \
    CRCEA_UPDATE_BYTE(IN, END);                                                                    \
        state = SHIFT(state, 8) ^ t[0][INDEX16((uint8_t)*IN ^ SLICE(state, 0, 8))];  \
    CRCEA_UPDATE_END();                                                                     \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY32_SEXDECTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY32_SEXDECTET */
