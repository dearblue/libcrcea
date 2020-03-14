/**
 * @file _reference.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.osdn.me>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 */

#ifdef CRCEA_ENABLE_REFERENCE

CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
CRCEA_UPDATE_REFERENCE(const crcea_design *design, const void *ptr, const void *const end, CRCEA_TYPE state)
{
    const uint8_t *p = (const uint8_t *)ptr;
    const uintptr_t pp = (uintptr_t)end;

    if (design->reflectin) {
        CRCEA_TYPE poly = CRCEA_ADAPT_POLYNOMIAL_R(design->polynomial, design->bitsize);
        if (design->appendzero) {
            for (; (uintptr_t)p < pp; p ++) {
                uint8_t ch = *p;
                for (int i = 8; i > 0; i --) {
                    state ^= ch & 1;
                    int carryover = state & 1;
                    state >>= 1;
                    if (carryover) { state ^= poly; }
                    ch >>= 1;
                }
            }
        } else {
            int sh = design->bitsize - 1;
            for (; (uintptr_t)p < pp; p ++) {
                CRCEA_TYPE ch = *p;
                for (int i = 8; i > 0; i --) {
                    int carryover = state & 1;
                    state >>= 1;
                    if (carryover) { state ^= poly; }
                    state ^= (ch & 1) << sh;
                    ch >>= 1;
                }
            }
        }
    } else {
        /*
         * NOTE: normal-input で design.bitsize と CRCEA_BITSIZE が不一致の場合
         * NOTE: MSB がビット列の先頭となるように state が左シフトされている。
         * NOTE: これは多くが定数化となることで、高速化を期待してのこと。
         */

        CRCEA_TYPE poly = CRCEA_ADAPT_POLYNOMIAL(design->polynomial, design->bitsize);

        if (design->appendzero) {
            for (; (uintptr_t)p < pp; p ++) {
                uint8_t ch = *p;
                for (int i = 8; i > 0; i --) {
                    state ^= ((CRCEA_TYPE)(ch >> 7) & 1) << (CRCEA_BITSIZE - 1);
                    int carryover = (state >> (CRCEA_BITSIZE - 1)) & 1;
                    state <<= 1;
                    if (carryover) { state ^= poly; }
                    ch <<= 1;
                }
            }
        } else {
            int sh = CRCEA_BITSIZE - design->bitsize;
            for (; (uintptr_t)p < pp; p ++) {
                uint8_t ch = *p;
                for (int i = 8; i > 0; i --) {
                    int carryover = (state >> (CRCEA_BITSIZE - 1)) & 1;
                    state <<= 1;
                    if (carryover) { state ^= poly; }
                    state ^= (CRCEA_TYPE)(ch >> 7) << sh;
                    ch <<= 1;
                }
            }
        }
    }

    return state;
}

#endif /* CRCEA_ENABLE_REFERENCE */
