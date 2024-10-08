#ifndef LUT_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define LUT_ITEM_BY_VAL(T)             T
#define LUT_ITEM_BY_REF(T)             T *
#define LUT_ITEM(T, M)                 LUT_ITEM_##M(T)

#define LUT_EMPTY                      SIZE_MAX
#define LUT_WIDTH_MIN                  3

#define LUT_SHIFT(width)               (width)
#define LUT_CAP(width)                 (!!width * (size_t)1ULL << LUT_SHIFT(width))


#define LUT_TYPE_FREE(F,X,T,M)         ((void (*)(LUT_ITEM(T,M)))(F))(X)

#define LUT_TYPE_CMP(C,A,B,T,M)        ((int (*)(const LUT_ITEM(T,M), const LUT_ITEM(T,M)))(C))(A, B)

#define LUT_PTR_BY_VAL   
#define LUT_PTR_BY_REF                 &
#define LUT_PTR(M)                     LUT_PTR_##M

#define LUT_REF_BY_VAL                 &
#define LUT_REF_BY_REF
#define LUT_REF(M)                     LUT_REF_##M

#define LUT_IS_BY_REF_BY_REF           1
#define LUT_IS_BY_REF_BY_VAL           0
#define LUT_IS_BY_REF(M)               LUT_IS_BY_REF_##M

#define LUT_ASSERT_ARG_M_BY_VAL(v)     do {} while(0)
#define LUT_ASSERT_ARG_M_BY_REF(v)     LUT_ASSERT_REAL(v, "expecting argument")
#define LUT_ASSERT_ARG_M(v, M)         LUT_ASSERT_ARG_M_##M(v)
#define LUT_ASSERT_ARG(v)              LUT_ASSERT_REAL(v, "expecting argument")
#define LUT_ASSERT_REAL(v, msg)        assert(v && msg)

#define LUT_INCLUDE(N, A, TK, MK, TV, MV) \
    typedef struct N##Item { \
        LUT_ITEM(TK, MK) key; \
        LUT_ITEM(TV, MV) val; \
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
    int A##_set(N *lut, LUT_ITEM(TK, MK) key, LUT_ITEM(TV, MV) val); \
    TV *A##_get(N *lut, LUT_ITEM(TK, MK) key); \
    void A##_del(N *lut, LUT_ITEM(TK, MK) key); \

#define LUT_IMPLEMENT(N, A, TK, MK, TV, MV, H, C, FK, FV)   \
    LUT_IMPLEMENT_COMMON_STATIC_GET_ITEM(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    LUT_IMPLEMENT_COMMON_FREE(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    LUT_IMPLEMENT_COMMON_GROW(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    LUT_IMPLEMENT_COMMON_SET(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    LUT_IMPLEMENT_COMMON_GET(N, A, TK, MK, TV, MV, H, C, FK, FV) \

#define LUT_IMPLEMENT_COMMON_STATIC_GET_ITEM(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    static N##Item **A##_static_get_item(N *lut, LUT_ITEM(TK, MK) key, size_t hash, bool intend_to_set) { \
        LUT_ASSERT_ARG(lut); \
        LUT_ASSERT_ARG_M(key, MK); \
        size_t perturb = hash >> 5; \
        size_t mask = ~(SIZE_MAX << LUT_SHIFT(lut->width)); \
        size_t i = mask & hash; \
        N##Item **item = &lut->buckets[i]; \
        for(;;) { \
            /*printff("  %zu", i);*/\
            if(!*item) break; \
            if(intend_to_set && (*item)->hash == LUT_EMPTY) break; \
            if((*item)->hash == hash) { \
                if(C != 0) { if(!LUT_TYPE_CMP(C, (*item)->key, key, TK, MK)) return item; } \
                else { if(!memcmp(item, LUT_REF(MK)key, sizeof(*LUT_REF(MK)key))) return item; } \
            } \
            perturb >>= 5; \
            i = mask & (i * 5 + perturb + 1); \
            /* get NEXT item */ \
            item = &lut->buckets[i]; \
        } \
        return item; \
    }

#define LUT_IMPLEMENT_COMMON_FREE(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    void A##_free(N *lut) { \
        ASSERT_ARG(lut); \
        for(size_t i = 0; i < LUT_CAP(lut->width); ++i) { \
            N##Item *item = lut->buckets[i]; \
            if(FK != 0) LUT_TYPE_FREE(FK, item->key, TK, MK); \
            if(FV != 0) LUT_TYPE_FREE(FV, item->val, TV, MV); \
            free(item); \
        } \
        free(lut->buckets); \
        memset(lut, 0, sizeof(*lut)); \
    }

#define LUT_IMPLEMENT_COMMON_GROW(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    int A##_grow(N *lut, size_t width) { \
        LUT_ASSERT_ARG(lut); \
        LUT_ASSERT_REAL(width > lut->width, "expect larger new width then current"); \
        if(width <= lut->width) return -1; \
        if(width < LUT_WIDTH_MIN) width = LUT_WIDTH_MIN; \
        /*printff("NEW WIDTH %zu", width);*/ \
        N grown = {0}; \
        grown.buckets = malloc(sizeof(grown.buckets) * LUT_CAP(width)); \
        if(!grown.buckets) return -1; \
        grown.width = width; \
        grown.used = lut->used; \
        memset(grown.buckets, 0, sizeof(grown.buckets) * LUT_CAP(width)); \
        /* re-add values */ \
        for(size_t i = 0; i < LUT_CAP(lut->width); ++i) { \
            N##Item *src = lut->buckets[i]; \
            if(!src) continue; \
            if(src->hash == LUT_EMPTY) { \
                if(src) { \
                    /*str_free(src->val); \
                    str_free(src->key); \
                    TODO: do this in del */ \
                    free(src); \
                } \
                continue; \
            } \
            size_t hash = src->hash; \
            N##Item **item = A##_static_get_item(&grown, src->key, hash, true); \
            *item = src; \
        } \
        free(lut->buckets); \
        /* assign grown table */ \
        *lut = grown; \
        return 0; \
    }

#define LUT_IMPLEMENT_COMMON_SET(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    int A##_set(N *lut, LUT_ITEM(TK, MK) key, LUT_ITEM(TV, MV) val) { \
        LUT_ASSERT_ARG(lut); \
        LUT_ASSERT_ARG_M(key, MK); \
        if(2 * lut->used >= LUT_CAP(lut->width)) { \
            if(A##_grow(lut, lut->width + 2)) return -1; \
        } \
        size_t hash = H(key); \
        N##Item **item = A##_static_get_item(lut, key, hash, true); \
        if(*item) { \
            /* free old key */ \
            if(FK != 0) LUT_TYPE_FREE(FK, (*item)->key, TK, MK); \
            if(FV != 0) LUT_TYPE_FREE(FV, (*item)->val, TV, MV); \
        } else { \
            size_t req = sizeof(**item); \
            if(LUT_IS_BY_REF(MK)) { \
                req += sizeof(*LUT_REF(MK)(*item)->key); \
            } \
            if(LUT_IS_BY_REF(MV)) { \
                req += sizeof(*LUT_REF(MK)(*item)->val); \
            } \
            *item = malloc(req); \
            memset(*item, 0, sizeof(**item)); \
            if(!*item) return -1; \
            if(LUT_IS_BY_REF(MK)) { \
                void *p = (void *)*item + sizeof(**item) + 0; \
                memset(p, 0, sizeof((*item)->key)); \
                memcpy(&(*item)->key, &p, sizeof((*item)->key)); \
            } \
            if(LUT_IS_BY_REF(MV)) { \
                void *p = (void *)*item + sizeof(**item) + sizeof(*LUT_REF(MK)(*item)->key); \
                memset(p, 0, sizeof((*item)->val)); \
                memcpy(&(*item)->val, &p, sizeof((*item)->val)); \
            } \
        } \
        memcpy(LUT_REF(MK)(*item)->key, LUT_REF(MK)key, sizeof(TK)); \
        memcpy(LUT_REF(MV)(*item)->val, LUT_REF(MV)val, sizeof(TV)); \
        (*item)->hash = hash; \
        ++lut->used; \
        return 0; \
    }

#define LUT_IMPLEMENT_COMMON_GET(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    TV *A##_get(N *lut, LUT_ITEM(TK, MK) key) { \
        LUT_ASSERT_ARG(lut); \
        LUT_ASSERT_ARG_M(key, MK); \
        size_t hash = H(key); \
        N##Item *item = *A##_static_get_item(lut, key, hash, false); \
        return item ? LUT_REF(MV)item->val : 0; \
    }

#define LUT_IMPLEMENT_COMMON_DEL(N, A, TK, MK, TV, MV, H, C, FK, FV) \
    void A##_del(N *lut, LUT_ITEM(TK, MK) key) { \
        LUT_ASSERT_ARG(lut); \
        LUT_ASSERT_ARG_M(key, TK); \
        size_t hash = H(key); \
        N##Item *item = *A##_static_get_item(lut, key, hash, true); \
        if(item) { \
            item->hash = LUT_EMPTY; \
            if(FK != 0) LUT_TYPE_FREE(FK, LUT_PTR(MK)item->key, TK, MK); \
            if(FV != 0) LUT_TYPE_FREE(FV, LUT_PTR(MV)item->val, TV, MV); \
        } \
    }



#define LUT_H
#endif

