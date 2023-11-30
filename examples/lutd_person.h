#ifndef LUTD_PERSON_H

#include <stdbool.h>
#include <stdarg.h>
#include <time.h>

#include "person.h"
#include "../src/lutd.h"

LUTD_INCLUDE(LutPersonR, lut_person_r, Person, BY_REF)
LUTD_INCLUDE(LutPersonV, lut_person_v, Person, BY_VAL)

#define LUTD_PERSON_H
#endif

