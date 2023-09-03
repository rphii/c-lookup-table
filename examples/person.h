#ifndef PERSON_H

#include <stdbool.h>
#include <stdarg.h>
#include <time.h>

#define STR_DEFAULT_SIZE    8

typedef struct Str {
    char *s;    /* string */
    size_t l;   /* length */
    size_t c;   /* capacity */
} Str;

int str_app(Str *str, char *format, ...);
int str_cap_ensure(Str *str, size_t cap);
void str_free_single(Str *str);

typedef struct Person {
    int age;
    Str name;
} Person;

void person_print(Person *person);
int person_random(Person *person);
void person_free(Person *person);

#define PERSON_H
#endif


