/**
 * @file _by_solo.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 */

#ifdef CRCEA_ENABLE_BY_SOLO

/*
 * Slicing by Single Solo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY_SOLO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE *t = (const CRCEA_TYPE *)table;

#define CRCEA_BY_SOLO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                        \
        state ^= INPUT(*IN);                                                \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
        state = SHIFT(state, 1) ^ t[SLICE(state, 0, 1)];                    \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY_SOLO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY_SOLO */

#ifdef CRCEA_ENABLE_BY1_SOLO

/*
 * Slicing by Octuple Solo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY1_SOLO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[2] = (const CRCEA_TYPE (*)[2])table;

#define CRCEA_BY1_SOLO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                        \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[7][SLICE8(n, 0, 1)] ^                                     \
                t[6][SLICE8(n, 1, 1)] ^                                     \
                t[5][SLICE8(n, 2, 1)] ^                                     \
                t[4][SLICE8(n, 3, 1)] ^                                     \
                t[3][SLICE8(n, 4, 1)] ^                                     \
                t[2][SLICE8(n, 5, 1)] ^                                     \
                t[1][SLICE8(n, 6, 1)] ^                                     \
                t[0][SLICE8(n, 7, 1)];                                      \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[7][SLICE8(n, 0, 1)] ^                                     \
                t[6][SLICE8(n, 1, 1)] ^                                     \
                t[5][SLICE8(n, 2, 1)] ^                                     \
                t[4][SLICE8(n, 3, 1)] ^                                     \
                t[3][SLICE8(n, 4, 1)] ^                                     \
                t[2][SLICE8(n, 5, 1)] ^                                     \
                t[1][SLICE8(n, 6, 1)] ^                                     \
                t[0][SLICE8(n, 7, 1)];                                      \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY1_SOLO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY1_SOLO */

#ifdef CRCEA_ENABLE_BY2_SOLO

/*
 * Slicing by Sexdecuple Solo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY2_SOLO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[2] = (const CRCEA_TYPE (*)[2])table;

#define CRCEA_BY2_SOLO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 2);                                        \
        const uint8_t n0 = (uint8_t)IN[0] ^ SLICE(state, 0, 8);             \
        const uint8_t n1 = (uint8_t)IN[1] ^ SLICE(state, 8, 8);             \
        state = SHIFT(state, 16) ^                                          \
                t[15][SLICE8(n0, 0, 1)] ^                                   \
                t[14][SLICE8(n0, 1, 1)] ^                                   \
                t[13][SLICE8(n0, 2, 1)] ^                                   \
                t[12][SLICE8(n0, 3, 1)] ^                                   \
                t[11][SLICE8(n0, 4, 1)] ^                                   \
                t[10][SLICE8(n0, 5, 1)] ^                                   \
                t[ 9][SLICE8(n0, 6, 1)] ^                                   \
                t[ 8][SLICE8(n0, 7, 1)] ^                                   \
                t[ 7][SLICE8(n1, 0, 1)] ^                                   \
                t[ 6][SLICE8(n1, 1, 1)] ^                                   \
                t[ 5][SLICE8(n1, 2, 1)] ^                                   \
                t[ 4][SLICE8(n1, 3, 1)] ^                                   \
                t[ 3][SLICE8(n1, 4, 1)] ^                                   \
                t[ 2][SLICE8(n1, 5, 1)] ^                                   \
                t[ 1][SLICE8(n1, 6, 1)] ^                                   \
                t[ 0][SLICE8(n1, 7, 1)];                                    \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[7][SLICE8(n, 0, 1)] ^                                     \
                t[6][SLICE8(n, 1, 1)] ^                                     \
                t[5][SLICE8(n, 2, 1)] ^                                     \
                t[4][SLICE8(n, 3, 1)] ^                                     \
                t[3][SLICE8(n, 4, 1)] ^                                     \
                t[2][SLICE8(n, 5, 1)] ^                                     \
                t[1][SLICE8(n, 6, 1)] ^                                     \
                t[0][SLICE8(n, 7, 1)];                                      \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY2_SOLO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY2_SOLO */

#ifdef CRCEA_ENABLE_BY4_SOLO

/*
 * Slicing by Duotriguple Solo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY4_SOLO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[2] = (const CRCEA_TYPE (*)[2])table;

#define CRCEA_BY4_SOLO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 4);                                        \
        const uint8_t n0 = (uint8_t)IN[0] ^ SLICE(state,  0, 8);            \
        const uint8_t n1 = (uint8_t)IN[1] ^ SLICE(state,  8, 8);            \
        const uint8_t n2 = (uint8_t)IN[2] ^ SLICE(state, 16, 8);            \
        const uint8_t n3 = (uint8_t)IN[3] ^ SLICE(state, 24, 8);            \
        state = SHIFT(state, 32) ^                                          \
                t[31][SLICE8(n0, 0, 1)] ^                                   \
                t[30][SLICE8(n0, 1, 1)] ^                                   \
                t[29][SLICE8(n0, 2, 1)] ^                                   \
                t[28][SLICE8(n0, 3, 1)] ^                                   \
                t[27][SLICE8(n0, 4, 1)] ^                                   \
                t[26][SLICE8(n0, 5, 1)] ^                                   \
                t[25][SLICE8(n0, 6, 1)] ^                                   \
                t[24][SLICE8(n0, 7, 1)] ^                                   \
                t[23][SLICE8(n1, 0, 1)] ^                                   \
                t[22][SLICE8(n1, 1, 1)] ^                                   \
                t[21][SLICE8(n1, 2, 1)] ^                                   \
                t[20][SLICE8(n1, 3, 1)] ^                                   \
                t[19][SLICE8(n1, 4, 1)] ^                                   \
                t[18][SLICE8(n1, 5, 1)] ^                                   \
                t[17][SLICE8(n1, 6, 1)] ^                                   \
                t[16][SLICE8(n1, 7, 1)] ^                                   \
                t[15][SLICE8(n2, 0, 1)] ^                                   \
                t[14][SLICE8(n2, 1, 1)] ^                                   \
                t[13][SLICE8(n2, 2, 1)] ^                                   \
                t[12][SLICE8(n2, 3, 1)] ^                                   \
                t[11][SLICE8(n2, 4, 1)] ^                                   \
                t[10][SLICE8(n2, 5, 1)] ^                                   \
                t[ 9][SLICE8(n2, 6, 1)] ^                                   \
                t[ 8][SLICE8(n2, 7, 1)] ^                                   \
                t[ 7][SLICE8(n3, 0, 1)] ^                                   \
                t[ 6][SLICE8(n3, 1, 1)] ^                                   \
                t[ 5][SLICE8(n3, 2, 1)] ^                                   \
                t[ 4][SLICE8(n3, 3, 1)] ^                                   \
                t[ 3][SLICE8(n3, 4, 1)] ^                                   \
                t[ 2][SLICE8(n3, 5, 1)] ^                                   \
                t[ 1][SLICE8(n3, 6, 1)] ^                                   \
                t[ 0][SLICE8(n3, 7, 1)];                                    \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[7][SLICE8(n, 0, 1)] ^                                     \
                t[6][SLICE8(n, 1, 1)] ^                                     \
                t[5][SLICE8(n, 2, 1)] ^                                     \
                t[4][SLICE8(n, 3, 1)] ^                                     \
                t[3][SLICE8(n, 4, 1)] ^                                     \
                t[2][SLICE8(n, 5, 1)] ^                                     \
                t[1][SLICE8(n, 6, 1)] ^                                     \
                t[0][SLICE8(n, 7, 1)];                                      \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY4_SOLO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY4_SOLO */

#ifdef CRCEA_ENABLE_BY8_SOLO

/*
 * Slicing by Quattuorsexaguple Solo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY8_SOLO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[2] = (const CRCEA_TYPE (*)[2])table;

#define CRCEA_BY8_SOLO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
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
                t[63][SLICE8(n0, 0, 1)] ^                                   \
                t[62][SLICE8(n0, 1, 1)] ^                                   \
                t[61][SLICE8(n0, 2, 1)] ^                                   \
                t[60][SLICE8(n0, 3, 1)] ^                                   \
                t[59][SLICE8(n0, 4, 1)] ^                                   \
                t[58][SLICE8(n0, 5, 1)] ^                                   \
                t[57][SLICE8(n0, 6, 1)] ^                                   \
                t[56][SLICE8(n0, 7, 1)] ^                                   \
                t[55][SLICE8(n1, 0, 1)] ^                                   \
                t[54][SLICE8(n1, 1, 1)] ^                                   \
                t[53][SLICE8(n1, 2, 1)] ^                                   \
                t[52][SLICE8(n1, 3, 1)] ^                                   \
                t[51][SLICE8(n1, 4, 1)] ^                                   \
                t[50][SLICE8(n1, 5, 1)] ^                                   \
                t[49][SLICE8(n1, 6, 1)] ^                                   \
                t[48][SLICE8(n1, 7, 1)] ^                                   \
                t[47][SLICE8(n2, 0, 1)] ^                                   \
                t[46][SLICE8(n2, 1, 1)] ^                                   \
                t[45][SLICE8(n2, 2, 1)] ^                                   \
                t[44][SLICE8(n2, 3, 1)] ^                                   \
                t[43][SLICE8(n2, 4, 1)] ^                                   \
                t[42][SLICE8(n2, 5, 1)] ^                                   \
                t[41][SLICE8(n2, 6, 1)] ^                                   \
                t[40][SLICE8(n2, 7, 1)] ^                                   \
                t[39][SLICE8(n3, 0, 1)] ^                                   \
                t[38][SLICE8(n3, 1, 1)] ^                                   \
                t[37][SLICE8(n3, 2, 1)] ^                                   \
                t[36][SLICE8(n3, 3, 1)] ^                                   \
                t[35][SLICE8(n3, 4, 1)] ^                                   \
                t[34][SLICE8(n3, 5, 1)] ^                                   \
                t[33][SLICE8(n3, 6, 1)] ^                                   \
                t[32][SLICE8(n3, 7, 1)] ^                                   \
                t[31][SLICE8(n4, 0, 1)] ^                                   \
                t[30][SLICE8(n4, 1, 1)] ^                                   \
                t[29][SLICE8(n4, 2, 1)] ^                                   \
                t[28][SLICE8(n4, 3, 1)] ^                                   \
                t[27][SLICE8(n4, 4, 1)] ^                                   \
                t[26][SLICE8(n4, 5, 1)] ^                                   \
                t[25][SLICE8(n4, 6, 1)] ^                                   \
                t[24][SLICE8(n4, 7, 1)] ^                                   \
                t[23][SLICE8(n5, 0, 1)] ^                                   \
                t[22][SLICE8(n5, 1, 1)] ^                                   \
                t[21][SLICE8(n5, 2, 1)] ^                                   \
                t[20][SLICE8(n5, 3, 1)] ^                                   \
                t[19][SLICE8(n5, 4, 1)] ^                                   \
                t[18][SLICE8(n5, 5, 1)] ^                                   \
                t[17][SLICE8(n5, 6, 1)] ^                                   \
                t[16][SLICE8(n5, 7, 1)] ^                                   \
                t[15][SLICE8(n6, 0, 1)] ^                                   \
                t[14][SLICE8(n6, 1, 1)] ^                                   \
                t[13][SLICE8(n6, 2, 1)] ^                                   \
                t[12][SLICE8(n6, 3, 1)] ^                                   \
                t[11][SLICE8(n6, 4, 1)] ^                                   \
                t[10][SLICE8(n6, 5, 1)] ^                                   \
                t[ 9][SLICE8(n6, 6, 1)] ^                                   \
                t[ 8][SLICE8(n6, 7, 1)] ^                                   \
                t[ 7][SLICE8(n7, 0, 1)] ^                                   \
                t[ 6][SLICE8(n7, 1, 1)] ^                                   \
                t[ 5][SLICE8(n7, 2, 1)] ^                                   \
                t[ 4][SLICE8(n7, 3, 1)] ^                                   \
                t[ 3][SLICE8(n7, 4, 1)] ^                                   \
                t[ 2][SLICE8(n7, 5, 1)] ^                                   \
                t[ 1][SLICE8(n7, 6, 1)] ^                                   \
                t[ 0][SLICE8(n7, 7, 1)];                                    \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[7][SLICE8(n, 0, 1)] ^                                     \
                t[6][SLICE8(n, 1, 1)] ^                                     \
                t[5][SLICE8(n, 2, 1)] ^                                     \
                t[4][SLICE8(n, 3, 1)] ^                                     \
                t[3][SLICE8(n, 4, 1)] ^                                     \
                t[2][SLICE8(n, 5, 1)] ^                                     \
                t[1][SLICE8(n, 6, 1)] ^                                     \
                t[0][SLICE8(n, 7, 1)];                                      \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY8_SOLO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY8_SOLO */

#ifdef CRCEA_ENABLE_BY16_SOLO

/*
 * Slicing by Octovigcentuple Solo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY16_SOLO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[2] = (const CRCEA_TYPE (*)[2])table;

#define CRCEA_BY16_SOLO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
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
                t[127][SLICE8(n0,  0, 1)] ^                                 \
                t[126][SLICE8(n0,  1, 1)] ^                                 \
                t[125][SLICE8(n0,  2, 1)] ^                                 \
                t[124][SLICE8(n0,  3, 1)] ^                                 \
                t[123][SLICE8(n0,  4, 1)] ^                                 \
                t[122][SLICE8(n0,  5, 1)] ^                                 \
                t[121][SLICE8(n0,  6, 1)] ^                                 \
                t[120][SLICE8(n0,  7, 1)] ^                                 \
                t[119][SLICE8(n1,  0, 1)] ^                                 \
                t[118][SLICE8(n1,  1, 1)] ^                                 \
                t[117][SLICE8(n1,  2, 1)] ^                                 \
                t[116][SLICE8(n1,  3, 1)] ^                                 \
                t[115][SLICE8(n1,  4, 1)] ^                                 \
                t[114][SLICE8(n1,  5, 1)] ^                                 \
                t[113][SLICE8(n1,  6, 1)] ^                                 \
                t[112][SLICE8(n1,  7, 1)] ^                                 \
                t[111][SLICE8(n2,  0, 1)] ^                                 \
                t[110][SLICE8(n2,  1, 1)] ^                                 \
                t[109][SLICE8(n2,  2, 1)] ^                                 \
                t[108][SLICE8(n2,  3, 1)] ^                                 \
                t[107][SLICE8(n2,  4, 1)] ^                                 \
                t[106][SLICE8(n2,  5, 1)] ^                                 \
                t[105][SLICE8(n2,  6, 1)] ^                                 \
                t[104][SLICE8(n2,  7, 1)] ^                                 \
                t[103][SLICE8(n3,  0, 1)] ^                                 \
                t[102][SLICE8(n3,  1, 1)] ^                                 \
                t[101][SLICE8(n3,  2, 1)] ^                                 \
                t[100][SLICE8(n3,  3, 1)] ^                                 \
                t[ 99][SLICE8(n3,  4, 1)] ^                                 \
                t[ 98][SLICE8(n3,  5, 1)] ^                                 \
                t[ 97][SLICE8(n3,  6, 1)] ^                                 \
                t[ 96][SLICE8(n3,  7, 1)] ^                                 \
                t[ 95][SLICE8(n4,  0, 1)] ^                                 \
                t[ 94][SLICE8(n4,  1, 1)] ^                                 \
                t[ 93][SLICE8(n4,  2, 1)] ^                                 \
                t[ 92][SLICE8(n4,  3, 1)] ^                                 \
                t[ 91][SLICE8(n4,  4, 1)] ^                                 \
                t[ 90][SLICE8(n4,  5, 1)] ^                                 \
                t[ 89][SLICE8(n4,  6, 1)] ^                                 \
                t[ 88][SLICE8(n4,  7, 1)] ^                                 \
                t[ 87][SLICE8(n5,  0, 1)] ^                                 \
                t[ 86][SLICE8(n5,  1, 1)] ^                                 \
                t[ 85][SLICE8(n5,  2, 1)] ^                                 \
                t[ 84][SLICE8(n5,  3, 1)] ^                                 \
                t[ 83][SLICE8(n5,  4, 1)] ^                                 \
                t[ 82][SLICE8(n5,  5, 1)] ^                                 \
                t[ 81][SLICE8(n5,  6, 1)] ^                                 \
                t[ 80][SLICE8(n5,  7, 1)] ^                                 \
                t[ 79][SLICE8(n6,  0, 1)] ^                                 \
                t[ 78][SLICE8(n6,  1, 1)] ^                                 \
                t[ 77][SLICE8(n6,  2, 1)] ^                                 \
                t[ 76][SLICE8(n6,  3, 1)] ^                                 \
                t[ 75][SLICE8(n6,  4, 1)] ^                                 \
                t[ 74][SLICE8(n6,  5, 1)] ^                                 \
                t[ 73][SLICE8(n6,  6, 1)] ^                                 \
                t[ 72][SLICE8(n6,  7, 1)] ^                                 \
                t[ 71][SLICE8(n7,  0, 1)] ^                                 \
                t[ 70][SLICE8(n7,  1, 1)] ^                                 \
                t[ 69][SLICE8(n7,  2, 1)] ^                                 \
                t[ 68][SLICE8(n7,  3, 1)] ^                                 \
                t[ 67][SLICE8(n7,  4, 1)] ^                                 \
                t[ 66][SLICE8(n7,  5, 1)] ^                                 \
                t[ 65][SLICE8(n7,  6, 1)] ^                                 \
                t[ 64][SLICE8(n7,  7, 1)] ^                                 \
                t[ 63][SLICE8(n8,  0, 1)] ^                                 \
                t[ 62][SLICE8(n8,  1, 1)] ^                                 \
                t[ 61][SLICE8(n8,  2, 1)] ^                                 \
                t[ 60][SLICE8(n8,  3, 1)] ^                                 \
                t[ 59][SLICE8(n8,  4, 1)] ^                                 \
                t[ 58][SLICE8(n8,  5, 1)] ^                                 \
                t[ 57][SLICE8(n8,  6, 1)] ^                                 \
                t[ 56][SLICE8(n8,  7, 1)] ^                                 \
                t[ 55][SLICE8(n9,  0, 1)] ^                                 \
                t[ 54][SLICE8(n9,  1, 1)] ^                                 \
                t[ 53][SLICE8(n9,  2, 1)] ^                                 \
                t[ 52][SLICE8(n9,  3, 1)] ^                                 \
                t[ 51][SLICE8(n9,  4, 1)] ^                                 \
                t[ 50][SLICE8(n9,  5, 1)] ^                                 \
                t[ 49][SLICE8(n9,  6, 1)] ^                                 \
                t[ 48][SLICE8(n9,  7, 1)] ^                                 \
                t[ 47][SLICE8(n10, 0, 1)] ^                                 \
                t[ 46][SLICE8(n10, 1, 1)] ^                                 \
                t[ 45][SLICE8(n10, 2, 1)] ^                                 \
                t[ 44][SLICE8(n10, 3, 1)] ^                                 \
                t[ 43][SLICE8(n10, 4, 1)] ^                                 \
                t[ 42][SLICE8(n10, 5, 1)] ^                                 \
                t[ 41][SLICE8(n10, 6, 1)] ^                                 \
                t[ 40][SLICE8(n10, 7, 1)] ^                                 \
                t[ 39][SLICE8(n11, 0, 1)] ^                                 \
                t[ 38][SLICE8(n11, 1, 1)] ^                                 \
                t[ 37][SLICE8(n11, 2, 1)] ^                                 \
                t[ 36][SLICE8(n11, 3, 1)] ^                                 \
                t[ 35][SLICE8(n11, 4, 1)] ^                                 \
                t[ 34][SLICE8(n11, 5, 1)] ^                                 \
                t[ 33][SLICE8(n11, 6, 1)] ^                                 \
                t[ 32][SLICE8(n11, 7, 1)] ^                                 \
                t[ 31][SLICE8(n12, 0, 1)] ^                                 \
                t[ 30][SLICE8(n12, 1, 1)] ^                                 \
                t[ 29][SLICE8(n12, 2, 1)] ^                                 \
                t[ 28][SLICE8(n12, 3, 1)] ^                                 \
                t[ 27][SLICE8(n12, 4, 1)] ^                                 \
                t[ 26][SLICE8(n12, 5, 1)] ^                                 \
                t[ 25][SLICE8(n12, 6, 1)] ^                                 \
                t[ 24][SLICE8(n12, 7, 1)] ^                                 \
                t[ 23][SLICE8(n13, 0, 1)] ^                                 \
                t[ 22][SLICE8(n13, 1, 1)] ^                                 \
                t[ 21][SLICE8(n13, 2, 1)] ^                                 \
                t[ 20][SLICE8(n13, 3, 1)] ^                                 \
                t[ 19][SLICE8(n13, 4, 1)] ^                                 \
                t[ 18][SLICE8(n13, 5, 1)] ^                                 \
                t[ 17][SLICE8(n13, 6, 1)] ^                                 \
                t[ 16][SLICE8(n13, 7, 1)] ^                                 \
                t[ 15][SLICE8(n14, 0, 1)] ^                                 \
                t[ 14][SLICE8(n14, 1, 1)] ^                                 \
                t[ 13][SLICE8(n14, 2, 1)] ^                                 \
                t[ 12][SLICE8(n14, 3, 1)] ^                                 \
                t[ 11][SLICE8(n14, 4, 1)] ^                                 \
                t[ 10][SLICE8(n14, 5, 1)] ^                                 \
                t[  9][SLICE8(n14, 6, 1)] ^                                 \
                t[  8][SLICE8(n14, 7, 1)] ^                                 \
                t[  7][SLICE8(n15, 0, 1)] ^                                 \
                t[  6][SLICE8(n15, 1, 1)] ^                                 \
                t[  5][SLICE8(n15, 2, 1)] ^                                 \
                t[  4][SLICE8(n15, 3, 1)] ^                                 \
                t[  3][SLICE8(n15, 4, 1)] ^                                 \
                t[  2][SLICE8(n15, 5, 1)] ^                                 \
                t[  1][SLICE8(n15, 6, 1)] ^                                 \
                t[  0][SLICE8(n15, 7, 1)];                                  \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[7][SLICE8(n, 0, 1)] ^                                     \
                t[6][SLICE8(n, 1, 1)] ^                                     \
                t[5][SLICE8(n, 2, 1)] ^                                     \
                t[4][SLICE8(n, 3, 1)] ^                                     \
                t[3][SLICE8(n, 4, 1)] ^                                     \
                t[2][SLICE8(n, 5, 1)] ^                                     \
                t[1][SLICE8(n, 6, 1)] ^                                     \
                t[0][SLICE8(n, 7, 1)];                                      \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY16_SOLO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY16_SOLO */

#ifdef CRCEA_ENABLE_BY32_SOLO

/*
 * Slicing by Sexquinquagducentuple Solo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BY32_SOLO(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[2] = (const CRCEA_TYPE (*)[2])table;

#define CRCEA_BY32_SOLO_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
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
                t[255][SLICE8(n0,  0, 1)] ^                                 \
                t[254][SLICE8(n0,  1, 1)] ^                                 \
                t[253][SLICE8(n0,  2, 1)] ^                                 \
                t[252][SLICE8(n0,  3, 1)] ^                                 \
                t[251][SLICE8(n0,  4, 1)] ^                                 \
                t[250][SLICE8(n0,  5, 1)] ^                                 \
                t[249][SLICE8(n0,  6, 1)] ^                                 \
                t[248][SLICE8(n0,  7, 1)] ^                                 \
                t[247][SLICE8(n1,  0, 1)] ^                                 \
                t[246][SLICE8(n1,  1, 1)] ^                                 \
                t[245][SLICE8(n1,  2, 1)] ^                                 \
                t[244][SLICE8(n1,  3, 1)] ^                                 \
                t[243][SLICE8(n1,  4, 1)] ^                                 \
                t[242][SLICE8(n1,  5, 1)] ^                                 \
                t[241][SLICE8(n1,  6, 1)] ^                                 \
                t[240][SLICE8(n1,  7, 1)] ^                                 \
                t[239][SLICE8(n2,  0, 1)] ^                                 \
                t[238][SLICE8(n2,  1, 1)] ^                                 \
                t[237][SLICE8(n2,  2, 1)] ^                                 \
                t[236][SLICE8(n2,  3, 1)] ^                                 \
                t[235][SLICE8(n2,  4, 1)] ^                                 \
                t[234][SLICE8(n2,  5, 1)] ^                                 \
                t[233][SLICE8(n2,  6, 1)] ^                                 \
                t[232][SLICE8(n2,  7, 1)] ^                                 \
                t[231][SLICE8(n3,  0, 1)] ^                                 \
                t[230][SLICE8(n3,  1, 1)] ^                                 \
                t[229][SLICE8(n3,  2, 1)] ^                                 \
                t[228][SLICE8(n3,  3, 1)] ^                                 \
                t[227][SLICE8(n3,  4, 1)] ^                                 \
                t[226][SLICE8(n3,  5, 1)] ^                                 \
                t[225][SLICE8(n3,  6, 1)] ^                                 \
                t[224][SLICE8(n3,  7, 1)] ^                                 \
                t[223][SLICE8(n4,  0, 1)] ^                                 \
                t[222][SLICE8(n4,  1, 1)] ^                                 \
                t[221][SLICE8(n4,  2, 1)] ^                                 \
                t[220][SLICE8(n4,  3, 1)] ^                                 \
                t[219][SLICE8(n4,  4, 1)] ^                                 \
                t[218][SLICE8(n4,  5, 1)] ^                                 \
                t[217][SLICE8(n4,  6, 1)] ^                                 \
                t[216][SLICE8(n4,  7, 1)] ^                                 \
                t[215][SLICE8(n5,  0, 1)] ^                                 \
                t[214][SLICE8(n5,  1, 1)] ^                                 \
                t[213][SLICE8(n5,  2, 1)] ^                                 \
                t[212][SLICE8(n5,  3, 1)] ^                                 \
                t[211][SLICE8(n5,  4, 1)] ^                                 \
                t[210][SLICE8(n5,  5, 1)] ^                                 \
                t[209][SLICE8(n5,  6, 1)] ^                                 \
                t[208][SLICE8(n5,  7, 1)] ^                                 \
                t[207][SLICE8(n6,  0, 1)] ^                                 \
                t[206][SLICE8(n6,  1, 1)] ^                                 \
                t[205][SLICE8(n6,  2, 1)] ^                                 \
                t[204][SLICE8(n6,  3, 1)] ^                                 \
                t[203][SLICE8(n6,  4, 1)] ^                                 \
                t[202][SLICE8(n6,  5, 1)] ^                                 \
                t[201][SLICE8(n6,  6, 1)] ^                                 \
                t[200][SLICE8(n6,  7, 1)] ^                                 \
                t[199][SLICE8(n7,  0, 1)] ^                                 \
                t[198][SLICE8(n7,  1, 1)] ^                                 \
                t[197][SLICE8(n7,  2, 1)] ^                                 \
                t[196][SLICE8(n7,  3, 1)] ^                                 \
                t[195][SLICE8(n7,  4, 1)] ^                                 \
                t[194][SLICE8(n7,  5, 1)] ^                                 \
                t[193][SLICE8(n7,  6, 1)] ^                                 \
                t[192][SLICE8(n7,  7, 1)] ^                                 \
                t[191][SLICE8(n8,  0, 1)] ^                                 \
                t[190][SLICE8(n8,  1, 1)] ^                                 \
                t[189][SLICE8(n8,  2, 1)] ^                                 \
                t[188][SLICE8(n8,  3, 1)] ^                                 \
                t[187][SLICE8(n8,  4, 1)] ^                                 \
                t[186][SLICE8(n8,  5, 1)] ^                                 \
                t[185][SLICE8(n8,  6, 1)] ^                                 \
                t[184][SLICE8(n8,  7, 1)] ^                                 \
                t[183][SLICE8(n9,  0, 1)] ^                                 \
                t[182][SLICE8(n9,  1, 1)] ^                                 \
                t[181][SLICE8(n9,  2, 1)] ^                                 \
                t[180][SLICE8(n9,  3, 1)] ^                                 \
                t[179][SLICE8(n9,  4, 1)] ^                                 \
                t[178][SLICE8(n9,  5, 1)] ^                                 \
                t[177][SLICE8(n9,  6, 1)] ^                                 \
                t[176][SLICE8(n9,  7, 1)] ^                                 \
                t[175][SLICE8(n10, 0, 1)] ^                                 \
                t[174][SLICE8(n10, 1, 1)] ^                                 \
                t[173][SLICE8(n10, 2, 1)] ^                                 \
                t[172][SLICE8(n10, 3, 1)] ^                                 \
                t[171][SLICE8(n10, 4, 1)] ^                                 \
                t[170][SLICE8(n10, 5, 1)] ^                                 \
                t[169][SLICE8(n10, 6, 1)] ^                                 \
                t[168][SLICE8(n10, 7, 1)] ^                                 \
                t[167][SLICE8(n11, 0, 1)] ^                                 \
                t[166][SLICE8(n11, 1, 1)] ^                                 \
                t[165][SLICE8(n11, 2, 1)] ^                                 \
                t[164][SLICE8(n11, 3, 1)] ^                                 \
                t[163][SLICE8(n11, 4, 1)] ^                                 \
                t[162][SLICE8(n11, 5, 1)] ^                                 \
                t[161][SLICE8(n11, 6, 1)] ^                                 \
                t[160][SLICE8(n11, 7, 1)] ^                                 \
                t[159][SLICE8(n12, 0, 1)] ^                                 \
                t[158][SLICE8(n12, 1, 1)] ^                                 \
                t[157][SLICE8(n12, 2, 1)] ^                                 \
                t[156][SLICE8(n12, 3, 1)] ^                                 \
                t[155][SLICE8(n12, 4, 1)] ^                                 \
                t[154][SLICE8(n12, 5, 1)] ^                                 \
                t[153][SLICE8(n12, 6, 1)] ^                                 \
                t[152][SLICE8(n12, 7, 1)] ^                                 \
                t[151][SLICE8(n13, 0, 1)] ^                                 \
                t[150][SLICE8(n13, 1, 1)] ^                                 \
                t[149][SLICE8(n13, 2, 1)] ^                                 \
                t[148][SLICE8(n13, 3, 1)] ^                                 \
                t[147][SLICE8(n13, 4, 1)] ^                                 \
                t[146][SLICE8(n13, 5, 1)] ^                                 \
                t[145][SLICE8(n13, 6, 1)] ^                                 \
                t[144][SLICE8(n13, 7, 1)] ^                                 \
                t[143][SLICE8(n14, 0, 1)] ^                                 \
                t[142][SLICE8(n14, 1, 1)] ^                                 \
                t[141][SLICE8(n14, 2, 1)] ^                                 \
                t[140][SLICE8(n14, 3, 1)] ^                                 \
                t[139][SLICE8(n14, 4, 1)] ^                                 \
                t[138][SLICE8(n14, 5, 1)] ^                                 \
                t[137][SLICE8(n14, 6, 1)] ^                                 \
                t[136][SLICE8(n14, 7, 1)] ^                                 \
                t[135][SLICE8(n15, 0, 1)] ^                                 \
                t[134][SLICE8(n15, 1, 1)] ^                                 \
                t[133][SLICE8(n15, 2, 1)] ^                                 \
                t[132][SLICE8(n15, 3, 1)] ^                                 \
                t[131][SLICE8(n15, 4, 1)] ^                                 \
                t[130][SLICE8(n15, 5, 1)] ^                                 \
                t[129][SLICE8(n15, 6, 1)] ^                                 \
                t[128][SLICE8(n15, 7, 1)] ^                                 \
                t[127][SLICE8(n16, 0, 1)] ^                                 \
                t[126][SLICE8(n16, 1, 1)] ^                                 \
                t[125][SLICE8(n16, 2, 1)] ^                                 \
                t[124][SLICE8(n16, 3, 1)] ^                                 \
                t[123][SLICE8(n16, 4, 1)] ^                                 \
                t[122][SLICE8(n16, 5, 1)] ^                                 \
                t[121][SLICE8(n16, 6, 1)] ^                                 \
                t[120][SLICE8(n16, 7, 1)] ^                                 \
                t[119][SLICE8(n17, 0, 1)] ^                                 \
                t[118][SLICE8(n17, 1, 1)] ^                                 \
                t[117][SLICE8(n17, 2, 1)] ^                                 \
                t[116][SLICE8(n17, 3, 1)] ^                                 \
                t[115][SLICE8(n17, 4, 1)] ^                                 \
                t[114][SLICE8(n17, 5, 1)] ^                                 \
                t[113][SLICE8(n17, 6, 1)] ^                                 \
                t[112][SLICE8(n17, 7, 1)] ^                                 \
                t[111][SLICE8(n18, 0, 1)] ^                                 \
                t[110][SLICE8(n18, 1, 1)] ^                                 \
                t[109][SLICE8(n18, 2, 1)] ^                                 \
                t[108][SLICE8(n18, 3, 1)] ^                                 \
                t[107][SLICE8(n18, 4, 1)] ^                                 \
                t[106][SLICE8(n18, 5, 1)] ^                                 \
                t[105][SLICE8(n18, 6, 1)] ^                                 \
                t[104][SLICE8(n18, 7, 1)] ^                                 \
                t[103][SLICE8(n19, 0, 1)] ^                                 \
                t[102][SLICE8(n19, 1, 1)] ^                                 \
                t[101][SLICE8(n19, 2, 1)] ^                                 \
                t[100][SLICE8(n19, 3, 1)] ^                                 \
                t[ 99][SLICE8(n19, 4, 1)] ^                                 \
                t[ 98][SLICE8(n19, 5, 1)] ^                                 \
                t[ 97][SLICE8(n19, 6, 1)] ^                                 \
                t[ 96][SLICE8(n19, 7, 1)] ^                                 \
                t[ 95][SLICE8(n20, 0, 1)] ^                                 \
                t[ 94][SLICE8(n20, 1, 1)] ^                                 \
                t[ 93][SLICE8(n20, 2, 1)] ^                                 \
                t[ 92][SLICE8(n20, 3, 1)] ^                                 \
                t[ 91][SLICE8(n20, 4, 1)] ^                                 \
                t[ 90][SLICE8(n20, 5, 1)] ^                                 \
                t[ 89][SLICE8(n20, 6, 1)] ^                                 \
                t[ 88][SLICE8(n20, 7, 1)] ^                                 \
                t[ 87][SLICE8(n21, 0, 1)] ^                                 \
                t[ 86][SLICE8(n21, 1, 1)] ^                                 \
                t[ 85][SLICE8(n21, 2, 1)] ^                                 \
                t[ 84][SLICE8(n21, 3, 1)] ^                                 \
                t[ 83][SLICE8(n21, 4, 1)] ^                                 \
                t[ 82][SLICE8(n21, 5, 1)] ^                                 \
                t[ 81][SLICE8(n21, 6, 1)] ^                                 \
                t[ 80][SLICE8(n21, 7, 1)] ^                                 \
                t[ 79][SLICE8(n22, 0, 1)] ^                                 \
                t[ 78][SLICE8(n22, 1, 1)] ^                                 \
                t[ 77][SLICE8(n22, 2, 1)] ^                                 \
                t[ 76][SLICE8(n22, 3, 1)] ^                                 \
                t[ 75][SLICE8(n22, 4, 1)] ^                                 \
                t[ 74][SLICE8(n22, 5, 1)] ^                                 \
                t[ 73][SLICE8(n22, 6, 1)] ^                                 \
                t[ 72][SLICE8(n22, 7, 1)] ^                                 \
                t[ 71][SLICE8(n23, 0, 1)] ^                                 \
                t[ 70][SLICE8(n23, 1, 1)] ^                                 \
                t[ 69][SLICE8(n23, 2, 1)] ^                                 \
                t[ 68][SLICE8(n23, 3, 1)] ^                                 \
                t[ 67][SLICE8(n23, 4, 1)] ^                                 \
                t[ 66][SLICE8(n23, 5, 1)] ^                                 \
                t[ 65][SLICE8(n23, 6, 1)] ^                                 \
                t[ 64][SLICE8(n23, 7, 1)] ^                                 \
                t[ 63][SLICE8(n24, 0, 1)] ^                                 \
                t[ 62][SLICE8(n24, 1, 1)] ^                                 \
                t[ 61][SLICE8(n24, 2, 1)] ^                                 \
                t[ 60][SLICE8(n24, 3, 1)] ^                                 \
                t[ 59][SLICE8(n24, 4, 1)] ^                                 \
                t[ 58][SLICE8(n24, 5, 1)] ^                                 \
                t[ 57][SLICE8(n24, 6, 1)] ^                                 \
                t[ 56][SLICE8(n24, 7, 1)] ^                                 \
                t[ 55][SLICE8(n25, 0, 1)] ^                                 \
                t[ 54][SLICE8(n25, 1, 1)] ^                                 \
                t[ 53][SLICE8(n25, 2, 1)] ^                                 \
                t[ 52][SLICE8(n25, 3, 1)] ^                                 \
                t[ 51][SLICE8(n25, 4, 1)] ^                                 \
                t[ 50][SLICE8(n25, 5, 1)] ^                                 \
                t[ 49][SLICE8(n25, 6, 1)] ^                                 \
                t[ 48][SLICE8(n25, 7, 1)] ^                                 \
                t[ 47][SLICE8(n26, 0, 1)] ^                                 \
                t[ 46][SLICE8(n26, 1, 1)] ^                                 \
                t[ 45][SLICE8(n26, 2, 1)] ^                                 \
                t[ 44][SLICE8(n26, 3, 1)] ^                                 \
                t[ 43][SLICE8(n26, 4, 1)] ^                                 \
                t[ 42][SLICE8(n26, 5, 1)] ^                                 \
                t[ 41][SLICE8(n26, 6, 1)] ^                                 \
                t[ 40][SLICE8(n26, 7, 1)] ^                                 \
                t[ 39][SLICE8(n27, 0, 1)] ^                                 \
                t[ 38][SLICE8(n27, 1, 1)] ^                                 \
                t[ 37][SLICE8(n27, 2, 1)] ^                                 \
                t[ 36][SLICE8(n27, 3, 1)] ^                                 \
                t[ 35][SLICE8(n27, 4, 1)] ^                                 \
                t[ 34][SLICE8(n27, 5, 1)] ^                                 \
                t[ 33][SLICE8(n27, 6, 1)] ^                                 \
                t[ 32][SLICE8(n27, 7, 1)] ^                                 \
                t[ 31][SLICE8(n28, 0, 1)] ^                                 \
                t[ 30][SLICE8(n28, 1, 1)] ^                                 \
                t[ 29][SLICE8(n28, 2, 1)] ^                                 \
                t[ 28][SLICE8(n28, 3, 1)] ^                                 \
                t[ 27][SLICE8(n28, 4, 1)] ^                                 \
                t[ 26][SLICE8(n28, 5, 1)] ^                                 \
                t[ 25][SLICE8(n28, 6, 1)] ^                                 \
                t[ 24][SLICE8(n28, 7, 1)] ^                                 \
                t[ 23][SLICE8(n29, 0, 1)] ^                                 \
                t[ 22][SLICE8(n29, 1, 1)] ^                                 \
                t[ 21][SLICE8(n29, 2, 1)] ^                                 \
                t[ 20][SLICE8(n29, 3, 1)] ^                                 \
                t[ 19][SLICE8(n29, 4, 1)] ^                                 \
                t[ 18][SLICE8(n29, 5, 1)] ^                                 \
                t[ 17][SLICE8(n29, 6, 1)] ^                                 \
                t[ 16][SLICE8(n29, 7, 1)] ^                                 \
                t[ 15][SLICE8(n30, 0, 1)] ^                                 \
                t[ 14][SLICE8(n30, 1, 1)] ^                                 \
                t[ 13][SLICE8(n30, 2, 1)] ^                                 \
                t[ 12][SLICE8(n30, 3, 1)] ^                                 \
                t[ 11][SLICE8(n30, 4, 1)] ^                                 \
                t[ 10][SLICE8(n30, 5, 1)] ^                                 \
                t[  9][SLICE8(n30, 6, 1)] ^                                 \
                t[  8][SLICE8(n30, 7, 1)] ^                                 \
                t[  7][SLICE8(n31, 0, 1)] ^                                 \
                t[  6][SLICE8(n31, 1, 1)] ^                                 \
                t[  5][SLICE8(n31, 2, 1)] ^                                 \
                t[  4][SLICE8(n31, 3, 1)] ^                                 \
                t[  3][SLICE8(n31, 4, 1)] ^                                 \
                t[  2][SLICE8(n31, 5, 1)] ^                                 \
                t[  1][SLICE8(n31, 6, 1)] ^                                 \
                t[  0][SLICE8(n31, 7, 1)];                                  \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                t[7][SLICE8(n, 0, 1)] ^                                     \
                t[6][SLICE8(n, 1, 1)] ^                                     \
                t[5][SLICE8(n, 2, 1)] ^                                     \
                t[4][SLICE8(n, 3, 1)] ^                                     \
                t[3][SLICE8(n, 4, 1)] ^                                     \
                t[2][SLICE8(n, 5, 1)] ^                                     \
                t[1][SLICE8(n, 6, 1)] ^                                     \
                t[0][SLICE8(n, 7, 1)];                                      \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BY32_SOLO_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BY32_SOLO */
