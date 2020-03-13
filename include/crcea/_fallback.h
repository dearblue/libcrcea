/**
 * @file _fallback.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 */

#ifndef CRCEA_ENABLE_FALLBACK
# if    defined CRCEA_ENABLE_BITWISE_BRANCHLESS
#  define CRCEA_UPDATE_FALLBACK         CRCEA_UPDATE_BITWISE_BRANCHLESS
# elif  defined CRCEA_ENABLE_BITWISE_CONDXOR
#  define CRCEA_UPDATE_FALLBACK         CRCEA_UPDATE_BITWISE_CONDXOR
# elif  defined CRCEA_ENABLE_BITWISE_BRANCHASSIGN
#  define CRCEA_UPDATE_FALLBACK         CRCEA_UPDATE_BITWISE_BRANCHASSIGN
# elif  defined CRCEA_ENABLE_BITWISE_BRANCHMIX
#  define CRCEA_UPDATE_FALLBACK         CRCEA_UPDATE_BITWISE_BRANCHMIX
# endif
#endif

#ifndef CRCEA_UPDATE_FALLBACK
# define CRCEA_UPDATE_FALLBACK          CRCEA_TOKEN(_update_fallback)
# define CRCEA_GALOIS_DIVISION2_NORMAL  CRCEA_TOKEN(_gdiv2_normal)
# define CRCEA_GALOIS_DIVISION2_REFLECTED CRCEA_TOKEN(_gdiv2_reflected)

/*
 * galois division
 */
CRCEA_VISIBILITY CRCEA_TYPE
CRCEA_GALOIS_DIVISION2_NORMAL(CRCEA_TYPE modpoly, CRCEA_TYPE num)
{
    num <<= CRCEA_BITSIZE - 2;

    for (int i = 0; i < 2; i ++) {
        num = (num << 1) ^ (((uint8_t)(num >> (CRCEA_BITSIZE - 1)) & 1) != 0 ? modpoly : 0);
    }

    return num;
}

CRCEA_VISIBILITY CRCEA_TYPE
CRCEA_GALOIS_DIVISION2_REFLECTED(CRCEA_TYPE modpoly, CRCEA_TYPE num)
{
    for (int i = 0; i < 2; i ++) {
        num = (num >> 1) ^ (((uint8_t)num & 1) != 0 ? modpoly : 0);
    }

    return num;
}

/*
 * Slicing by Quadruple Duo as fallback
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_FALLBACK(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state)
{
    CRCEA_TYPE table[4][4];

    if (design->reflectin) {
        CRCEA_TYPE modpoly = CRCEA_BITREFLECT(design->polynomial) >> (CRCEA_BITSIZE - design->bitsize);

        table[0][0] = 0;
        table[0][1] = CRCEA_GALOIS_DIVISION2_REFLECTED(modpoly, 1);
        table[0][2] = CRCEA_GALOIS_DIVISION2_REFLECTED(modpoly, 2);
        table[0][3] = CRCEA_GALOIS_DIVISION2_REFLECTED(modpoly, 3);
        table[1][0] = 0;
        table[1][1] = (table[0][1] >> 2) ^ table[0][(uint8_t)table[0][1] & 0x03];
        table[1][2] = (table[0][2] >> 2) ^ table[0][(uint8_t)table[0][2] & 0x03];
        table[1][3] = (table[0][3] >> 2) ^ table[0][(uint8_t)table[0][3] & 0x03];
        table[2][0] = 0;
        table[2][1] = (table[1][1] >> 2) ^ table[0][(uint8_t)table[1][1] & 0x03];
        table[2][2] = (table[1][2] >> 2) ^ table[0][(uint8_t)table[1][2] & 0x03];
        table[2][3] = (table[1][3] >> 2) ^ table[0][(uint8_t)table[1][3] & 0x03];
        table[3][0] = 0;
        table[3][1] = (table[2][1] >> 2) ^ table[0][(uint8_t)table[2][1] & 0x03];
        table[3][2] = (table[2][2] >> 2) ^ table[0][(uint8_t)table[2][2] & 0x03];
        table[3][3] = (table[2][3] >> 2) ^ table[0][(uint8_t)table[2][3] & 0x03];
    } else {
        CRCEA_TYPE modpoly = design->polynomial << (CRCEA_BITSIZE - design->bitsize);

        table[0][0] = 0;
        table[0][1] = CRCEA_GALOIS_DIVISION2_NORMAL(modpoly, 1);
        table[0][2] = CRCEA_GALOIS_DIVISION2_NORMAL(modpoly, 2);
        table[0][3] = CRCEA_GALOIS_DIVISION2_NORMAL(modpoly, 3);
        table[1][0] = 0;
        table[1][1] = (table[0][1] << 2) ^ table[0][(uint8_t)(table[0][1] >> (CRCEA_BITSIZE - 2)) & 0x03];
        table[1][2] = (table[0][2] << 2) ^ table[0][(uint8_t)(table[0][2] >> (CRCEA_BITSIZE - 2)) & 0x03];
        table[1][3] = (table[0][3] << 2) ^ table[0][(uint8_t)(table[0][3] >> (CRCEA_BITSIZE - 2)) & 0x03];
        table[2][0] = 0;
        table[2][1] = (table[1][1] << 2) ^ table[0][(uint8_t)(table[1][1] >> (CRCEA_BITSIZE - 2)) & 0x03];
        table[2][2] = (table[1][2] << 2) ^ table[0][(uint8_t)(table[1][2] >> (CRCEA_BITSIZE - 2)) & 0x03];
        table[2][3] = (table[1][3] << 2) ^ table[0][(uint8_t)(table[1][3] >> (CRCEA_BITSIZE - 2)) & 0x03];
        table[3][0] = 0;
        table[3][1] = (table[2][1] << 2) ^ table[0][(uint8_t)(table[2][1] >> (CRCEA_BITSIZE - 2)) & 0x03];
        table[3][2] = (table[2][2] << 2) ^ table[0][(uint8_t)(table[2][2] >> (CRCEA_BITSIZE - 2)) & 0x03];
        table[3][3] = (table[2][3] << 2) ^ table[0][(uint8_t)(table[2][3] >> (CRCEA_BITSIZE - 2)) & 0x03];
    }

#define CRCEA_FALLBACK_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                        \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                table[3][SLICE8(n, 0, 2)] ^                                 \
                table[2][SLICE8(n, 2, 2)] ^                                 \
                table[1][SLICE8(n, 4, 2)] ^                                 \
                table[0][SLICE8(n, 6, 2)];                                  \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        const uint8_t n = SLICE(state, 0, 8);                               \
        state = SHIFT(state, 8) ^                                           \
                table[3][SLICE8(n, 0, 2)] ^                                 \
                table[2][SLICE8(n, 2, 2)] ^                                 \
                table[1][SLICE8(n, 4, 2)] ^                                 \
                table[0][SLICE8(n, 6, 2)];                                  \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_FALLBACK_DECL);

    return state;
}

#endif /* CRCEA_UPDATE_FALLBACK */
