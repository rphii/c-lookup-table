#ifndef LUTS_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define LUTS_ITEM_BY_VAL(T) T
#define LUTS_ITEM_BY_REF(T) T *
#define LUTS_ITEM(T, M)     LUTS_ITEM_##M(T)

#define LUTS_EMPTY           SIZE_MAX
#define LUTS_WIDTH_MIN       3

#define LUTS_SHIFT(width)           (width)
#define LUTS_CAP(width)             (!!width * (size_t)1ULL << LUT_SHIFT(width))


#define LUTS_CAST_FREE(X)           ((void *)(X))
#define LUTS_TYPE_FREE(F,X,T)       ((void (*)(T *))(F))(LUTS_CAST_FREE(X))

#define LUTS_IS_BY_REF_BY_REF       1
#define LUTS_IS_BY_REF_BY_VAL       0
#define LUTS_IS_BY_REF(M)           LUTS_IS_BY_REF_##M

#define LUTS_ASSERT_REAL(v)             assert(v && "assertion failed")
#define LUTS_ASSERT_ARG(v)              LUTS_ASSERT_REAL(v)
#define LUTS_ASSERT_ARG_M_BY_VAL(v)     do {} while(0)
#define LUTS_ASSERT_ARG_M_BY_REF(v)     LUTS_ASSERT_REAL(v)
#define LUTS_ASSERT_ARG_M(v, M)         LUTS_ASSERT_ARG_M_##M(v)

#define LUTS_INCLUDE(N, A, TK, MK, TV, MV) \
    typedef struct N##Item { \
        TK key; \
        TV val; \
        size_t hash; \
    } N##Item; \
    typedef struct N { \
        N##Item **buckets; \
        size_t used; \
        uint8_t width; \
    } N; \
    \
    void A##_free(N *lut); \
    int A##_grow(N *lut, size_t width); \
    int A##_set(N *lut, LUTS_ITEM(TK, MK) *key, LUTS_ITEM(TV, MV) *val); \
    LUTS_ITEM(TV, MV) *A##_get(N *lut, LUTS_ITEM(TK, MK) *key); \
    void A##_del(N *lut, LUTS_ITEM(TK, MK) *key); \

#define LUTS_IMPLEMENT(N, A, TK, MK, TV, MV, H, C, FK, FV)   \
    LUTS_IMPLEMENT_COMMON_STATIC_GET_ITEM(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    LUTS_IMPLEMENT_COMMON_FREE(N, A, TK, MK, TV, MV, H, C, FK, FV) \

#define LUTS_IMPLEMENT_COMMON_STATIC_GET_ITEM(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    N##Item *A##_static_get_item(N *lut, LUTS_ITEM(TK, MK) key, size_t hash, bool intend_to_set) { \
        LUTS_ASSERT_ARG(lut); \
        LUTS_ASSERT_ARG_M(key, MK); \
        size_t perturb = hash >> 5; \
        size_t mask = ~(SIZE_MAX << LUT_SHIFT(lut->width)); \
        size_t i = mask & hash; \
        N##Item *item = &lut->buckets[i]; \
        for(;;) { \
            if(!item->key) break; \
            if(intend_to_set && item->hash == LUT_EMPTY) break; \
            if(item->hash == hash && !str_cmp(item->key, key)) return item; \
            i = mask & (i * 5 + perturb + 1); \
            /* get NEXT item */ \
            item = &lut->buckets[i]; \
        } \
        return item; \
    }

#define LUTS_IMPLEMENT_COMMON_FREE(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    void A##_free(N *lut) { \
        ASSERT_ARG(lut); \
        for(size_t i = 0; i < LUT_CAP(lut->width); ++i) { \
            N##Item *item = &lut->buckets[i]; \
            if(FK != 0) LUTS_TYPE_FREE(FK, item->val, TK); \
            if(FV != 0) LUTS_TYPE_FREE(FV, item->key, TV); \
            if(LUTS_IS_BY_REF(MK)) { free(item->val); } \
            if(LUTS_IS_BY_REF(MV)) { free(item->key); } \
        } \
        free(lut->buckets); \
        memset(lut, 0, sizeof(*lut)); \
    }

#if 0
#define LUTS_IMPLEMENT_COMMON_GROW(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    int A##_grow(N *lut, size_t width) { \
        ASSERT_ARG(lut); \
        ASSERT(width > lut->width, "expect larger new width then current"); \
        if(width <= lut->width) return -1; \
        if(width < LUT_WIDTH_MIN) width = LUT_WIDTH_MIN; \
        N new = {0}; \
    new.buckets = malloc(sizeof(*new.buckets) * LUT_CAP(width));
    if(!new.buckets) return -1;
    new.width = width;
    new.used = lut->used;
    memset(new.buckets, 0, sizeof(*new.buckets) * LUT_CAP(width));
    /* re-add values */
    for(size_t i = 0; i < LUT_CAP(lut->width); ++i) {
        LutItem *src = &lut->buckets[i];
        if(!src->key) continue;
        if(src->hash == LUT_EMPTY) {
            if(src->key) {
                /////str_free(src->key);
                free(src->key);
            }
            if(src->val) {
                /////str_free(src->val);
                free(src->val);
            }
            continue;
        }
        size_t hash = src->hash;
        LutItem *item = static_lut_get_item(&new, src->key, hash, true);
        item->key = src->key;
        item->val = src->val;
    }
    free(lut->buckets);
    /* assign new table */
    *lut = new;
    return 0;
}
#endif


#define LUTS_H
#endif

