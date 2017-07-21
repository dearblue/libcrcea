/*
 * file:: examples/without-context/main.c
 * author:: dearblue <https://github.com/dearblue>
 * license:: Creative Commons Zero License (CC0 / Public Domain)
 *
 * crcea/core.h を用いた実例コード。
 *
 * テーブルは static 変数で確保し、初期化は最初の一回のみ。
 */

#define CRCEA_ACADEMIC

#define CRCEA_PREFIX crc32
#define CRCEA_TYPE uint32_t
#include <crcea/core.h>

static const crcea_model crc32_model = {
    .bitsize = 32,
    .polynomial = 0x04c11db7ul,
    .reflectin = 1,
    .reflectout = 1,
    .xoroutput = ~0ul,
};

static const crcea_model crc32x_model = {
    .bitsize = 32,
    .polynomial = 0x04c11db7ul,
    .reflectin = 0,
    .reflectout = 0,
    .xoroutput = ~0ul,
};

uint32_t
crc32_bitbybit(const void *ptr, const void *end, uint32_t crc)
{
    uint32_t s = crc32_setup(&crc32_model, crc);
    s = crc32_update_bitbybit(&crc32_model, ptr, end, s);
    return crc32_finish(&crc32_model, s);
}

uint32_t
crc32_by8q(const void *ptr, const void *end, uint32_t crc)
{
    static uint32_t table[16][16] = { 0 };
    if (table[0][1] == 0) {
        crc32_build_table(&crc32_model, CRCEA_BY8_QUARTET, table);
    }

    uint32_t s = crc32_setup(&crc32_model, crc);
    s = crc32_update(&crc32_model, ptr, end, s, CRCEA_BY8_QUARTET, table);
    return crc32_finish(&crc32_model, s);
}

uint32_t
crc32_slicing_by8(const void *ptr, const void *end, uint32_t crc)
{
    static uint32_t table[16][256] = { 0 };
    if (table[0][1] == 0) {
        crc32_build_table(&crc32_model, CRCEA_BY8_OCTET, table);
    }

    uint32_t s = crc32_setup(&crc32_model, crc);
    s = crc32_update_by8_octet(&crc32_model, ptr, end, s, table);
    return crc32_finish(&crc32_model, s);
}
