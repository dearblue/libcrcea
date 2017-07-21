/*
 * file:: examples/with-context/main.c
 * author:: dearblue <https://github.com/dearblue>
 * license:: Creative Commons Zero License (CC0 / Public Domain)
 *
 * crcea.h を用いた実例コード。
 *
 * テーブルの確保や初期化は libcrcea に丸投げ。
 */

#include <crcea.h>

static const crcea_model crc32_model = {
    .bitsize = 32,
    .polynomial = 0x04c11db7ul,
    .reflect_input = 1,
    .reflect_output = 1,
    .xor_output = ~0ul,
};

uint32_t
crc32_bitbybit(const void *ptr, const void *end, uint32_t crc)
{
    static crcea_context context = {
        .model = &crc32_model,
        .algorithm = CRCEA_BITBYBIT,
        .table = NULL,
        .alloc = NULL,
    };

    uint32_t s = crcea_setup(&context, crc);
    s = crcea_update(&context, ptr, end, s);
    return crcea_finish(&context, s);
}

uint32_t
crc32_slicing_by8(const void *ptr, const void *end, uint32_t crc)
{
    static crcea_context context = {
        .model = &crc32_model,
        .algorithm = CRCEA_BY8_OCTET,
        .table = NULL,
        .alloc = NULL,
    };

    uint32_t s = crcea_setup(&context, crc);
    s = crcea_update(&context, ptr, end, s);
    return crcea_finish(&context, s);
}
