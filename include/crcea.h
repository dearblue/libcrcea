/**
 * @file crcea.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.osdn.me>
 * @license Creative Commons Zero License (CC0 / Public Domain)
 */

#ifndef CRCEA_H__
#define CRCEA_H__ 1

#include "crcea/defs.h"

size_t crcea_tablesize(const crcea_context *cc);
int crcea_prepare_table(crcea_context *cc);
crcea_int crcea_setup(const crcea_context *cc, crcea_int crc);
crcea_int crcea_update(const crcea_context *cc, const void *src, const void *srcend, crcea_int state);
crcea_int crcea_finish(const crcea_context *cc, crcea_int state);
crcea_int crcea(const crcea_context *cc, const void *src, const void *srcend, crcea_int crc);

#endif /* CRCEA_H__ */
