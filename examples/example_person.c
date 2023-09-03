#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lutd_person.h"

static int test_person_by_ref(time_t seed, size_t count)
{
    srand(time(&seed));
    int err = 0;
    LutPersonR lut = {0};
    Person person = {0};
    Person **dump_val = 0;
    size_t *dump_counts = 0;
    size_t dump_count = 0;
    if(lut_person_r_init(&lut, 1)) goto error;
    for(size_t i = 0; i < count; i++) {
        if(person_random(&person)) goto error;
        person_print(&person);printf("\n");
        if(lut_person_r_add(&lut, &person)) goto error;
    }
    if(lut_person_r_dump(&lut, &dump_val, &dump_counts, &dump_count)) goto error;
    printf("dumped %zu unique items\n", dump_count);
    size_t dump_count_total = 0;
    for(size_t i = 0; i < dump_count; i++) {
        Person *p_i = dump_val[i];
        printf("  ");
        person_print(p_i);
        printf(" was added %zu times\n", dump_counts[i]);
        dump_count_total += dump_counts[i];
    } 
clean:
    lut_person_r_free(&lut);
    free(dump_val);
    free(dump_counts);
    return err;
error:
    person_free(&person); /* reallocation might have failed somewhere..... */
    err = -1;
    goto clean;
}

static int test_person_by_val(time_t seed, size_t count)
{
    srand(time(&seed));
    int err = 0;
    LutPersonV lut = {0};
    Person person = {0};
    Person *dump_val = 0;
    size_t *dump_counts = 0;
    size_t dump_count = 0;
    if(lut_person_v_init(&lut, 8)) goto error;
    for(size_t i = 0; i < count; i++) {
        if(person_random(&person)) goto error;
        person_print(&person);printf("\n");
        if(lut_person_v_add(&lut, person)) goto error;
    }
    if(lut_person_v_dump(&lut, &dump_val, &dump_counts, &dump_count)) goto error;
    printf("dumped %zu unique items\n", dump_count);
    size_t dump_count_total = 0;
    for(size_t i = 0; i < dump_count; i++) {
        Person p_i = dump_val[i];
        printf("  ");
        person_print(&p_i);
        printf(" was added %zu times\n", dump_counts[i]);
        dump_count_total += dump_counts[i];
    } 
clean:
    lut_person_v_free(&lut);
    free(dump_val);
    free(dump_counts);
    return err;
error:
    person_free(&person); /* reallocation might have failed somewhere..... */
    err = -1;
    goto clean;
}



int main(void)
{
    time_t seed = time(0);
    test_person_by_val(seed, 10);
    test_person_by_ref(seed, 10);
    int result = 0;
    return result;
}


