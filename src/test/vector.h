#ifndef VECTOR_H

#include "vec.h"

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

VEC_INCLUDE(VSize, vsize, size_t, BY_VAL);

typedef struct Str Str;
VEC_INCLUDE(VStr, vstr, Str, BY_REF);
VEC_INCLUDE(VrStr, vrstr, Str, BY_REF);


#define VECTOR_H
#endif

