#include "lutd_person.h"

static inline size_t lut_person_r_hash(Person *a)
{
    size_t hash = 5381;
    for(size_t i = 0; i < a->name.l; i++) {
        char c = a->name.s[i];
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

static inline int lut_person_r_cmp(Person *a, Person *b)
{
    if(!a || !b) return -1;
    //printf("COMPARE ");
    //person_print(a);
    //person_print(b);
    //printf("\n");
    if(a->name.l != b->name.l) return -1;
    if(!a->name.s || !b->name.s) return -1;
    return memcmp(a->name.s, b->name.s, a->name.l);
}

LUTD_IMPLEMENT(LutPersonR, lut_person_r, Person, BY_REF, \
        lut_person_r_hash, lut_person_r_cmp, person_free)

static inline size_t lut_person_v_hash(Person a)
{
    size_t hash = 5381;
    for(size_t i = 0; i < a.name.l; i++) {
        char c = a.name.s[i];
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

static inline int lut_person_v_cmp(Person a, Person b)
{
    if(a.name.l != b.name.l) return -1;
    return memcmp(a.name.s, b.name.s, a.name.l);
}

LUTD_IMPLEMENT(LutPersonV, lut_person_v, Person, BY_VAL, \
        lut_person_v_hash, lut_person_v_cmp, person_free)

