#include "lutd_int.h"
/* https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key */

static inline size_t lut_int_hash_impl(unsigned int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

LUTD_IMPLEMENT(LutInt, lut_int, int, BY_VAL, lut_int_hash_impl, 0, 0)


