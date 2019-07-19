#ifndef _jclib_string_h_
#define _jclib_string_h_


#include <stdbool.h>
#include <stdlib.h>


#define JCARRAY_TYPE char
#define JCARRAY_NAME string
#define JCARRAY_PROTOTYPE
#include "jclib/algorithm/array.h"


#define STRING_ARG(_str) (_str).data, (_str).length


#define STRING(_str) \
    (string_t){ \
        .data = _str, \
        .length = strlen(_str), \
        .reserved = 0 \
    }


int string_from_s(string_t* str, const char* s);


int string_fmt(string_t* str, const char* fmt, ...);


int string_cat(string_t* str, const string_t* other);


bool string_eq(const string_t* a, const string_t* b);


#endif
