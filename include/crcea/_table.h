/**
 * @file _table.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.osdn.me>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 */

#define CRCEA_BUILD_TABLE_DEFINE(BITS, DESIGN, F)                           \
    do {                                                                    \
        if (BITS > 8 && CRCEA_BITSIZE < 16) {                               \
            if ((DESIGN)->reflectin) {                                      \
                F(uint16_t, CRCEA_ADAPT_POLYNOMIAL_R,                       \
                        CRCEA_INPUTW_R, CRCEA_RSH16, CRCEA_RSH,             \
                        CRCEA_SLICE16_R, CRCEA_SLICE_R, CRCEA_STORE_R);     \
            } else {                                                        \
                F(uint16_t, CRCEA_ADAPT_POLYNOMIALW, CRCEA_INPUT16,         \
                        CRCEA_LSH16, CRCEA_LSH, CRCEA_SLICE16,              \
                        CRCEA_SLICE, CRCEA_STORE16);                        \
            }                                                               \
        } else {                                                            \
            if ((DESIGN)->reflectin) {                                      \
                F(CRCEA_TYPE, CRCEA_ADAPT_POLYNOMIAL_R, CRCEA_INPUTW_R,     \
                        CRCEA_RSH, CRCEA_RSH, CRCEA_SLICE_R,                \
                        CRCEA_SLICE_R, CRCEA_STORE_R);                      \
            } else {                                                        \
                F(CRCEA_TYPE, CRCEA_ADAPT_POLYNOMIAL, CRCEA_INPUTW,         \
                        CRCEA_LSH, CRCEA_LSH, CRCEA_SLICE,                  \
                        CRCEA_SLICE, CRCEA_STORE);                          \
            }                                                               \
        }                                                                   \
    } while (0)                                                             \

CRCEA_VISIBILITY CRCEA_INLINE size_t
CRCEA_TABLESIZE(int algo)
{
    switch (algo) {
    case CRCEA_BY_SOLO:
        return sizeof(CRCEA_TYPE[2]);
    case CRCEA_BY1_SOLO:
    case CRCEA_BY2_SOLO:
    case CRCEA_BY4_SOLO:
    case CRCEA_BY8_SOLO:
    case CRCEA_BY16_SOLO:
    case CRCEA_BY32_SOLO:
        return sizeof(CRCEA_TYPE[8 * (1 << (algo & 0xff) >> 1)][2]);
    case CRCEA_BY_DUO:
        return sizeof(CRCEA_TYPE[4]);
    case CRCEA_BY1_DUO:
    case CRCEA_BY2_DUO:
    case CRCEA_BY4_DUO:
    case CRCEA_BY8_DUO:
    case CRCEA_BY16_DUO:
    case CRCEA_BY32_DUO:
        return sizeof(CRCEA_TYPE[4 * (1 << (algo & 0xff) >> 1)][4]);
    case CRCEA_BY_QUARTET:
        return sizeof(CRCEA_TYPE[16]);
    case CRCEA_BY1_QUARTET:
    case CRCEA_BY2_QUARTET:
    case CRCEA_BY4_QUARTET:
    case CRCEA_BY8_QUARTET:
    case CRCEA_BY16_QUARTET:
    case CRCEA_BY32_QUARTET:
        return sizeof(CRCEA_TYPE[2 * (1 << (algo & 0xff) >> 1)][16]);
    case CRCEA_BY1_OCTET:
    case CRCEA_BY2_OCTET:
    case CRCEA_BY4_OCTET:
    case CRCEA_BY8_OCTET:
    case CRCEA_BY16_OCTET:
    case CRCEA_BY32_OCTET:
        return sizeof(CRCEA_TYPE[1 * (1 << (algo & 0xff) >> 1)][256]);
    case CRCEA_BY2_SEXDECTET:
    case CRCEA_BY4_SEXDECTET:
    case CRCEA_BY8_SEXDECTET:
    case CRCEA_BY16_SEXDECTET:
    case CRCEA_BY32_SEXDECTET:
        return sizeof(CRCEA_TYPE[1 * (1 << (algo & 0xff) >> 2)][65536]);
    default:
        return 0;
    }
}

CRCEA_VISIBILITY CRCEA_INLINE void
CRCEA_BUILD_TABLE(const crcea_design *design, int algorithm, void *table)
{
    unsigned int times, round, bits;
    switch (algorithm) {
    case CRCEA_BY_SOLO:
    case CRCEA_BY1_SOLO:
    case CRCEA_BY2_SOLO:
    case CRCEA_BY4_SOLO:
    case CRCEA_BY8_SOLO:
    case CRCEA_BY16_SOLO:
    case CRCEA_BY32_SOLO:
        round = 8 * (1 << (algorithm & 0xff) >> 1);
        bits = 1;
        break;
    case CRCEA_BY_DUO:
    case CRCEA_BY1_DUO:
    case CRCEA_BY2_DUO:
    case CRCEA_BY4_DUO:
    case CRCEA_BY8_DUO:
    case CRCEA_BY16_DUO:
    case CRCEA_BY32_DUO:
        round = 4 * (1 << (algorithm & 0xff) >> 1);
        bits = 2;
        break;
    case CRCEA_BY_QUARTET:
    case CRCEA_BY1_QUARTET:
    case CRCEA_BY2_QUARTET:
    case CRCEA_BY4_QUARTET:
    case CRCEA_BY8_QUARTET:
    case CRCEA_BY16_QUARTET:
    case CRCEA_BY32_QUARTET:
        round = 2 * (1 << (algorithm & 0xff) >> 1);
        bits = 4;
        break;
    case CRCEA_BY1_OCTET:
    case CRCEA_BY2_OCTET:
    case CRCEA_BY4_OCTET:
    case CRCEA_BY8_OCTET:
    case CRCEA_BY16_OCTET:
    case CRCEA_BY32_OCTET:
        round = (1 << (algorithm & 0xff) >> 1);
        bits = 8;
        break;
    case CRCEA_BY2_SEXDECTET:
    case CRCEA_BY4_SEXDECTET:
    case CRCEA_BY8_SEXDECTET:
    case CRCEA_BY16_SEXDECTET:
    case CRCEA_BY32_SEXDECTET:
        round = (1 << (algorithm & 0xff) >> 2);
        bits = 16;
        break;
    default:
        return;
    }

    times = 1 << bits;

    CRCEA_TYPE *t = table;
    const CRCEA_TYPE *tt = t;

#define CRCEA_BUILD_TABLE_DECL(TYPE, ADAPT, INPUT, SHIFT, SHIFTS, SLICE, SLICES, STORE) \
    TYPE poly = ADAPT(design->polynomial, design->bitsize);                 \
    for (uint32_t b = 0; b < times; b ++, t ++) {                           \
        TYPE r = INPUT((TYPE)b, bits);                                      \
        for (int i = bits; i > 0; i --) {                                   \
            r = SHIFT(r, 1) ^ (poly & -SLICE(r, 0, 1));                     \
        }                                                                   \
        *t = STORE(r);                                                      \
    }                                                                       \
                                                                            \
    for (unsigned int s = 1; s < round; s ++) {                             \
        const CRCEA_TYPE *q = t - times;                                    \
        for (uint32_t b = 0; b < times; b ++, t ++, q ++) {                 \
            *t = tt[SLICES(*q, 0, bits)] ^ SHIFTS(*q, bits);                \
        }                                                                   \
    }                                                                       \

    CRCEA_BUILD_TABLE_DEFINE(bits, design, CRCEA_BUILD_TABLE_DECL);
}
