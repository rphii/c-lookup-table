#ifndef LUT_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>


#define STATIC_ASSERT(...) _Static_assert(__VA_ARGS__)

typedef enum {
    LUTD_ERROR_NONE,
    /* errors below */
    LUTD_ERROR_MALLOC = -1,
    LUTD_ERROR_REALLOC = -2,
    LUTD_ERROR_EXPECT_NULL = -3,
} LutsErrorList;

#define LUT_EMPTY           SIZE_MAX
#define LUT_WIDTH_MIN       3

#define LUT_SHIFT(width)    (width)
#define LUT_CAP(width)      (!!width * (size_t)1ULL << LUT_SHIFT(width))

#include "str.h"    // only include because this is an example draft

typedef struct LutItem {
    Str key;       // make this variable
    Str val;       // make this variable
    size_t hash;
} LutItem;

typedef struct Lut {
    LutItem **buckets;
    size_t used;
    uint8_t width;    // cap=1ULL<<(width)
} Lut;

void lut_free(Lut *lut, bool sub);      // remove sub argument via free func
int lut_grow(Lut *lut, size_t width);

int lut_set(Lut *lut, Str *key, Str *val);
Str *lut_get(Lut *lut, Str *key);
void lut_del(Lut *lut, Str *key);


#define LUT_H
#endif

