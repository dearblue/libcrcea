/**
 * @file _bitbybit.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 */

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITBYBIT(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITBYBIT_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    CRCEA_TYPE poly = ADAPT(model->polynomial, model->bitsize);             \
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

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BITBYBIT_DECL);

    return state;
}

#ifdef CRCEA_ENABLE_BITBYBIT_FAST

/*
 * reference:
 * * http://www.hackersdelight.org/hdcodetxt/crc.c.txt#crc32h
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_BITBYBIT_FAST(const crcea_model *model, const char *p, const char *pp, CRCEA_TYPE state)
{
#define CRCEA_BITBYBIT_FAST_DECL(IN, END, ADAPT, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \
    const CRCEA_TYPE g0 = ADAPT(model->polynomial, model->bitsize),         \
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

    CRCEA_UPDATE_DECL(model, p, pp, state, CRCEA_BITBYBIT_FAST_DECL);

    return state;
}

#endif /* CRCEA_ENABLE_BITBYBIT_FAST */
