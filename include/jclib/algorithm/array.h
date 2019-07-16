/*
 * jclib/algorithm/array.h - Dynamic array generation file
 */
// Checks ---------------------------------------------------------------------
#ifndef JCARRAY_TYPE
    #error "JCARRAY_TYPE undefined"
#endif


// Helpers --------------------------------------------------------------------
#define __TCONCAT(x, y) x ## _ ## y
#define _TCONCAT(x, y) __TCONCAT(x, y)
#define TYPE JCARRAY_TYPE
#ifndef JCARRAY_NAME
    #define TYPENAME _TCONCAT(TYPE, array)
#else
    #define TYPENAME JCARRAY_NAME
#endif
#define TYPEDEF _TCONCAT(TYPENAME, t)
#define FUNC(_name) _TCONCAT(TYPENAME, _name)


// Common macros --------------------------------------------------------------
#ifndef JCARRAY_COMMON
#define JCARRAY_COMMON


#define ARRAY_ARG(_a) (_a).data, (_a).length


#endif


// Prototype ------------------------------------------------------------------
#ifdef JCARRAY_PROTOTYPE
#undef JCARRAY_PROTOTYPE
#include <stddef.h>


typedef struct TYPENAME {
    TYPE* data;
    size_t length;
    size_t reserved;
} TYPEDEF;


int FUNC(init)(TYPEDEF* array, size_t reserved);


TYPEDEF* FUNC(wipe)(TYPEDEF* array);


int FUNC(from)(TYPEDEF* array, const TYPE* data, size_t count);


size_t FUNC(length)(const TYPEDEF* array);


TYPE FUNC(get)(const TYPEDEF* array, size_t i);


TYPE* FUNC(getp)(TYPEDEF* array, size_t i);


const TYPE* FUNC(cgetp)(const TYPEDEF* array, size_t i);


int FUNC(eq_with)(const TYPEDEF* array, const TYPE* values, size_t count);


void FUNC(set)(TYPEDEF* array, size_t i, TYPE v);


int FUNC(prepend)(TYPEDEF* array, TYPE v);


int FUNC(append)(TYPEDEF* array, TYPE v);


int FUNC(insert)(TYPEDEF* array, size_t i, TYPE v);


TYPE FUNC(remove)(TYPEDEF* array, size_t i);


TYPE FUNC(pop_back)(TYPEDEF* array);


TYPE FUNC(pop_front)(TYPEDEF* array);


int FUNC(index_of)(const TYPEDEF* array, TYPE v);


#endif


// Implementation -------------------------------------------------------------
#ifdef JCARRAY_IMPLEMENTATION
#undef JCARRAY_IMPLEMENTATION


#ifndef JCARRAY_EQ
    #define JCARRAY_EQ(_a, _b) (!memcmp(&_a, &_b, sizeof(JCARRAY_TYPE)))
#endif


#ifndef JCARRAY_CMP
    #define JCARRAY_CMP(_a, _b) (memcmp(&_a, &_b, sizeof(JCARRAY_TYPE)))
#endif


#ifndef JCARRAY_WIPE
    #define JCARRAY_WIPE(_a)
#endif


#ifndef JCARRAY_COPY
    #define JCARRAY_COPY(_a, _b) (_a = _b)
#endif


#include <assert.h>
#include <stdlib.h>
#include <string.h>


#include "jclib/jclib.h"


int FUNC(init)(TYPEDEF* array, size_t reserved) {
    array->length = 0;
    array->reserved = reserved;
    array->data = malloc(reserved * sizeof(TYPE));
    if (reserved > 0 && !array->data) {
        return -1;
    }
    return 0;
}


TYPEDEF* FUNC(wipe)(TYPEDEF* array) {
    free(array->data); 
    return array;
}


int FUNC(from)(TYPEDEF* array, const TYPE* data, size_t count) {
    if (FUNC(init)(array, count) < 0) {
        return -1;
    }
    memcpy(array->data, data, count * sizeof(TYPE));
    array->length = count;
    return 0;
}


size_t FUNC(length)(const TYPEDEF* array) {
    return array->length;
}


TYPE FUNC(get)(const TYPEDEF* array, size_t i) {
    assert(array->length > i);
    return array->data[i];
}


TYPE* FUNC(getp)(TYPEDEF* array, size_t i) {
    assert(array->length > i);
    return array->data + i;
}


const TYPE* FUNC(cgetp)(const TYPEDEF* array, size_t i) {
    assert(array->length > i);
    return (const TYPE*)(array->data + i);
}


int FUNC(eq_with)(const TYPEDEF* array, const TYPE* values, size_t count) {
    if (array->length != count) {
        return 0;
    }
    return !memcmp(array->data, values, count * sizeof(TYPE));
}


void FUNC(set)(TYPEDEF* array, size_t i, TYPE v) {
    assert(array->length > i);
    array->data[i] = v;
}


static int FUNC(resize)(TYPEDEF* array) {
    size_t new_size = array->reserved * 2 + 1;
    TYPE* new_data = realloc(array->data, new_size * sizeof(TYPE));
    if (!new_data) {
        return -1;
    }
    array->data = new_data;
    array->reserved = new_size;
    return 0;
}


static int FUNC(rshift)(TYPEDEF* array, size_t idx, size_t size) {
    assert(array->length > idx);
    while (array->reserved < array->length + size) {
        if (FUNC(resize)(array) < 0) {
            return -1;
        }
    }
    for (size_t i = array->length + size - 1; i >= idx + size; i--) {
        array->data[i] = array->data[i - size];
        array->data[i - size] = 0;
    }
    array->length += size;
    return 0;
}


static void FUNC(lshift)(TYPEDEF* array, size_t idx, size_t size) {
    assert(array->length >= idx + size);
    for (size_t i = idx; i < array->length - size; i++) {
        array->data[i] = array->data[i + size];
        array->data[i + size] = 0;
    }
    array->length -= size;
}


int FUNC(prepend)(TYPEDEF* array, TYPE v) {
    if (FUNC(rshift)(array, 0, 1) < 0) {
        return -1;
    }
    array->data[0] = v;
    return 0;
}


int FUNC(append)(TYPEDEF* array, TYPE v) {
    if (array->length == array->reserved) {
        if (FUNC(resize)(array) < 0) {
            return -1;
        }
    }
    array->data[array->length++] = v;
    return 0;
}


int FUNC(insert)(TYPEDEF* array, size_t i, TYPE v) {
    if (FUNC(rshift)(array, i, 1) < 0) {
        return -1;
    }
    array->data[i] = v;
    return 0;
}


TYPE FUNC(remove)(TYPEDEF* array, size_t i) {
    TYPE v = FUNC(get)(array, i);
    FUNC(lshift)(array, i, 1);
    return v;
}


TYPE FUNC(pop_back)(TYPEDEF* array) {
    return FUNC(remove)(array, array->length - 1);
}


TYPE FUNC(pop_front)(TYPEDEF* array) {
    return FUNC(remove)(array, 0);
}


int FUNC(index_of)(const TYPEDEF* array, TYPE v) {
    for (size_t i = 0; i < array->length; i++) {
        if (JCARRAY_EQ(v, array->data[i])) {
            return i;
        }
    }
    return -1;
}


#undef JCARRAY_IMPLEMENTATION
#endif


// Cleanup --------------------------------------------------------------------
#undef JCARRAY_TYPE
#undef JCARRAY_EQ
#undef JCARRAY_WIPE
#undef JCARRAY_COPY
#undef JCARRAY_NAME
#undef __TCONCAT
#undef _TCONCAT
#undef TYPE
#undef TYPENAME
#undef TYPEDEF
#undef FUNC


// ----------------------------------------------------------------------------
