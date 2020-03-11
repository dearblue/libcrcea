# libcrcea - Configurable general CRC calcurator for C

C で書かれた汎用 CRC の実装です。mruby-crc-0.2 から分離独立しました。

"crcea" は "crc" と "sea" を掛けたものです。
そのため "crcea" は『シーアールシー』とお読み下さい。

[クリエイティブ・コモンズ ゼロ ライセンス (CC0 / Public Domain)](http://creativecommons.org/publicdomain/zero/1.0/) の下で利用することが出来ます。

利用者がプログラム実行中に crcea_model 構造体として設定可能な項目は次の通りです:

| フィールド名 | 意味                        | 値の範囲     | CRC-32     | CRC-32C    | CRC-32-MPEG-2 | CRC-16-CCITT (mruby) |
| ------------ | --------------------------- | ------------ | ---------- | ---------- | ------------- | ------------ |
| bitsize      | CRC ビット長                | 1-64         | 32         | 32         | 32            | 16           |
| polynomial   | 生成多項式                  | 任意の整数値 | 0x04C11DB7 | 0x1EDC6F41 | 0x04C11DB7    | 0x1021       |
| reflectin    | 入力ビット方向は逆順か      | 0, 1         | 1          | 1          | 0             | 0            |
| reflectout   | 出力ビット方向は逆順か      | 0, 1         | 1          | 1          | 0             | 0            |
| appendzero   | 入力値の後ろに 0 を詰めるか | 0, 1         | 1          | 1          | 1             | 0            |
| xoroutput    | 出力 XOR ビットマスク       | 任意の整数値 | 0xffffffff | 0xffffffff | 0             | 0            |

``appendzero=1`` は <https://ja.wikipedia.org/wiki/巡回冗長検査> にある “検査対象のビットストリームに多項式除算を行う前にnビットの0を常に後置する実装” の事です。
``appendzero=0`` だと “検査対象のビットストリームに固定ビットパターンを常に前置する実装” (ただし固定ビットパターンは "0") を意味します。
<https://en.wikipedia.org/wiki/Cyclic_redundancy_check> では “appends n 0-bits”、“prefixes a fixed bit pattern” としてまとめられています。

実際の設定値をどうするのかについては、[Catalogue of parametrised CRC algorithms](http://reveng.sourceforge.net/crc-catalogue/all.htm) が良い資料となるでしょう。

また、``struct crcea_context`` の ``algorithm`` フィールドに ``enum crcea_algorithms`` の値を代入することでアルゴリズムの変更が行なえます。

利用可能なアルゴリズムを次に示します:

| 定数値                                       | 概要                                         |
| -------------------------------------------- | -------------------------------------------- |
| CRCEA_REFERENCE                              | 高速化手法を用いない最も基本的なアルゴリズム |
| CRCEA_BITWISE_CONDXOR                        | ビット単位で計算するアルゴリズム (シフトしてから桁溢れの分岐する) |
| CRCEA_BITWISE_BRANCHASSIGN                   | ビット単位で計算するアルゴリズム (事前に桁溢れを検出して分岐する) |
| CRCEA_BITWISE_BRANCHMIX                      | ビット単位で計算するアルゴリズム (シフトと桁溢れの分岐処理を混合する) |
| CRCEA_BITWISE_BRANCHLESS                     | ビット単位で計算するアルゴリズム (分岐せずに桁溢れの処理を混合する) |
| CRCEA_BITCOMBINE2                            | 2ビット単位の並列計算を行うアルゴリズム      |
| CRCEA_BITCOMBINE4                            | 4ビット単位の並列計算を行うアルゴリズム      |
| CRCEA_BITCOMBINE8                            | 8ビット単位の並列計算を行うアルゴリズム      |
| CRCEA_BITCOMBINE16                           | 最大16ビット単位の並列計算を行うアルゴリズム |
| CRCEA_BITCOMBINE32                           | 最大32ビット単位の並列計算を行うアルゴリズム |
| CRCEA_{BY,BY1,BY2,BY4,BY8,BY16,BY32}_SOLO    | 1 ビット単位によるテーブル引きアルゴリズム   |
| CRCEA_{BY,BY1,BY2,BY4,BY8,BY16,BY32}_DUO     | 2 ビット単位によるテーブル引きアルゴリズム   |
| CRCEA_{BY,BY1,BY2,BY4,BY8,BY16,BY32}_QUARTET | 4 ビット単位によるテーブル引きアルゴリズム   |
| CRCEA_{BY1,BY2,BY4,BY8,BY16,BY32}_OCTET      | 8 ビット単位によるテーブル引きアルゴリズム   |
| CRCEA_{BY2,BY4,BY8,BY16,BY32}_SEXDECTET      | 16 ビット単位によるテーブル引きアルゴリズム  |

  * CRCEA_BY1_OCTET は標準的なテーブルアルゴリズムです。
  * CRCEA_BY{2,4,8,16,32}_\* はインテルが発表した Slicing by 4/8 アルゴリズムを元にした変種です。
  * CRCEA_BY_QUARTET はハーフバイトテーブルアルゴリズムです。
  * CRCEA_BY1_SOLO は CRCEA_ENABLE_BITCOMBINE8 をテーブルに置き換えたアルゴリズムに相当します。

## HOW TO USAGE

### 高水準 API

静的ライブラリ libcrcea.a をリンクさせることで利用可能な方法です。

整数値型を区別する必要はなく、テーブルの動的確保・初期化まで自動で行われます。

今のところ libcrcea.a は CRCEA_ACADEMIC が定義され構築されているため、特定の CRC を計算するためには巨大すぎるという欠点があります。

使い方は

 1. crcea_model と crcea_context を用意する
 2. crcea_setup() で CRC 値を内部処理の値に変換する
 3. crcea_update() で任意長の入力値を処理する
 4. crcea_finish() で最終 CRC 値に変換する

というのが一連の流れとなります。

```c:mycrc32.c
#include <stdio.h>
#include <string.h>
#include <crcea.h>

int
main(int argc, char *argv[])
{
    static const crc32_init = 0ul;

    static const crcea_model crc32_model = {
        .bitsize = 32,
        .reflectin = 1,
        .reflectout = 1,
        .appendzero = 1,
        .polynomial = 0x04c11db7ul,
        .xoroutput = ~0ul,
    };

    static crcea_context crc32_context = {
        .model = &crc32_model,
        .algorithm = CRCEA_SLICING_BY_4,
        .table = NULL,
        .alloc = NULL,
        .opaque = NULL,
    };

    for (int i = 1; i < argc; i ++) {
        crcea_int s = crcea_setup(&crc32_context, crc32_init);
        s = crcea_update(&crc32_context, argv[i], argv[i] + strlen(argv[i]), s);
        printf("0x%08x : %s\n", crcea_finish(&crc32_context, s), argv[i]);
    }

    return 0;
}
```

```shell:shell
$ cc -o mycrc32 mycrc32.c -lcrcea
```

#### 構造体

```c:c
struct crcea_model
{
    uint32_t bitsize:8;
    uint32_t reflectin:1;
    uint32_t reflectout:1;
    uint32_t appendzero:1;
    crcea_int polynomial;
    crcea_int xoroutput;
};

struct crcea_context
{
    const crcea_model *model;
    int16_t algorithm;  /*< enum crcea_algorithms */
    const void *table;
    crcea_alloc_f *alloc;
    void *opaque;       /*< for custom memory allocator */
};
```

#### 関数

```c:c
size_t crcea_tablesize(const crcea_context *cc);
int crcea_prepare_table(crcea_context *cc);
crcea_int crcea_setup(crcea_context *cc, crcea_int crc);
crcea_int crcea_update(crcea_context *cc, const void *src, const void *srcend, crcea_int state);
crcea_int crcea_finish(crcea_context *cc, crcea_int state);
crcea_int crcea(crcea_context *cc, const void *src, const void *srcend, crcea_int crc);
```

### 低水準 API

``#include <crcea/core.h>`` して利用する方法です。

``#include`` する前に、``CRCEA_PREFIX`` と ``CRCEA_TYPE`` の定義が必要です。

また任意で ``CRCEA_MINIMAL`` ``CRCEA_TINY`` ``CRCEA_SMALL`` ``CRCEA_LARGE`` ``CRCEA_ACADEMIC`` を ``#define``
することで有効化されるアルゴリズムをまとめて指定することが出来ます。
なにが有効化される・されないについては [include/crcea/defs.h](include/crcea/defs.h) にて確認できます。

``libcrcea.a`` とのリンクは不要ですが、おそらくコンパイルに時間がかかります。

高度なコンパイラ (gcc や clang) を用いると、``crcea_model`` の静的・不変データを用意することで、最適化が見込めるかもしれません。
静的・不変な事前に計算されたテーブルを用意すると、さらなる最適化が見込める場合があります。

```c:mycrc32.c
/*
 * この例は有効アルゴリズムのプリセットを minimal にした上で
 * crcea_update_by4_octet のみを有効にする
 */
#define CRCEA_MINIMAL
#define CRCEA_ENABLE_BY4_OCTET

#define CRCEA_PREFIX private_crc32
#define CRCEA_TYPE   uint32_t
#include <crcea/core.h>

/* CRCEA_PREFIX と CRCEA_TYPE は crcea/core.h の終わりで定義が取り消されます。 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char *argv[])
{
    static const crc32_init = 0ul;

    static const crcea_model crc32_model = {
        .bitsize = 32,
        .reflectin = 1,
        .reflectout = 1,
        .appendzero = 1,
        .polynomial = 0x04c11db7ul,
        .xoroutput = ~0ul,
    };

    int algorithm = CRCEA_SLICING_BY_4;
    void *crc32_table = malloc(private_crc32_tablesize(algorithm));
    private_crc32_build_table(&crc32_model, algorithm, crc32_table);

    for (int i = 1; i < argc; i ++) {
        crcea_int s = private_crc32_setup(&crc32_model, crc32_init);
        s = private_crc32_update_by4_octet(&crc32_model, argv[i], argv[i] + strlen(argv[i]), s, crc32_table);
        printf("0x%08x : %s\n", private_crc32_finish(&crc32_model, s), argv[i]);
    }

    return 0;
}
```
