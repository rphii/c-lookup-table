#include "lut.h"

static LutItem **static_lut_get_item(Lut *lut, Str *key, size_t hash, bool intend_to_set)
{
    ASSERT_ARG(lut);
    ASSERT_ARG(key);
    size_t perturb = hash >> 5;
    size_t mask = ~(SIZE_MAX << LUT_SHIFT(lut->width));
    size_t i = mask & hash;
    LutItem **item = &lut->buckets[i];
    size_t n = 0;
    for(;;) {
        //printff("  n = %2zu  i = %2zi  [%.*s] [%.*s]", n, i, STR_F(&item->key), STR_F(&item->val));
        if(!*item) break;
        if(intend_to_set && (*item)->hash == LUT_EMPTY) break;
        if((*item)->hash == hash && !str_cmp(&(*item)->key, key)) return item;
        i = mask & (i * 5 + perturb + 1);
        /* get NEXT item */
        item = &lut->buckets[i];
        /* check against full table */
        ++n;
        //if(++n >= LUT_CAP(lut->width)) return 0;
    }
    //if(intend_to_set) {
        return item;
    //} else {
        //return 0;
    //}
}

void lut_free(Lut *lut, bool sub)
{
    ASSERT_ARG(lut);
    for(size_t i = 0; i < LUT_CAP(lut->width); ++i) {
        LutItem *item = lut->buckets[i];
        if(sub) {
            str_free(&item->val);
            str_free(&item->key);
        }
        free(item);
    }
    free(lut->buckets);
    memset(lut, 0, sizeof(*lut));
}

int lut_grow(Lut *lut, size_t width)
{
    ASSERT_ARG(lut);
    ASSERT(width > lut->width, "expect larger new width then current");
    if(width <= lut->width) return -1;
    if(width < LUT_WIDTH_MIN) width = LUT_WIDTH_MIN;
    printff("NEW WIDTH %zu", width);
    Lut new = {0};
    new.buckets = malloc(sizeof(new.buckets) * LUT_CAP(width));
    if(!new.buckets) return -1;
    new.width = width;
    new.used = lut->used;
    memset(new.buckets, 0, sizeof(new.buckets) * LUT_CAP(width));
    /* re-add values */
    for(size_t i = 0; i < LUT_CAP(lut->width); ++i) {
        LutItem *src = lut->buckets[i];
        if(!src) continue;
        if(src->hash == LUT_EMPTY) {
            if(src) {
                /////str_free(src->val);
                /////str_free(src->key);
                free(src);
            }
            continue;
        }
        size_t hash = src->hash;
        LutItem **item = static_lut_get_item(&new, &src->key, hash, true);
        *item = src;
        //ASSERT(item, ERR_UNREACHABLE);
        //item->key = src->key;
        //item->val = src->val;
    }
    free(lut->buckets);
    /* assign new table */
    *lut = new;
    return 0;
}

int lut_set(Lut *lut, Str *key, Str *val)
{
    ASSERT_ARG(lut);
    ASSERT_ARG(key);
    if(2 * lut->used >= LUT_CAP(lut->width)) {
        printff("GROW, since 2*%zu >= %zu!", lut->used, LUT_CAP(lut->width));
        if(lut_grow(lut, lut->width + 2)) return -1;
        printff("  grown!");
        //return -1;
    }
    size_t hash = str_hash(key);
    //printff("SETTING %.*s", STR_F(key));
    LutItem **item = static_lut_get_item(lut, key, hash, true);
    printff("  get item");
    //if(!item) return -1;
    printff("  got item");
    if(*item) {
        /* FREE OLD KEY */
    } else {
        *item = malloc(sizeof(**item));
        if(!*item) return -1;
    }
    (*item)->key = *key;
    (*item)->val = *val;
    (*item)->hash = hash;
    ++lut->used;
    return 0;
}

Str *lut_get(Lut *lut, Str *key)
{
    ASSERT_ARG(lut);
    ASSERT_ARG(key);
#if 1
    size_t hash = str_hash(key);
    LutItem *item = *static_lut_get_item(lut, key, hash, false);
#else
    size_t hash = str_hash(key);
    size_t perturb = hash >> 5;
    size_t mask = ~(SIZE_MAX << LUT_SHIFT(lut->width));
    size_t i = mask & hash;
    LutItem *item = &lut->buckets[i];
    for(;;) { //size_t i = hash; lut->buckets[i].hash != hash; i = i * 5 + 1 + perturb) {
        if(!item->key) break;
        /////if(item->hash == LUT_EMPTY) break;
        if(item->hash == hash && !str_cmp(item->key, key)) {
            return item->val;
        }
        i = mask & (i * 5 + perturb + 1);
        item = &lut->buckets[i];
    }
    return 0;
#endif
    //printff("item %p", item);
    return item ? &item->val : 0;
}

void lut_del(Lut *lut, Str *key)
{
    ASSERT_ARG(lut);
    ASSERT_ARG(key);
#if 1
    size_t hash = str_hash(key);
    LutItem *item = *static_lut_get_item(lut, key, hash, true);
    if(item) {
        item->hash = LUT_EMPTY;
    }
#else
    size_t hash = str_hash(key);
    size_t perturb = hash >> 5;
    size_t mask = ~(SIZE_MAX << LUT_SHIFT(lut->width));
    size_t i = mask & hash;
    LutItem *item = &lut->buckets[i];
    for(;;) { //size_t i = hash; lut->buckets[i].hash != hash; i = i * 5 + 1 + perturb) {
        if(!item->key) break;
        /////if(item->hash == LUT_EMPTY) break;
        if(item->hash == hash && !str_cmp(item->key, key)) {
            item->hash = LUT_EMPTY;
            break;
        }
        i = mask & (i * 5 + perturb + 1);
        item = &lut->buckets[i];
    }
#endif
}



