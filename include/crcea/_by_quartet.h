/**
 * @file _by_quartet.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 */

#ifdef CRCEA_ENABLE_BY_QUARTET

/*
 * Slicing by Single Quartet
 *
 * This algorithm is Half-Byte Algorithm.
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE *t = (const CRCEA_TYPE *)table;

#define CRCEA_BY_QUARTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                             \
        state ^= INPUT(*IN);                                           \
        state = SHIFT(state, 4) ^ t[SLICE(state, 0, 4)];                   \
        state = SHIFT(state, 4) ^ t[SLICE(state, 0, 4)];                   \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                           \
        state = SHIFT(state, 4) ^ t[SLICE(state, 0, 4)];                   \
        state = SHIFT(state, 4) ^ t[SLICE(state, 0, 4)];                   \
    CRCEA_UPDATE_END();                                             \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY_QUARTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY_QUARTET */

#ifdef CRCEA_ENABLE_BY1_QUARTET

/*
 * Slicing by Double Quartet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY1_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[16] = (const CRCEA_TYPE (*)[16])table;

#define CRCEA_BY1_QUARTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                             \
        state ^= INPUT(*IN);                                           \
        const uint8_t n = SLICE(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[1][SLICE8(n, 0, 4)] ^ \
                t[0][SLICE8(n, 4, 4)];                   \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                           \
        const uint8_t n = SLICE(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[1][SLICE8(n, 0, 4)] ^ \
                t[0][SLICE8(n, 4, 4)];                   \
    CRCEA_UPDATE_END();                                             \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY1_QUARTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY1_QUARTET */

#ifdef CRCEA_ENABLE_BY2_QUARTET

/*
 * Slicing by Quadruple Quartet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY2_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[16] = (const CRCEA_TYPE (*)[16])table;

#define CRCEA_BY2_QUARTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 2);                                             \
        const uint8_t n0 = (uint8_t)IN[0] ^ SLICE(state, 0, 8); \
        const uint8_t n1 = (uint8_t)IN[1] ^ SLICE(state, 8, 8); \
        state = SHIFT(state, 16) ^ \
                t[3][SLICE8(n0, 0, 4)] ^ \
                t[2][SLICE8(n0, 4, 4)] ^ \
                t[1][SLICE8(n1, 0, 4)] ^ \
                t[0][SLICE8(n1, 4, 4)]; \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                           \
        const uint8_t n = SLICE(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[1][SLICE8(n, 0, 4)] ^ \
                t[0][SLICE8(n, 4, 4)];                   \
    CRCEA_UPDATE_END();                                                                   \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY2_QUARTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY2_QUARTET */

#ifdef CRCEA_ENABLE_BY4_QUARTET

/*
 * Slicing by Octuple Quartet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY4_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[16] = (const CRCEA_TYPE (*)[16])table;

#define CRCEA_BY4_QUARTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 4);                                             \
        const uint8_t n0 = (uint8_t)IN[0] ^ (uint8_t)SLICE(state,  0, 8); \
        const uint8_t n1 = (uint8_t)IN[1] ^ (uint8_t)SLICE(state,  8, 8); \
        const uint8_t n2 = (uint8_t)IN[2] ^ (uint8_t)SLICE(state, 16, 8); \
        const uint8_t n3 = (uint8_t)IN[3] ^ (uint8_t)SLICE(state, 24, 8); \
        state = SHIFT(state, 32) ^ \
                t[7][SLICE8(n0, 0, 4)] ^ \
                t[6][SLICE8(n0, 4, 4)] ^ \
                t[5][SLICE8(n1, 0, 4)] ^ \
                t[4][SLICE8(n1, 4, 4)] ^ \
                t[3][SLICE8(n2, 0, 4)] ^ \
                t[2][SLICE8(n2, 4, 4)] ^ \
                t[1][SLICE8(n3, 0, 4)] ^ \
                t[0][SLICE8(n3, 4, 4)]; \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                           \
        const uint8_t n = SLICE(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[1][SLICE8(n, 0, 4)] ^ \
                t[0][SLICE8(n, 4, 4)];                   \
    CRCEA_UPDATE_END();                                                                   \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY4_QUARTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY4_QUARTET */

#ifdef CRCEA_ENABLE_BY8_QUARTET

/*
 * Slicing by Sexdecuple Quartet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY8_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[16] = (const CRCEA_TYPE (*)[16])table;

#define CRCEA_BY8_QUARTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 8);                                             \
        const uint8_t n0 = (uint8_t)IN[0] ^ (uint8_t)SLICE(state,  0, 8); \
        const uint8_t n1 = (uint8_t)IN[1] ^ (uint8_t)SLICE(state,  8, 8); \
        const uint8_t n2 = (uint8_t)IN[2] ^ (uint8_t)SLICE(state, 16, 8); \
        const uint8_t n3 = (uint8_t)IN[3] ^ (uint8_t)SLICE(state, 24, 8); \
        const uint8_t n4 = (uint8_t)IN[4] ^ (uint8_t)SLICE(state, 32, 8); \
        const uint8_t n5 = (uint8_t)IN[5] ^ (uint8_t)SLICE(state, 40, 8); \
        const uint8_t n6 = (uint8_t)IN[6] ^ (uint8_t)SLICE(state, 48, 8); \
        const uint8_t n7 = (uint8_t)IN[7] ^ (uint8_t)SLICE(state, 56, 8); \
        state = SHIFT(state, 64) ^ \
                t[15][SLICE8(n0, 0, 4)] ^ \
                t[14][SLICE8(n0, 4, 4)] ^ \
                t[13][SLICE8(n1, 0, 4)] ^ \
                t[12][SLICE8(n1, 4, 4)] ^ \
                t[11][SLICE8(n2, 0, 4)] ^ \
                t[10][SLICE8(n2, 4, 4)] ^ \
                t[ 9][SLICE8(n3, 0, 4)] ^ \
                t[ 8][SLICE8(n3, 4, 4)] ^ \
                t[ 7][SLICE8(n4, 0, 4)] ^ \
                t[ 6][SLICE8(n4, 4, 4)] ^ \
                t[ 5][SLICE8(n5, 0, 4)] ^ \
                t[ 4][SLICE8(n5, 4, 4)] ^ \
                t[ 3][SLICE8(n6, 0, 4)] ^ \
                t[ 2][SLICE8(n6, 4, 4)] ^ \
                t[ 1][SLICE8(n7, 0, 4)] ^ \
                t[ 0][SLICE8(n7, 4, 4)]; \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                           \
        const uint8_t n = SLICE(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[1][SLICE8(n, 0, 4)] ^ \
                t[0][SLICE8(n, 4, 4)];                   \
    CRCEA_UPDATE_END();                                                                   \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY8_QUARTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY8_QUARTET */

#ifdef CRCEA_ENABLE_BY16_QUARTET

/*
 * Slicing by Duotriguple Quartet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY16_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[16] = (const CRCEA_TYPE (*)[16])table;

#define CRCEA_BY16_QUARTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 16);                                             \
        const uint8_t n0  = (uint8_t)IN[ 0] ^ (uint8_t)SLICE(state,   0, 8); \
        const uint8_t n1  = (uint8_t)IN[ 1] ^ (uint8_t)SLICE(state,   8, 8); \
        const uint8_t n2  = (uint8_t)IN[ 2] ^ (uint8_t)SLICE(state,  16, 8); \
        const uint8_t n3  = (uint8_t)IN[ 3] ^ (uint8_t)SLICE(state,  24, 8); \
        const uint8_t n4  = (uint8_t)IN[ 4] ^ (uint8_t)SLICE(state,  32, 8); \
        const uint8_t n5  = (uint8_t)IN[ 5] ^ (uint8_t)SLICE(state,  40, 8); \
        const uint8_t n6  = (uint8_t)IN[ 6] ^ (uint8_t)SLICE(state,  48, 8); \
        const uint8_t n7  = (uint8_t)IN[ 7] ^ (uint8_t)SLICE(state,  56, 8); \
        const uint8_t n8  = (uint8_t)IN[ 8] ^ (uint8_t)SLICE(state,  64, 8); \
        const uint8_t n9  = (uint8_t)IN[ 9] ^ (uint8_t)SLICE(state,  72, 8); \
        const uint8_t n10 = (uint8_t)IN[10] ^ (uint8_t)SLICE(state,  80, 8); \
        const uint8_t n11 = (uint8_t)IN[11] ^ (uint8_t)SLICE(state,  88, 8); \
        const uint8_t n12 = (uint8_t)IN[12] ^ (uint8_t)SLICE(state,  96, 8); \
        const uint8_t n13 = (uint8_t)IN[13] ^ (uint8_t)SLICE(state, 104, 8); \
        const uint8_t n14 = (uint8_t)IN[14] ^ (uint8_t)SLICE(state, 112, 8); \
        const uint8_t n15 = (uint8_t)IN[15] ^ (uint8_t)SLICE(state, 120, 8); \
        state = SHIFT(state, 128) ^ \
                t[31][SLICE8(n0,  0, 4)] ^ \
                t[30][SLICE8(n0,  4, 4)] ^ \
                t[29][SLICE8(n1,  0, 4)] ^ \
                t[28][SLICE8(n1,  4, 4)] ^ \
                t[27][SLICE8(n2,  0, 4)] ^ \
                t[26][SLICE8(n2,  4, 4)] ^ \
                t[25][SLICE8(n3,  0, 4)] ^ \
                t[24][SLICE8(n3,  4, 4)] ^ \
                t[23][SLICE8(n4,  0, 4)] ^ \
                t[22][SLICE8(n4,  4, 4)] ^ \
                t[21][SLICE8(n5,  0, 4)] ^ \
                t[20][SLICE8(n5,  4, 4)] ^ \
                t[19][SLICE8(n6,  0, 4)] ^ \
                t[18][SLICE8(n6,  4, 4)] ^ \
                t[17][SLICE8(n7,  0, 4)] ^ \
                t[16][SLICE8(n7,  4, 4)] ^ \
                t[15][SLICE8(n8,  0, 4)] ^ \
                t[14][SLICE8(n8,  4, 4)] ^ \
                t[13][SLICE8(n9,  0, 4)] ^ \
                t[12][SLICE8(n9,  4, 4)] ^ \
                t[11][SLICE8(n10, 0, 4)] ^ \
                t[10][SLICE8(n10, 4, 4)] ^ \
                t[ 9][SLICE8(n11, 0, 4)] ^ \
                t[ 8][SLICE8(n11, 4, 4)] ^ \
                t[ 7][SLICE8(n12, 0, 4)] ^ \
                t[ 6][SLICE8(n12, 4, 4)] ^ \
                t[ 5][SLICE8(n13, 0, 4)] ^ \
                t[ 4][SLICE8(n13, 4, 4)] ^ \
                t[ 3][SLICE8(n14, 0, 4)] ^ \
                t[ 2][SLICE8(n14, 4, 4)] ^ \
                t[ 1][SLICE8(n15, 0, 4)] ^ \
                t[ 0][SLICE8(n15, 4, 4)]; \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                           \
        const uint8_t n = SLICE(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[1][SLICE8(n, 0, 4)] ^ \
                t[0][SLICE8(n, 4, 4)];                   \
    CRCEA_UPDATE_END();                                                                   \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY16_QUARTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY16_QUARTET */

#ifdef CRCEA_ENABLE_BY32_QUARTET

/*
 * Slicing by Quattuorsexaguple Quartet
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY32_QUARTET(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[16] = (const CRCEA_TYPE (*)[16])table;

#define CRCEA_BY32_QUARTET_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 32);                                             \
        const uint8_t n0  = (uint8_t)IN[ 0] ^ (uint8_t)SLICE(state,   0, 8); \
        const uint8_t n1  = (uint8_t)IN[ 1] ^ (uint8_t)SLICE(state,   8, 8); \
        const uint8_t n2  = (uint8_t)IN[ 2] ^ (uint8_t)SLICE(state,  16, 8); \
        const uint8_t n3  = (uint8_t)IN[ 3] ^ (uint8_t)SLICE(state,  24, 8); \
        const uint8_t n4  = (uint8_t)IN[ 4] ^ (uint8_t)SLICE(state,  32, 8); \
        const uint8_t n5  = (uint8_t)IN[ 5] ^ (uint8_t)SLICE(state,  40, 8); \
        const uint8_t n6  = (uint8_t)IN[ 6] ^ (uint8_t)SLICE(state,  48, 8); \
        const uint8_t n7  = (uint8_t)IN[ 7] ^ (uint8_t)SLICE(state,  56, 8); \
        const uint8_t n8  = (uint8_t)IN[ 8] ^ (uint8_t)SLICE(state,  64, 8); \
        const uint8_t n9  = (uint8_t)IN[ 9] ^ (uint8_t)SLICE(state,  72, 8); \
        const uint8_t n10 = (uint8_t)IN[10] ^ (uint8_t)SLICE(state,  80, 8); \
        const uint8_t n11 = (uint8_t)IN[11] ^ (uint8_t)SLICE(state,  88, 8); \
        const uint8_t n12 = (uint8_t)IN[12] ^ (uint8_t)SLICE(state,  96, 8); \
        const uint8_t n13 = (uint8_t)IN[13] ^ (uint8_t)SLICE(state, 104, 8); \
        const uint8_t n14 = (uint8_t)IN[14] ^ (uint8_t)SLICE(state, 112, 8); \
        const uint8_t n15 = (uint8_t)IN[15] ^ (uint8_t)SLICE(state, 120, 8); \
        const uint8_t n16 = (uint8_t)IN[16] ^ (uint8_t)SLICE(state, 128, 8); \
        const uint8_t n17 = (uint8_t)IN[17] ^ (uint8_t)SLICE(state, 136, 8); \
        const uint8_t n18 = (uint8_t)IN[18] ^ (uint8_t)SLICE(state, 144, 8); \
        const uint8_t n19 = (uint8_t)IN[19] ^ (uint8_t)SLICE(state, 152, 8); \
        const uint8_t n20 = (uint8_t)IN[20] ^ (uint8_t)SLICE(state, 160, 8); \
        const uint8_t n21 = (uint8_t)IN[21] ^ (uint8_t)SLICE(state, 168, 8); \
        const uint8_t n22 = (uint8_t)IN[22] ^ (uint8_t)SLICE(state, 176, 8); \
        const uint8_t n23 = (uint8_t)IN[23] ^ (uint8_t)SLICE(state, 184, 8); \
        const uint8_t n24 = (uint8_t)IN[24] ^ (uint8_t)SLICE(state, 192, 8); \
        const uint8_t n25 = (uint8_t)IN[25] ^ (uint8_t)SLICE(state, 200, 8); \
        const uint8_t n26 = (uint8_t)IN[26] ^ (uint8_t)SLICE(state, 208, 8); \
        const uint8_t n27 = (uint8_t)IN[27] ^ (uint8_t)SLICE(state, 216, 8); \
        const uint8_t n28 = (uint8_t)IN[28] ^ (uint8_t)SLICE(state, 224, 8); \
        const uint8_t n29 = (uint8_t)IN[29] ^ (uint8_t)SLICE(state, 232, 8); \
        const uint8_t n30 = (uint8_t)IN[30] ^ (uint8_t)SLICE(state, 240, 8); \
        const uint8_t n31 = (uint8_t)IN[31] ^ (uint8_t)SLICE(state, 248, 8); \
        state = SHIFT(state, 256) ^ \
                t[63][SLICE8(n0,  0, 4)] ^ \
                t[62][SLICE8(n0,  4, 4)] ^ \
                t[61][SLICE8(n1,  0, 4)] ^ \
                t[60][SLICE8(n1,  4, 4)] ^ \
                t[59][SLICE8(n2,  0, 4)] ^ \
                t[58][SLICE8(n2,  4, 4)] ^ \
                t[57][SLICE8(n3,  0, 4)] ^ \
                t[56][SLICE8(n3,  4, 4)] ^ \
                t[55][SLICE8(n4,  0, 4)] ^ \
                t[54][SLICE8(n4,  4, 4)] ^ \
                t[53][SLICE8(n5,  0, 4)] ^ \
                t[52][SLICE8(n5,  4, 4)] ^ \
                t[51][SLICE8(n6,  0, 4)] ^ \
                t[50][SLICE8(n6,  4, 4)] ^ \
                t[49][SLICE8(n7,  0, 4)] ^ \
                t[48][SLICE8(n7,  4, 4)] ^ \
                t[47][SLICE8(n8,  0, 4)] ^ \
                t[46][SLICE8(n8,  4, 4)] ^ \
                t[45][SLICE8(n9,  0, 4)] ^ \
                t[44][SLICE8(n9,  4, 4)] ^ \
                t[43][SLICE8(n10, 0, 4)] ^ \
                t[42][SLICE8(n10, 4, 4)] ^ \
                t[41][SLICE8(n11, 0, 4)] ^ \
                t[40][SLICE8(n11, 4, 4)] ^ \
                t[39][SLICE8(n12, 0, 4)] ^ \
                t[38][SLICE8(n12, 4, 4)] ^ \
                t[37][SLICE8(n13, 0, 4)] ^ \
                t[36][SLICE8(n13, 4, 4)] ^ \
                t[35][SLICE8(n14, 0, 4)] ^ \
                t[34][SLICE8(n14, 4, 4)] ^ \
                t[33][SLICE8(n15, 0, 4)] ^ \
                t[32][SLICE8(n15, 4, 4)] ^ \
                t[31][SLICE8(n16, 0, 4)] ^ \
                t[30][SLICE8(n16, 4, 4)] ^ \
                t[29][SLICE8(n17, 0, 4)] ^ \
                t[28][SLICE8(n17, 4, 4)] ^ \
                t[27][SLICE8(n18, 0, 4)] ^ \
                t[26][SLICE8(n18, 4, 4)] ^ \
                t[25][SLICE8(n19, 0, 4)] ^ \
                t[24][SLICE8(n19, 4, 4)] ^ \
                t[23][SLICE8(n20, 0, 4)] ^ \
                t[22][SLICE8(n20, 4, 4)] ^ \
                t[21][SLICE8(n21, 0, 4)] ^ \
                t[20][SLICE8(n21, 4, 4)] ^ \
                t[19][SLICE8(n22, 0, 4)] ^ \
                t[18][SLICE8(n22, 4, 4)] ^ \
                t[17][SLICE8(n23, 0, 4)] ^ \
                t[16][SLICE8(n23, 4, 4)] ^ \
                t[15][SLICE8(n24, 0, 4)] ^ \
                t[14][SLICE8(n24, 4, 4)] ^ \
                t[13][SLICE8(n25, 0, 4)] ^ \
                t[12][SLICE8(n25, 4, 4)] ^ \
                t[11][SLICE8(n26, 0, 4)] ^ \
                t[10][SLICE8(n26, 4, 4)] ^ \
                t[ 9][SLICE8(n27, 0, 4)] ^ \
                t[ 8][SLICE8(n27, 4, 4)] ^ \
                t[ 7][SLICE8(n28, 0, 4)] ^ \
                t[ 6][SLICE8(n28, 4, 4)] ^ \
                t[ 5][SLICE8(n29, 0, 4)] ^ \
                t[ 4][SLICE8(n29, 4, 4)] ^ \
                t[ 3][SLICE8(n30, 0, 4)] ^ \
                t[ 2][SLICE8(n30, 4, 4)] ^ \
                t[ 1][SLICE8(n31, 0, 4)] ^ \
                t[ 0][SLICE8(n31, 4, 4)]; \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                           \
        const uint8_t n = SLICE(state, 0, 8); \
        state = SHIFT(state, 8) ^ \
                t[1][SLICE8(n, 0, 4)] ^ \
                t[0][SLICE8(n, 4, 4)];                   \
    CRCEA_UPDATE_END();                                                                   \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY32_QUARTET_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY32_QUARTET */
