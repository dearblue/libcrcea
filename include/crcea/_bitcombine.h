/**
 * @file _bitcombine.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 */

/*
 * reference:
 * - https://www.hackersdelight.org/hdcodetxt/crc.c.txt#crc32g
 * - https://www.hackersdelight.org/hdcodetxt/crc.c.txt#crc32h
 */

#ifdef CRCEA_ENABLE_BITCOMBINE2

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITCOMBINE2(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITCOMBINE2_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    const CRCEA_TYPE g0 = ADAPT(design->polynomial, design->bitsize),       \
                     g1 = SHIFT(g0, 1) ^ (g0 & -SLICE(g0, 0, 1));           \
                                                                            \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                        \
        state ^= INPUT(*IN);                                                \
        state = SHIFT(state, 2) ^                                           \
                (g1 & -SLICE(state, 0, 1)) ^                                \
                (g0 & -SLICE(state, 1, 1));                                 \
        state = SHIFT(state, 2) ^                                           \
                (g1 & -SLICE(state, 0, 1)) ^                                \
                (g0 & -SLICE(state, 1, 1));                                 \
        state = SHIFT(state, 2) ^                                           \
                (g1 & -SLICE(state, 0, 1)) ^                                \
                (g0 & -SLICE(state, 1, 1));                                 \
        state = SHIFT(state, 2) ^                                           \
                (g1 & -SLICE(state, 0, 1)) ^                                \
                (g0 & -SLICE(state, 1, 1));                                 \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        state = SHIFT(state, 2) ^                                           \
                (g1 & -SLICE(state, 0, 1)) ^                                \
                (g0 & -SLICE(state, 1, 1));                                 \
        state = SHIFT(state, 2) ^                                           \
                (g1 & -SLICE(state, 0, 1)) ^                                \
                (g0 & -SLICE(state, 1, 1));                                 \
        state = SHIFT(state, 2) ^                                           \
                (g1 & -SLICE(state, 0, 1)) ^                                \
                (g0 & -SLICE(state, 1, 1));                                 \
        state = SHIFT(state, 2) ^                                           \
                (g1 & -SLICE(state, 0, 1)) ^                                \
                (g0 & -SLICE(state, 1, 1));                                 \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BITCOMBINE2_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BITCOMBINE2 */

#ifdef CRCEA_ENABLE_BITCOMBINE4

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITCOMBINE4(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITCOMBINE4_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    const CRCEA_TYPE g0 = ADAPT(design->polynomial, design->bitsize),       \
                     g1 = SHIFT(g0, 1) ^ (g0 & -SLICE(g0, 0, 1)),           \
                     g2 = SHIFT(g1, 1) ^ (g0 & -SLICE(g1, 0, 1)),           \
                     g3 = SHIFT(g2, 1) ^ (g0 & -SLICE(g2, 0, 1));           \
                                                                            \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                        \
        state ^= INPUT(*IN);                                                \
        state = SHIFT(state, 4) ^                                           \
                (g3 & -SLICE(state, 0, 1)) ^                                \
                (g2 & -SLICE(state, 1, 1)) ^                                \
                (g1 & -SLICE(state, 2, 1)) ^                                \
                (g0 & -SLICE(state, 3, 1));                                 \
        state = SHIFT(state, 4) ^                                           \
                (g3 & -SLICE(state, 0, 1)) ^                                \
                (g2 & -SLICE(state, 1, 1)) ^                                \
                (g1 & -SLICE(state, 2, 1)) ^                                \
                (g0 & -SLICE(state, 3, 1));                                 \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        state = SHIFT(state, 4) ^                                           \
                (g3 & -SLICE(state, 0, 1)) ^                                \
                (g2 & -SLICE(state, 1, 1)) ^                                \
                (g1 & -SLICE(state, 2, 1)) ^                                \
                (g0 & -SLICE(state, 3, 1));                                 \
        state = SHIFT(state, 4) ^                                           \
                (g3 & -SLICE(state, 0, 1)) ^                                \
                (g2 & -SLICE(state, 1, 1)) ^                                \
                (g1 & -SLICE(state, 2, 1)) ^                                \
                (g0 & -SLICE(state, 3, 1));                                 \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BITCOMBINE4_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BITCOMBINE4 */

#ifdef CRCEA_ENABLE_BITCOMBINE8

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITCOMBINE8(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITCOMBINE8_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    const CRCEA_TYPE g0 = ADAPT(design->polynomial, design->bitsize),       \
                     g1 = SHIFT(g0, 1) ^ (g0 & -SLICE(g0, 0, 1)),           \
                     g2 = SHIFT(g1, 1) ^ (g0 & -SLICE(g1, 0, 1)),           \
                     g3 = SHIFT(g2, 1) ^ (g0 & -SLICE(g2, 0, 1)),           \
                     g4 = SHIFT(g3, 1) ^ (g0 & -SLICE(g3, 0, 1)),           \
                     g5 = SHIFT(g4, 1) ^ (g0 & -SLICE(g4, 0, 1)),           \
                     g6 = SHIFT(g5, 1) ^ (g0 & -SLICE(g5, 0, 1)),           \
                     g7 = SHIFT(g6, 1) ^ (g0 & -SLICE(g6, 0, 1));           \
                                                                            \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                        \
        state ^= INPUT(*IN);                                                \
        state = SHIFT(state, 8) ^                                           \
                (g7 & -SLICE(state, 0, 1)) ^                                \
                (g6 & -SLICE(state, 1, 1)) ^                                \
                (g5 & -SLICE(state, 2, 1)) ^                                \
                (g4 & -SLICE(state, 3, 1)) ^                                \
                (g3 & -SLICE(state, 4, 1)) ^                                \
                (g2 & -SLICE(state, 5, 1)) ^                                \
                (g1 & -SLICE(state, 6, 1)) ^                                \
                (g0 & -SLICE(state, 7, 1));                                 \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        state = SHIFT(state, 8) ^                                           \
                (g7 & -SLICE(state, 0, 1)) ^                                \
                (g6 & -SLICE(state, 1, 1)) ^                                \
                (g5 & -SLICE(state, 2, 1)) ^                                \
                (g4 & -SLICE(state, 3, 1)) ^                                \
                (g3 & -SLICE(state, 4, 1)) ^                                \
                (g2 & -SLICE(state, 5, 1)) ^                                \
                (g1 & -SLICE(state, 6, 1)) ^                                \
                (g0 & -SLICE(state, 7, 1));                                 \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BITCOMBINE8_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BITCOMBINE8 */

#ifdef CRCEA_ENABLE_BITCOMBINE16

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITCOMBINE16(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITCOMBINE16_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    const CRCEA_TYPE g0 = ADAPT(design->polynomial, design->bitsize),       \
                     g1 = SHIFT(g0, 1) ^ (g0 & -SLICE(g0, 0, 1)),           \
                     g2 = SHIFT(g1, 1) ^ (g0 & -SLICE(g1, 0, 1)),           \
                     g3 = SHIFT(g2, 1) ^ (g0 & -SLICE(g2, 0, 1)),           \
                     g4 = SHIFT(g3, 1) ^ (g0 & -SLICE(g3, 0, 1)),           \
                     g5 = SHIFT(g4, 1) ^ (g0 & -SLICE(g4, 0, 1)),           \
                     g6 = SHIFT(g5, 1) ^ (g0 & -SLICE(g5, 0, 1)),           \
                     g7 = SHIFT(g6, 1) ^ (g0 & -SLICE(g6, 0, 1)),           \
                     g8 = SHIFT(g7, 1) ^ (g0 & -SLICE(g7, 0, 1)),           \
                     g9 = SHIFT(g8, 1) ^ (g0 & -SLICE(g8, 0, 1)),           \
                     gA = SHIFT(g9, 1) ^ (g0 & -SLICE(g9, 0, 1)),           \
                     gB = SHIFT(gA, 1) ^ (g0 & -SLICE(gA, 0, 1)),           \
                     gC = SHIFT(gB, 1) ^ (g0 & -SLICE(gB, 0, 1)),           \
                     gD = SHIFT(gC, 1) ^ (g0 & -SLICE(gC, 0, 1)),           \
                     gE = SHIFT(gD, 1) ^ (g0 & -SLICE(gD, 0, 1)),           \
                     gF = SHIFT(gE, 1) ^ (g0 & -SLICE(gE, 0, 1));           \
                                                                            \
    CRCEA_UPDATE_STRIPE(IN, END, 2);                                        \
        const uint8_t n0 = (uint8_t)IN[0] ^ SLICE(state, 0, 8);             \
        const uint8_t n1 = (uint8_t)IN[1] ^ SLICE(state, 8, 8);             \
        state = SHIFT(state, 16) ^                                          \
                (gF & -SLICE8(n0, 0, 1)) ^                                  \
                (gE & -SLICE8(n0, 1, 1)) ^                                  \
                (gD & -SLICE8(n0, 2, 1)) ^                                  \
                (gC & -SLICE8(n0, 3, 1)) ^                                  \
                (gB & -SLICE8(n0, 4, 1)) ^                                  \
                (gA & -SLICE8(n0, 5, 1)) ^                                  \
                (g9 & -SLICE8(n0, 6, 1)) ^                                  \
                (g8 & -SLICE8(n0, 7, 1)) ^                                  \
                (g7 & -SLICE8(n1, 0, 1)) ^                                  \
                (g6 & -SLICE8(n1, 1, 1)) ^                                  \
                (g5 & -SLICE8(n1, 2, 1)) ^                                  \
                (g4 & -SLICE8(n1, 3, 1)) ^                                  \
                (g3 & -SLICE8(n1, 4, 1)) ^                                  \
                (g2 & -SLICE8(n1, 5, 1)) ^                                  \
                (g1 & -SLICE8(n1, 6, 1)) ^                                  \
                (g0 & -SLICE8(n1, 7, 1));                                   \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        state = SHIFT(state, 8) ^                                           \
                (g7 & -SLICE(state, 0, 1)) ^                                \
                (g6 & -SLICE(state, 1, 1)) ^                                \
                (g5 & -SLICE(state, 2, 1)) ^                                \
                (g4 & -SLICE(state, 3, 1)) ^                                \
                (g3 & -SLICE(state, 4, 1)) ^                                \
                (g2 & -SLICE(state, 5, 1)) ^                                \
                (g1 & -SLICE(state, 6, 1)) ^                                \
                (g0 & -SLICE(state, 7, 1));                                 \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BITCOMBINE16_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BITCOMBINE16 */

#ifdef CRCEA_ENABLE_BITCOMBINE32

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITCOMBINE32(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITCOMBINE32_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    const CRCEA_TYPE g0 = ADAPT(design->polynomial, design->bitsize),       \
                     g1 = SHIFT(g0, 1) ^ (g0 & -SLICE(g0, 0, 1)),           \
                     g2 = SHIFT(g1, 1) ^ (g0 & -SLICE(g1, 0, 1)),           \
                     g3 = SHIFT(g2, 1) ^ (g0 & -SLICE(g2, 0, 1)),           \
                     g4 = SHIFT(g3, 1) ^ (g0 & -SLICE(g3, 0, 1)),           \
                     g5 = SHIFT(g4, 1) ^ (g0 & -SLICE(g4, 0, 1)),           \
                     g6 = SHIFT(g5, 1) ^ (g0 & -SLICE(g5, 0, 1)),           \
                     g7 = SHIFT(g6, 1) ^ (g0 & -SLICE(g6, 0, 1)),           \
                     g8 = SHIFT(g7, 1) ^ (g0 & -SLICE(g7, 0, 1)),           \
                     g9 = SHIFT(g8, 1) ^ (g0 & -SLICE(g8, 0, 1)),           \
                     gA = SHIFT(g9, 1) ^ (g0 & -SLICE(g9, 0, 1)),           \
                     gB = SHIFT(gA, 1) ^ (g0 & -SLICE(gA, 0, 1)),           \
                     gC = SHIFT(gB, 1) ^ (g0 & -SLICE(gB, 0, 1)),           \
                     gD = SHIFT(gC, 1) ^ (g0 & -SLICE(gC, 0, 1)),           \
                     gE = SHIFT(gD, 1) ^ (g0 & -SLICE(gD, 0, 1)),           \
                     gF = SHIFT(gE, 1) ^ (g0 & -SLICE(gE, 0, 1)),           \
                     gG = SHIFT(gF, 1) ^ (g0 & -SLICE(gF, 0, 1)),           \
                     gH = SHIFT(gG, 1) ^ (g0 & -SLICE(gG, 0, 1)),           \
                     gI = SHIFT(gH, 1) ^ (g0 & -SLICE(gH, 0, 1)),           \
                     gJ = SHIFT(gI, 1) ^ (g0 & -SLICE(gI, 0, 1)),           \
                     gK = SHIFT(gJ, 1) ^ (g0 & -SLICE(gJ, 0, 1)),           \
                     gL = SHIFT(gK, 1) ^ (g0 & -SLICE(gK, 0, 1)),           \
                     gM = SHIFT(gL, 1) ^ (g0 & -SLICE(gL, 0, 1)),           \
                     gN = SHIFT(gM, 1) ^ (g0 & -SLICE(gM, 0, 1)),           \
                     gO = SHIFT(gN, 1) ^ (g0 & -SLICE(gN, 0, 1)),           \
                     gP = SHIFT(gO, 1) ^ (g0 & -SLICE(gO, 0, 1)),           \
                     gQ = SHIFT(gP, 1) ^ (g0 & -SLICE(gP, 0, 1)),           \
                     gR = SHIFT(gQ, 1) ^ (g0 & -SLICE(gQ, 0, 1)),           \
                     gS = SHIFT(gR, 1) ^ (g0 & -SLICE(gR, 0, 1)),           \
                     gT = SHIFT(gS, 1) ^ (g0 & -SLICE(gS, 0, 1)),           \
                     gU = SHIFT(gT, 1) ^ (g0 & -SLICE(gT, 0, 1)),           \
                     gV = SHIFT(gU, 1) ^ (g0 & -SLICE(gU, 0, 1));           \
                                                                            \
    CRCEA_UPDATE_STRIPE(IN, END, 4);                                        \
        const uint8_t n0 = (uint8_t)IN[0] ^ SLICE(state,  0, 8);            \
        const uint8_t n1 = (uint8_t)IN[1] ^ SLICE(state,  8, 8);            \
        const uint8_t n2 = (uint8_t)IN[2] ^ SLICE(state, 16, 8);            \
        const uint8_t n3 = (uint8_t)IN[3] ^ SLICE(state, 24, 8);            \
        state = SHIFT(state, 32) ^                                          \
                (gV & -SLICE8(n0, 0, 1)) ^                                  \
                (gU & -SLICE8(n0, 1, 1)) ^                                  \
                (gT & -SLICE8(n0, 2, 1)) ^                                  \
                (gS & -SLICE8(n0, 3, 1)) ^                                  \
                (gR & -SLICE8(n0, 4, 1)) ^                                  \
                (gQ & -SLICE8(n0, 5, 1)) ^                                  \
                (gP & -SLICE8(n0, 6, 1)) ^                                  \
                (gO & -SLICE8(n0, 7, 1)) ^                                  \
                (gN & -SLICE8(n1, 0, 1)) ^                                  \
                (gM & -SLICE8(n1, 1, 1)) ^                                  \
                (gL & -SLICE8(n1, 2, 1)) ^                                  \
                (gK & -SLICE8(n1, 3, 1)) ^                                  \
                (gJ & -SLICE8(n1, 4, 1)) ^                                  \
                (gI & -SLICE8(n1, 5, 1)) ^                                  \
                (gH & -SLICE8(n1, 6, 1)) ^                                  \
                (gG & -SLICE8(n1, 7, 1)) ^                                  \
                (gF & -SLICE8(n2, 0, 1)) ^                                  \
                (gE & -SLICE8(n2, 1, 1)) ^                                  \
                (gD & -SLICE8(n2, 2, 1)) ^                                  \
                (gC & -SLICE8(n2, 3, 1)) ^                                  \
                (gB & -SLICE8(n2, 4, 1)) ^                                  \
                (gA & -SLICE8(n2, 5, 1)) ^                                  \
                (g9 & -SLICE8(n2, 6, 1)) ^                                  \
                (g8 & -SLICE8(n2, 7, 1)) ^                                  \
                (g7 & -SLICE8(n3, 0, 1)) ^                                  \
                (g6 & -SLICE8(n3, 1, 1)) ^                                  \
                (g5 & -SLICE8(n3, 2, 1)) ^                                  \
                (g4 & -SLICE8(n3, 3, 1)) ^                                  \
                (g3 & -SLICE8(n3, 4, 1)) ^                                  \
                (g2 & -SLICE8(n3, 5, 1)) ^                                  \
                (g1 & -SLICE8(n3, 6, 1)) ^                                  \
                (g0 & -SLICE8(n3, 7, 1));                                   \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        state ^= INPUT(*IN);                                                \
        state = SHIFT(state, 8) ^                                           \
                (g7 & -SLICE(state, 0, 1)) ^                                \
                (g6 & -SLICE(state, 1, 1)) ^                                \
                (g5 & -SLICE(state, 2, 1)) ^                                \
                (g4 & -SLICE(state, 3, 1)) ^                                \
                (g3 & -SLICE(state, 4, 1)) ^                                \
                (g2 & -SLICE(state, 5, 1)) ^                                \
                (g1 & -SLICE(state, 6, 1)) ^                                \
                (g0 & -SLICE(state, 7, 1));                                 \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BITCOMBINE32_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BITCOMBINE32 */
