/**
 * @file crcea.h
 * @brief 汎用 CRC 生成器
 * @author dearblue <dearblue@users.noreply.github.com>
 * @license Creative Commons License Zero (CC0 / Public Domain)
 */

#ifndef CRCEA_H__
#define CRCEA_H__ 1

#include "crcea/defs.h"

crcea_int crcea_setup(crcea_context *cc, crcea_int crc);
crcea_int crcea_update(crcea_context *cc, const void *src, const void *srcend, crcea_int state);
crcea_int crcea_finish(crcea_context *cc, crcea_int state);

#endif /* CRCEA_H__ */
