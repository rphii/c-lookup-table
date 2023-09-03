#include "person.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void person_print(Person *person)
{
    assert(person);
    printf("age: %u, name: %.*s", person->age, (int)person->name.l, person->name.s);
}

int person_random(Person *person)
{
    assert(person);
    int result = 0;
    memset(person, 0, sizeof(*person));
    person->age = rand() % 100;
    size_t name_len = rand() % 10 + 3;
    for(size_t i = 0; i < name_len; i++) {
        result |= result ?: str_app(&person->name, "%c", rand() % ('Z' - 'A') + 'A');
    }
    return result;
}

void person_free(Person *person)
{
    if(!person) return;
    str_free_single(&person->name);
    memset(person, 0, sizeof(*person));
}


int str_app(Str *str, char *format, ...)
{
    if(!str) goto error;
    if(!format) goto error;
    // calculate length of append string
    va_list argp;
    va_start(argp, format);
    size_t len_app = (size_t)vsnprintf(0, 0, format, argp);
    if((int)len_app < 0) {
        goto error;
    }
    va_end(argp);
    // calculate required memory
    size_t len_new = str->l + len_app + 1;
    size_t required = str->c ? str->c : STR_DEFAULT_SIZE;
    while(required < len_new) required = required << 1;
    // make sure to have enough memory
    if(required > str->c)
    {
        char *temp = realloc(str->s, required);
        // safety check
        // apply address and set new allocd
        if(!temp) goto error;
        str->s = temp;
        str->c = required;
    }
    // actual append
    va_start(argp, format);
    int len_chng = vsnprintf(&(str->s)[str->l], len_app + 1, format, argp);
    va_end(argp);
    // check for success
    if(len_chng >= 0 && (size_t)len_chng <= len_app) {
        str->l += (size_t)len_chng; // successful, change length
    } else {
        goto error;
    }
    return 0;
error:
    return -1;
}

int str_cap_ensure(Str *str, size_t cap)
{
    assert(str);
    if(cap) {
        /* calculate required memory */
        size_t required = str->c ? str->c : STR_DEFAULT_SIZE;
        while(required < cap) required *= 2;
        /* only increase capacity */
        if(required > str->c) {
            void *temp = realloc(str->s, sizeof(*str->s) * cap);
            if(!temp) return -1;
            str->s = temp;
            str->c = required;
        }
    }
    return 0;
}

void str_free_single(Str *str)
{
    if(!str) return;
    free(str->s);
    str->s = 0;
    str->c = 0;
    str->l = 0;
}



