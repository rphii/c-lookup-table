#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "lutd_int.h"

int test_lut_int(size_t count)
{
    LutInt lut = {0};
    int err = 0;
    int *dump_val = 0;
    size_t *dump_counts = 0;
    size_t dump_count = 0;
    if(lut_int_init(&lut, 6)) goto error;
    for(size_t i = 0; i < count; i++) {
        int val = rand() % 256;
        lut_int_add(&lut, val);
    }
    if(lut_int_dump(&lut, &dump_val, &dump_counts, &dump_count)) goto error;
    printf("dumped %zu unique items\n", dump_count);
    size_t dump_count_total = 0;
    for(size_t i = 0; i < dump_count; i++) {
        printf("  %u was added %zu times\n", dump_val[i], dump_counts[i]);
        dump_count_total += dump_counts[i];
    } 
clean:
    lut_int_free(&lut);
    free(dump_val);
    free(dump_counts);
    return err;
error:
    err = -1;
    goto clean;
}

int main(void)
{
    srand(time(0));
    int result = test_lut_int(1000000UL);
    return result;
}


