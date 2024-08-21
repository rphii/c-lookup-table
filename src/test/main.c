#include "lut.h"
#include "str.h"
#include "luts.h"

//LUTS_INCLUDE(TrKV, trkv, Str, BY_REF, Str, BY_REF);
//LUTS_INCLUDE(TvKV, tvkv, Str, BY_VAL, Str, BY_VAL);
//
//LUTS_IMPLEMENT(TrKV, trkv, Str, BY_REF, Str, BY_REF, str_hash, str_cmp, 0, 0);
//LUTS_IMPLEMENT(TvKV, tvkv, Str, BY_VAL, Str, BY_VAL, str_hash, str_cmp, 0, 0);

int main(void)
{

    Lut lut = {0};
    //lut_grow(&lut, 8);

    size_t i = 0;
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("hello"), &STR("world")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("abc"), &STR("1")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("def"), &STR("2")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("ghi"), &STR("3")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("jkl"), &STR("4")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("mno"), &STR("5")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("pqr"), &STR("6")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("stu"), &STR("7")));

    //lut_grow(&lut, 6);

    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("vwx"), &STR("8")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("yz"), &STR("9")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("!!!"), &STR("???")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("hello"), &STR("WORLD")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("???"), &STR("!!!")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("111"), &STR("one")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("222"), &STR("two")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("333"), &STR("three")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("444"), &STR("four")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("555"), &STR("five")));
    printf("set %zu: %i\n", i++, lut_set(&lut, &STR("666"), &STR("six")));

    //lut_del(&lut, &STR("hello"));

    printff("getting value : hello");
    Str *val = lut_get(&lut, &STR("hello"));
    printff("VALUE: [%.*s]", STR_F(val));

    printff("getting value : lmao");
    Str *val2 = lut_get(&lut, &STR("lmao"));
    printff("VALUE: [%.*s]", STR_F(val2));

    lut_free(&lut, false);
    return 0;
}

