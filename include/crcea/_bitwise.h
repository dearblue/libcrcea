/**
 * @file _bitwise.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 */

#ifdef CRCEA_ENABLE_BITWISE_CONDXOR

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITWISE_CONDXOR(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITWISE_CONDXOR_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_TYPE poly = ADAPT(design->polynomial, design->bitsize);           \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                        \
        int i;                                                              \
        state ^= INPUT(*IN);                                                \
        for (i = 8; i > 0; i --) {                                          \
            char head = SLICE(state, 0, 1);                                 \
            state = SHIFT(state, 1);                                        \
            if (head) { state ^= poly; }                                    \
        }                                                                   \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        int i;                                                              \
        state ^= INPUT(*IN);                                                \
        for (i = 8; i > 0; i --) {                                          \
            char head = SLICE(state, 0, 1);                                 \
            state = SHIFT(state, 1);                                        \
            if (head) { state ^= poly; }                                    \
        }                                                                   \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BITWISE_CONDXOR_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BITWISE_CONDXOR */

#ifdef CRCEA_ENABLE_BITWISE_BRANCHASSIGN

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITWISE_BRANCHASSIGN(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITWISE_BRANCHASSIGN_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_TYPE poly = ADAPT(design->polynomial, design->bitsize);           \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                        \
        int i;                                                              \
        state ^= INPUT(*IN);                                                \
        for (i = 8; i > 0; i --) {                                          \
            if (SLICE(state, 0, 1) == 0) {                                  \
                state = SHIFT(state, 1);                                    \
            } else {                                                        \
                state = SHIFT(state, 1) ^ poly;                             \
            }                                                               \
        }                                                                   \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        int i;                                                              \
        state ^= INPUT(*IN);                                                \
        for (i = 8; i > 0; i --) {                                          \
            if (SLICE(state, 0, 1) == 0) {                                  \
                state = SHIFT(state, 1);                                    \
            } else {                                                        \
                state = SHIFT(state, 1) ^ poly;                             \
            }                                                               \
        }                                                                   \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BITWISE_BRANCHASSIGN_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BITWISE_BRANCHASSIGN */

#ifdef CRCEA_ENABLE_BITWISE_BRANCHMIX

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITWISE_BRANCHMIX(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITWISE_BRANCHMIX_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_TYPE poly = ADAPT(design->polynomial, design->bitsize);           \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                        \
        int i;                                                              \
        state ^= INPUT(*IN);                                                \
        for (i = 8; i > 0; i --) {                                          \
            state = SHIFT(state, 1) ^ (SLICE(state, 0, 1) == 0 ? 0 : poly); \
        }                                                                   \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        int i;                                                              \
        state ^= INPUT(*IN);                                                \
        for (i = 8; i > 0; i --) {                                          \
            state = SHIFT(state, 1) ^ (SLICE(state, 0, 1) == 0 ? 0 : poly); \
        }                                                                   \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BITWISE_BRANCHMIX_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BITWISE_BRANCHMIX */

#ifdef CRCEA_ENABLE_BITWISE_BRANCHLESS

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITWISE_BRANCHLESS(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITWISE_BRANCHLESS_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_TYPE poly = ADAPT(design->polynomial, design->bitsize);           \
    CRCEA_UPDATE_STRIPE(IN, END, 1);                                        \
        int i;                                                              \
        state ^= INPUT(*IN);                                                \
        for (i = 8; i > 0; i --) {                                          \
            state = SHIFT(state, 1) ^ (-SLICE(state, 0, 1) & poly);         \
        }                                                                   \
    CRCEA_UPDATE_BYTE(IN, END);                                             \
        int i;                                                              \
        state ^= INPUT(*IN);                                                \
        for (i = 8; i > 0; i --) {                                          \
            state = SHIFT(state, 1) ^ (-SLICE(state, 0, 1) & poly);         \
        }                                                                   \
    CRCEA_UPDATE_END();                                                     \

    CRCEA_UPDATE_DECL(design, p, pp, state, CRCEA_BITWISE_BRANCHLESS_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BITWISE_BRANCHLESS */
