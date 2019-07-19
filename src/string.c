#include "jclib/string.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


#define JCARRAY_TYPE char
#define JCARRAY_NAME string
#define JCARRAY_IMPLEMENTATION
#include "jclib/algorithm/array.h"


int string_from_s(string_t* str, const char* s) {
    size_t size = strlen(s);
    return string_from(str, s, s ? size + 1 : 0);
}


int string_fmt(string_t* str, const char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    if (string_init(str, len) < 0) {
        return -1;
    }

    va_start(args, fmt);
    vsnprintf(str->data, str->reserved, fmt, args);
    va_end(args);
    str->length = str->reserved;
    return 0;
}


bool string_eq(const string_t* a, const string_t* b) {
    return string_eq_with(a, STRING_ARG(*b));
}


int string_cat(string_t* str, const string_t* other) {
    if (str->length > 0) {
        // Remove last null byte
        str->length--;
    }
    if (string_appendm(str, ARRAY_ARG(*other)) < 0) {
        return -1;
    }
    string_append(str, 0);
    return 0;
}


#if 0
static char** _push_split(char** res, size_t* nres, char* split) {
    char** next_res = realloc(res, (*nres + 1) * sizeof(char*));
    if (!next_res) {
        return NULL;
    }
    res = next_res;
    res[*nres] = split;
    (*nres)++; 
    return res;
}


char** split(const char* str, const char* sep, size_t* nsplit) {
    char** res = NULL;
    size_t nres = 0;
    char* cur = NULL;
    size_t ncur = 0;

    while (*str) {
        if (!strcmp(str, sep)) {
            if (!cur) {
                cur = malloc(sizeof(char));
                *cur = '\0';
            }
            char** next_res = _push_split(res, &nres, cur);
            if (!next_res) {
                goto error;
            }
            res = next_res;
            cur = NULL;
            ncur = 0;
            do {
                str += strlen(sep);
            } while (!strcmp(str, sep));
        } else {
            char* next_cur = realloc(cur, (ncur + 2) * sizeof(char));
            if (!next_cur) {
                goto error;
            }
            cur = next_cur;
            cur[ncur] = *str;
            cur[ncur + 1] = '\0';
            ncur++;
            str++;
        }
    }

    if (cur) {
        char** next_res = _push_split(res, &nres, cur);
        if (!next_res) {
            goto error;
        }
        res = next_res;
    }

    *nsplit = nres;
    return res;

  error:
    return NULL;
}


void split_free(char** split, size_t nsplit) {
    for (size_t i = 0; i < nsplit; i++) {
        free(split[i]);
    }
    free(split);
}
#endif
