/**
 * @file _by_duo.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 */

#ifdef CRCEA_ENABLE_BY_DUO

/*
 * Slicing by Single Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY_DUO(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE *t = (const CRCEA_TYPE *)table;

#define CRCEA_BY_DUO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                        \
        state ^= INPUT(*IN);                                                \
        state = SHIFT(state, 2) ^ t[SLICE(state, 0, 2)];                    \
        state = SHIFT(state, 2) ^ t[SLICE(state, 0, 2)];                    \
        state = SHIFT(state, 2) ^ t[SLICE(state, 0, 2)];                    \
        state = SHIFT(state, 2) ^ t[SLICE(state, 0, 2)];                    \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        state = SHIFT(state, 2) ^ t[SLICE(state, 0, 2)];                    \
        state = SHIFT(state, 2) ^ t[SLICE(state, 0, 2)];                    \
        state = SHIFT(state, 2) ^ t[SLICE(state, 0, 2)];                    \
        state = SHIFT(state, 2) ^ t[SLICE(state, 0, 2)];                    \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BY_DUO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY_DUO */

#ifdef CRCEA_ENABLE_BY1_DUO

/*
 * Slicing by Quadruple Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY1_DUO(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[4] = (const CRCEA_TYPE (*)[4])table;

#define CRCEA_BY1_DUO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                        \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[3][SLICE8(n, 0, 2)] ^                                     \
                t[2][SLICE8(n, 2, 2)] ^                                     \
                t[1][SLICE8(n, 4, 2)] ^                                     \
                t[0][SLICE8(n, 6, 2)];                                      \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[3][SLICE8(n, 0, 2)] ^                                     \
                t[2][SLICE8(n, 2, 2)] ^                                     \
                t[1][SLICE8(n, 4, 2)] ^                                     \
                t[0][SLICE8(n, 6, 2)];                                      \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BY1_DUO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY1_DUO */

#ifdef CRCEA_ENABLE_BY2_DUO

/*
 * Slicing by Octuple Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY2_DUO(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[4] = (const CRCEA_TYPE (*)[4])table;

#define CRCEA_BY2_DUO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 2);                                        \
        const uint8_t n0 = (uint8_t)IN[0] ^ SLICE(state, 0, 8);             \
        const uint8_t n1 = (uint8_t)IN[1] ^ SLICE(state, 8, 8);             \
        state = SHIFT(state, 16) ^                                          \
                t[7][SLICE8(n0, 0, 2)] ^                                    \
                t[6][SLICE8(n0, 2, 2)] ^                                    \
                t[5][SLICE8(n0, 4, 2)] ^                                    \
                t[4][SLICE8(n0, 6, 2)] ^                                    \
                t[3][SLICE8(n1, 0, 2)] ^                                    \
                t[2][SLICE8(n1, 2, 2)] ^                                    \
                t[1][SLICE8(n1, 4, 2)] ^                                    \
                t[0][SLICE8(n1, 6, 2)];                                     \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[3][SLICE8(n, 0, 2)] ^                                     \
                t[2][SLICE8(n, 2, 2)] ^                                     \
                t[1][SLICE8(n, 4, 2)] ^                                     \
                t[0][SLICE8(n, 6, 2)];                                      \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BY2_DUO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY2_DUO */

#ifdef CRCEA_ENABLE_BY4_DUO

/*
 * Slicing by Sexdecuple Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY4_DUO(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[4] = (const CRCEA_TYPE (*)[4])table;

#define CRCEA_BY4_DUO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 4);                                        \
        const uint8_t n0 = (uint8_t)IN[0] ^ SLICE(state,  0, 8);            \
        const uint8_t n1 = (uint8_t)IN[1] ^ SLICE(state,  8, 8);            \
        const uint8_t n2 = (uint8_t)IN[2] ^ SLICE(state, 16, 8);            \
        const uint8_t n3 = (uint8_t)IN[3] ^ SLICE(state, 24, 8);            \
        state = SHIFT(state, 32) ^                                          \
                t[15][SLICE8(n0, 0, 2)] ^                                   \
                t[14][SLICE8(n0, 2, 2)] ^                                   \
                t[13][SLICE8(n0, 4, 2)] ^                                   \
                t[12][SLICE8(n0, 6, 2)] ^                                   \
                t[11][SLICE8(n1, 0, 2)] ^                                   \
                t[10][SLICE8(n1, 2, 2)] ^                                   \
                t[ 9][SLICE8(n1, 4, 2)] ^                                   \
                t[ 8][SLICE8(n1, 6, 2)] ^                                   \
                t[ 7][SLICE8(n2, 0, 2)] ^                                   \
                t[ 6][SLICE8(n2, 2, 2)] ^                                   \
                t[ 5][SLICE8(n2, 4, 2)] ^                                   \
                t[ 4][SLICE8(n2, 6, 2)] ^                                   \
                t[ 3][SLICE8(n3, 0, 2)] ^                                   \
                t[ 2][SLICE8(n3, 2, 2)] ^                                   \
                t[ 1][SLICE8(n3, 4, 2)] ^                                   \
                t[ 0][SLICE8(n3, 6, 2)];                                    \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[3][SLICE8(n, 0, 2)] ^                                     \
                t[2][SLICE8(n, 2, 2)] ^                                     \
                t[1][SLICE8(n, 4, 2)] ^                                     \
                t[0][SLICE8(n, 6, 2)];                                      \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BY4_DUO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY4_DUO */

#ifdef CRCEA_ENABLE_BY8_DUO

/*
 * Slicing by Duotriguple Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY8_DUO(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[4] = (const CRCEA_TYPE (*)[4])table;

#define CRCEA_BY8_DUO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 8);                                        \
        const uint8_t n0 = (uint8_t)IN[0] ^ SLICE(state,  0, 8);            \
        const uint8_t n1 = (uint8_t)IN[1] ^ SLICE(state,  8, 8);            \
        const uint8_t n2 = (uint8_t)IN[2] ^ SLICE(state, 16, 8);            \
        const uint8_t n3 = (uint8_t)IN[3] ^ SLICE(state, 24, 8);            \
        const uint8_t n4 = (uint8_t)IN[4] ^ SLICE(state, 32, 8);            \
        const uint8_t n5 = (uint8_t)IN[5] ^ SLICE(state, 40, 8);            \
        const uint8_t n6 = (uint8_t)IN[6] ^ SLICE(state, 48, 8);            \
        const uint8_t n7 = (uint8_t)IN[7] ^ SLICE(state, 56, 8);            \
        state = SHIFT(state, 64) ^                                          \
                t[31][SLICE8(n0, 0, 2)] ^                                   \
                t[30][SLICE8(n0, 2, 2)] ^                                   \
                t[29][SLICE8(n0, 4, 2)] ^                                   \
                t[28][SLICE8(n0, 6, 2)] ^                                   \
                t[27][SLICE8(n1, 0, 2)] ^                                   \
                t[26][SLICE8(n1, 2, 2)] ^                                   \
                t[25][SLICE8(n1, 4, 2)] ^                                   \
                t[24][SLICE8(n1, 6, 2)] ^                                   \
                t[23][SLICE8(n2, 0, 2)] ^                                   \
                t[22][SLICE8(n2, 2, 2)] ^                                   \
                t[21][SLICE8(n2, 4, 2)] ^                                   \
                t[20][SLICE8(n2, 6, 2)] ^                                   \
                t[19][SLICE8(n3, 0, 2)] ^                                   \
                t[18][SLICE8(n3, 2, 2)] ^                                   \
                t[17][SLICE8(n3, 4, 2)] ^                                   \
                t[16][SLICE8(n3, 6, 2)] ^                                   \
                t[15][SLICE8(n4, 0, 2)] ^                                   \
                t[14][SLICE8(n4, 2, 2)] ^                                   \
                t[13][SLICE8(n4, 4, 2)] ^                                   \
                t[12][SLICE8(n4, 6, 2)] ^                                   \
                t[11][SLICE8(n5, 0, 2)] ^                                   \
                t[10][SLICE8(n5, 2, 2)] ^                                   \
                t[ 9][SLICE8(n5, 4, 2)] ^                                   \
                t[ 8][SLICE8(n5, 6, 2)] ^                                   \
                t[ 7][SLICE8(n6, 0, 2)] ^                                   \
                t[ 6][SLICE8(n6, 2, 2)] ^                                   \
                t[ 5][SLICE8(n6, 4, 2)] ^                                   \
                t[ 4][SLICE8(n6, 6, 2)] ^                                   \
                t[ 3][SLICE8(n7, 0, 2)] ^                                   \
                t[ 2][SLICE8(n7, 2, 2)] ^                                   \
                t[ 1][SLICE8(n7, 4, 2)] ^                                   \
                t[ 0][SLICE8(n7, 6, 2)];                                    \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[3][SLICE8(n, 0, 2)] ^                                     \
                t[2][SLICE8(n, 2, 2)] ^                                     \
                t[1][SLICE8(n, 4, 2)] ^                                     \
                t[0][SLICE8(n, 6, 2)];                                      \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BY8_DUO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY8_DUO */

#ifdef CRCEA_ENABLE_BY16_DUO

/*
 * Slicing by Quattuorsexaguple Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY16_DUO(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[4] = (const CRCEA_TYPE (*)[4])table;

#define CRCEA_BY16_DUO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 16);                                       \
        const uint8_t n0  = (uint8_t)IN[ 0] ^ SLICE(state,   0, 8);         \
        const uint8_t n1  = (uint8_t)IN[ 1] ^ SLICE(state,   8, 8);         \
        const uint8_t n2  = (uint8_t)IN[ 2] ^ SLICE(state,  16, 8);         \
        const uint8_t n3  = (uint8_t)IN[ 3] ^ SLICE(state,  24, 8);         \
        const uint8_t n4  = (uint8_t)IN[ 4] ^ SLICE(state,  32, 8);         \
        const uint8_t n5  = (uint8_t)IN[ 5] ^ SLICE(state,  40, 8);         \
        const uint8_t n6  = (uint8_t)IN[ 6] ^ SLICE(state,  48, 8);         \
        const uint8_t n7  = (uint8_t)IN[ 7] ^ SLICE(state,  56, 8);         \
        const uint8_t n8  = (uint8_t)IN[ 8] ^ SLICE(state,  64, 8);         \
        const uint8_t n9  = (uint8_t)IN[ 9] ^ SLICE(state,  72, 8);         \
        const uint8_t n10 = (uint8_t)IN[10] ^ SLICE(state,  80, 8);         \
        const uint8_t n11 = (uint8_t)IN[11] ^ SLICE(state,  88, 8);         \
        const uint8_t n12 = (uint8_t)IN[12] ^ SLICE(state,  96, 8);         \
        const uint8_t n13 = (uint8_t)IN[13] ^ SLICE(state, 104, 8);         \
        const uint8_t n14 = (uint8_t)IN[14] ^ SLICE(state, 112, 8);         \
        const uint8_t n15 = (uint8_t)IN[15] ^ SLICE(state, 120, 8);         \
        state = SHIFT(state, 128) ^                                         \
                t[63][SLICE8(n0,  0, 2)] ^                                  \
                t[62][SLICE8(n0,  2, 2)] ^                                  \
                t[61][SLICE8(n0,  4, 2)] ^                                  \
                t[60][SLICE8(n0,  6, 2)] ^                                  \
                t[59][SLICE8(n1,  0, 2)] ^                                  \
                t[58][SLICE8(n1,  2, 2)] ^                                  \
                t[57][SLICE8(n1,  4, 2)] ^                                  \
                t[56][SLICE8(n1,  6, 2)] ^                                  \
                t[55][SLICE8(n2,  0, 2)] ^                                  \
                t[54][SLICE8(n2,  2, 2)] ^                                  \
                t[53][SLICE8(n2,  4, 2)] ^                                  \
                t[52][SLICE8(n2,  6, 2)] ^                                  \
                t[51][SLICE8(n3,  0, 2)] ^                                  \
                t[50][SLICE8(n3,  2, 2)] ^                                  \
                t[49][SLICE8(n3,  4, 2)] ^                                  \
                t[48][SLICE8(n3,  6, 2)] ^                                  \
                t[47][SLICE8(n4,  0, 2)] ^                                  \
                t[46][SLICE8(n4,  2, 2)] ^                                  \
                t[45][SLICE8(n4,  4, 2)] ^                                  \
                t[44][SLICE8(n4,  6, 2)] ^                                  \
                t[43][SLICE8(n5,  0, 2)] ^                                  \
                t[42][SLICE8(n5,  2, 2)] ^                                  \
                t[41][SLICE8(n5,  4, 2)] ^                                  \
                t[40][SLICE8(n5,  6, 2)] ^                                  \
                t[39][SLICE8(n6,  0, 2)] ^                                  \
                t[38][SLICE8(n6,  2, 2)] ^                                  \
                t[37][SLICE8(n6,  4, 2)] ^                                  \
                t[36][SLICE8(n6,  6, 2)] ^                                  \
                t[35][SLICE8(n7,  0, 2)] ^                                  \
                t[34][SLICE8(n7,  2, 2)] ^                                  \
                t[33][SLICE8(n7,  4, 2)] ^                                  \
                t[32][SLICE8(n7,  6, 2)] ^                                  \
                t[31][SLICE8(n8,  0, 2)] ^                                  \
                t[30][SLICE8(n8,  2, 2)] ^                                  \
                t[29][SLICE8(n8,  4, 2)] ^                                  \
                t[28][SLICE8(n8,  6, 2)] ^                                  \
                t[27][SLICE8(n9,  0, 2)] ^                                  \
                t[26][SLICE8(n9,  2, 2)] ^                                  \
                t[25][SLICE8(n9,  4, 2)] ^                                  \
                t[24][SLICE8(n9,  6, 2)] ^                                  \
                t[23][SLICE8(n10, 0, 2)] ^                                  \
                t[22][SLICE8(n10, 2, 2)] ^                                  \
                t[21][SLICE8(n10, 4, 2)] ^                                  \
                t[20][SLICE8(n10, 6, 2)] ^                                  \
                t[19][SLICE8(n11, 0, 2)] ^                                  \
                t[18][SLICE8(n11, 2, 2)] ^                                  \
                t[17][SLICE8(n11, 4, 2)] ^                                  \
                t[16][SLICE8(n11, 6, 2)] ^                                  \
                t[15][SLICE8(n12, 0, 2)] ^                                  \
                t[14][SLICE8(n12, 2, 2)] ^                                  \
                t[13][SLICE8(n12, 4, 2)] ^                                  \
                t[12][SLICE8(n12, 6, 2)] ^                                  \
                t[11][SLICE8(n13, 0, 2)] ^                                  \
                t[10][SLICE8(n13, 2, 2)] ^                                  \
                t[ 9][SLICE8(n13, 4, 2)] ^                                  \
                t[ 8][SLICE8(n13, 6, 2)] ^                                  \
                t[ 7][SLICE8(n14, 0, 2)] ^                                  \
                t[ 6][SLICE8(n14, 2, 2)] ^                                  \
                t[ 5][SLICE8(n14, 4, 2)] ^                                  \
                t[ 4][SLICE8(n14, 6, 2)] ^                                  \
                t[ 3][SLICE8(n15, 0, 2)] ^                                  \
                t[ 2][SLICE8(n15, 2, 2)] ^                                  \
                t[ 1][SLICE8(n15, 4, 2)] ^                                  \
                t[ 0][SLICE8(n15, 6, 2)];                                   \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[3][SLICE8(n, 0, 2)] ^                                     \
                t[2][SLICE8(n, 2, 2)] ^                                     \
                t[1][SLICE8(n, 4, 2)] ^                                     \
                t[0][SLICE8(n, 6, 2)];                                      \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BY16_DUO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY16_DUO */

#ifdef CRCEA_ENABLE_BY32_DUO

/*
 * Slicing by Octovigcentuple Duo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY32_DUO(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[4] = (const CRCEA_TYPE (*)[4])table;

#define CRCEA_BY32_DUO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 32);                                       \
        const uint8_t n0  = (uint8_t)IN[ 0] ^ SLICE(state,   0, 8);         \
        const uint8_t n1  = (uint8_t)IN[ 1] ^ SLICE(state,   8, 8);         \
        const uint8_t n2  = (uint8_t)IN[ 2] ^ SLICE(state,  16, 8);         \
        const uint8_t n3  = (uint8_t)IN[ 3] ^ SLICE(state,  24, 8);         \
        const uint8_t n4  = (uint8_t)IN[ 4] ^ SLICE(state,  32, 8);         \
        const uint8_t n5  = (uint8_t)IN[ 5] ^ SLICE(state,  40, 8);         \
        const uint8_t n6  = (uint8_t)IN[ 6] ^ SLICE(state,  48, 8);         \
        const uint8_t n7  = (uint8_t)IN[ 7] ^ SLICE(state,  56, 8);         \
        const uint8_t n8  = (uint8_t)IN[ 8] ^ SLICE(state,  64, 8);         \
        const uint8_t n9  = (uint8_t)IN[ 9] ^ SLICE(state,  72, 8);         \
        const uint8_t n10 = (uint8_t)IN[10] ^ SLICE(state,  80, 8);         \
        const uint8_t n11 = (uint8_t)IN[11] ^ SLICE(state,  88, 8);         \
        const uint8_t n12 = (uint8_t)IN[12] ^ SLICE(state,  96, 8);         \
        const uint8_t n13 = (uint8_t)IN[13] ^ SLICE(state, 104, 8);         \
        const uint8_t n14 = (uint8_t)IN[14] ^ SLICE(state, 112, 8);         \
        const uint8_t n15 = (uint8_t)IN[15] ^ SLICE(state, 120, 8);         \
        const uint8_t n16 = (uint8_t)IN[16] ^ SLICE(state, 128, 8);         \
        const uint8_t n17 = (uint8_t)IN[17] ^ SLICE(state, 136, 8);         \
        const uint8_t n18 = (uint8_t)IN[18] ^ SLICE(state, 144, 8);         \
        const uint8_t n19 = (uint8_t)IN[19] ^ SLICE(state, 152, 8);         \
        const uint8_t n20 = (uint8_t)IN[20] ^ SLICE(state, 160, 8);         \
        const uint8_t n21 = (uint8_t)IN[21] ^ SLICE(state, 168, 8);         \
        const uint8_t n22 = (uint8_t)IN[22] ^ SLICE(state, 176, 8);         \
        const uint8_t n23 = (uint8_t)IN[23] ^ SLICE(state, 184, 8);         \
        const uint8_t n24 = (uint8_t)IN[24] ^ SLICE(state, 192, 8);         \
        const uint8_t n25 = (uint8_t)IN[25] ^ SLICE(state, 200, 8);         \
        const uint8_t n26 = (uint8_t)IN[26] ^ SLICE(state, 208, 8);         \
        const uint8_t n27 = (uint8_t)IN[27] ^ SLICE(state, 216, 8);         \
        const uint8_t n28 = (uint8_t)IN[28] ^ SLICE(state, 224, 8);         \
        const uint8_t n29 = (uint8_t)IN[29] ^ SLICE(state, 232, 8);         \
        const uint8_t n30 = (uint8_t)IN[30] ^ SLICE(state, 240, 8);         \
        const uint8_t n31 = (uint8_t)IN[31] ^ SLICE(state, 248, 8);         \
        state = SHIFT(state, 256) ^                                         \
                t[127][SLICE8(n0,  0, 2)] ^                                 \
                t[126][SLICE8(n0,  2, 2)] ^                                 \
                t[125][SLICE8(n0,  4, 2)] ^                                 \
                t[124][SLICE8(n0,  6, 2)] ^                                 \
                t[123][SLICE8(n1,  0, 2)] ^                                 \
                t[122][SLICE8(n1,  2, 2)] ^                                 \
                t[121][SLICE8(n1,  4, 2)] ^                                 \
                t[120][SLICE8(n1,  6, 2)] ^                                 \
                t[119][SLICE8(n2,  0, 2)] ^                                 \
                t[118][SLICE8(n2,  2, 2)] ^                                 \
                t[117][SLICE8(n2,  4, 2)] ^                                 \
                t[116][SLICE8(n2,  6, 2)] ^                                 \
                t[115][SLICE8(n3,  0, 2)] ^                                 \
                t[114][SLICE8(n3,  2, 2)] ^                                 \
                t[113][SLICE8(n3,  4, 2)] ^                                 \
                t[112][SLICE8(n3,  6, 2)] ^                                 \
                t[111][SLICE8(n4,  0, 2)] ^                                 \
                t[110][SLICE8(n4,  2, 2)] ^                                 \
                t[109][SLICE8(n4,  4, 2)] ^                                 \
                t[108][SLICE8(n4,  6, 2)] ^                                 \
                t[107][SLICE8(n5,  0, 2)] ^                                 \
                t[106][SLICE8(n5,  2, 2)] ^                                 \
                t[105][SLICE8(n5,  4, 2)] ^                                 \
                t[104][SLICE8(n5,  6, 2)] ^                                 \
                t[103][SLICE8(n6,  0, 2)] ^                                 \
                t[102][SLICE8(n6,  2, 2)] ^                                 \
                t[101][SLICE8(n6,  4, 2)] ^                                 \
                t[100][SLICE8(n6,  6, 2)] ^                                 \
                t[ 99][SLICE8(n7,  0, 2)] ^                                 \
                t[ 98][SLICE8(n7,  2, 2)] ^                                 \
                t[ 97][SLICE8(n7,  4, 2)] ^                                 \
                t[ 96][SLICE8(n7,  6, 2)] ^                                 \
                t[ 95][SLICE8(n8,  0, 2)] ^                                 \
                t[ 94][SLICE8(n8,  2, 2)] ^                                 \
                t[ 93][SLICE8(n8,  4, 2)] ^                                 \
                t[ 92][SLICE8(n8,  6, 2)] ^                                 \
                t[ 91][SLICE8(n9,  0, 2)] ^                                 \
                t[ 90][SLICE8(n9,  2, 2)] ^                                 \
                t[ 89][SLICE8(n9,  4, 2)] ^                                 \
                t[ 88][SLICE8(n9,  6, 2)] ^                                 \
                t[ 87][SLICE8(n10, 0, 2)] ^                                 \
                t[ 86][SLICE8(n10, 2, 2)] ^                                 \
                t[ 85][SLICE8(n10, 4, 2)] ^                                 \
                t[ 84][SLICE8(n10, 6, 2)] ^                                 \
                t[ 83][SLICE8(n11, 0, 2)] ^                                 \
                t[ 82][SLICE8(n11, 2, 2)] ^                                 \
                t[ 81][SLICE8(n11, 4, 2)] ^                                 \
                t[ 80][SLICE8(n11, 6, 2)] ^                                 \
                t[ 79][SLICE8(n12, 0, 2)] ^                                 \
                t[ 78][SLICE8(n12, 2, 2)] ^                                 \
                t[ 77][SLICE8(n12, 4, 2)] ^                                 \
                t[ 76][SLICE8(n12, 6, 2)] ^                                 \
                t[ 75][SLICE8(n13, 0, 2)] ^                                 \
                t[ 74][SLICE8(n13, 2, 2)] ^                                 \
                t[ 73][SLICE8(n13, 4, 2)] ^                                 \
                t[ 72][SLICE8(n13, 6, 2)] ^                                 \
                t[ 71][SLICE8(n14, 0, 2)] ^                                 \
                t[ 70][SLICE8(n14, 2, 2)] ^                                 \
                t[ 69][SLICE8(n14, 4, 2)] ^                                 \
                t[ 68][SLICE8(n14, 6, 2)] ^                                 \
                t[ 67][SLICE8(n15, 0, 2)] ^                                 \
                t[ 66][SLICE8(n15, 2, 2)] ^                                 \
                t[ 65][SLICE8(n15, 4, 2)] ^                                 \
                t[ 64][SLICE8(n15, 6, 2)] ^                                 \
                t[ 63][SLICE8(n16, 0, 2)] ^                                 \
                t[ 62][SLICE8(n16, 2, 2)] ^                                 \
                t[ 61][SLICE8(n16, 4, 2)] ^                                 \
                t[ 60][SLICE8(n16, 6, 2)] ^                                 \
                t[ 59][SLICE8(n17, 0, 2)] ^                                 \
                t[ 58][SLICE8(n17, 2, 2)] ^                                 \
                t[ 57][SLICE8(n17, 4, 2)] ^                                 \
                t[ 56][SLICE8(n17, 6, 2)] ^                                 \
                t[ 55][SLICE8(n18, 0, 2)] ^                                 \
                t[ 54][SLICE8(n18, 2, 2)] ^                                 \
                t[ 53][SLICE8(n18, 4, 2)] ^                                 \
                t[ 52][SLICE8(n18, 6, 2)] ^                                 \
                t[ 51][SLICE8(n19, 0, 2)] ^                                 \
                t[ 50][SLICE8(n19, 2, 2)] ^                                 \
                t[ 49][SLICE8(n19, 4, 2)] ^                                 \
                t[ 48][SLICE8(n19, 6, 2)] ^                                 \
                t[ 47][SLICE8(n20, 0, 2)] ^                                 \
                t[ 46][SLICE8(n20, 2, 2)] ^                                 \
                t[ 45][SLICE8(n20, 4, 2)] ^                                 \
                t[ 44][SLICE8(n20, 6, 2)] ^                                 \
                t[ 43][SLICE8(n21, 0, 2)] ^                                 \
                t[ 42][SLICE8(n21, 2, 2)] ^                                 \
                t[ 41][SLICE8(n21, 4, 2)] ^                                 \
                t[ 40][SLICE8(n21, 6, 2)] ^                                 \
                t[ 39][SLICE8(n22, 0, 2)] ^                                 \
                t[ 38][SLICE8(n22, 2, 2)] ^                                 \
                t[ 37][SLICE8(n22, 4, 2)] ^                                 \
                t[ 36][SLICE8(n22, 6, 2)] ^                                 \
                t[ 35][SLICE8(n23, 0, 2)] ^                                 \
                t[ 34][SLICE8(n23, 2, 2)] ^                                 \
                t[ 33][SLICE8(n23, 4, 2)] ^                                 \
                t[ 32][SLICE8(n23, 6, 2)] ^                                 \
                t[ 31][SLICE8(n24, 0, 2)] ^                                 \
                t[ 30][SLICE8(n24, 2, 2)] ^                                 \
                t[ 29][SLICE8(n24, 4, 2)] ^                                 \
                t[ 28][SLICE8(n24, 6, 2)] ^                                 \
                t[ 27][SLICE8(n25, 0, 2)] ^                                 \
                t[ 26][SLICE8(n25, 2, 2)] ^                                 \
                t[ 25][SLICE8(n25, 4, 2)] ^                                 \
                t[ 24][SLICE8(n25, 6, 2)] ^                                 \
                t[ 23][SLICE8(n26, 0, 2)] ^                                 \
                t[ 22][SLICE8(n26, 2, 2)] ^                                 \
                t[ 21][SLICE8(n26, 4, 2)] ^                                 \
                t[ 20][SLICE8(n26, 6, 2)] ^                                 \
                t[ 19][SLICE8(n27, 0, 2)] ^                                 \
                t[ 18][SLICE8(n27, 2, 2)] ^                                 \
                t[ 17][SLICE8(n27, 4, 2)] ^                                 \
                t[ 16][SLICE8(n27, 6, 2)] ^                                 \
                t[ 15][SLICE8(n28, 0, 2)] ^                                 \
                t[ 14][SLICE8(n28, 2, 2)] ^                                 \
                t[ 13][SLICE8(n28, 4, 2)] ^                                 \
                t[ 12][SLICE8(n28, 6, 2)] ^                                 \
                t[ 11][SLICE8(n29, 0, 2)] ^                                 \
                t[ 10][SLICE8(n29, 2, 2)] ^                                 \
                t[  9][SLICE8(n29, 4, 2)] ^                                 \
                t[  8][SLICE8(n29, 6, 2)] ^                                 \
                t[  7][SLICE8(n30, 0, 2)] ^                                 \
                t[  6][SLICE8(n30, 2, 2)] ^                                 \
                t[  5][SLICE8(n30, 4, 2)] ^                                 \
                t[  4][SLICE8(n30, 6, 2)] ^                                 \
                t[  3][SLICE8(n31, 0, 2)] ^                                 \
                t[  2][SLICE8(n31, 2, 2)] ^                                 \
                t[  1][SLICE8(n31, 4, 2)] ^                                 \
                t[  0][SLICE8(n31, 6, 2)];                                  \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[3][SLICE8(n, 0, 2)] ^                                     \
                t[2][SLICE8(n, 2, 2)] ^                                     \
                t[1][SLICE8(n, 4, 2)] ^                                     \
                t[0][SLICE8(n, 6, 2)];                                      \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BY32_DUO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY32_DUO */
