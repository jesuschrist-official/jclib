/*
 * jclib - the fabulous C Jesus Christ' library. Get in touch !
 */
#ifndef _jclib_h_
#define _jclib_h_


#include <stdbool.h>


#define offsetof(st, m) __builtin_offsetof(st, m)


#define containerof(ptr, st, m) \
    ((st*)((char*)ptr) - offsetof(st, m))


#define countof(_array) \
    (sizeof(_array) / sizeof(_array[0]))


#define __unused__  __attribute__((unused))


#define swap(_a, _b) \
    { \
        typeof(_a) _jc_tmp_v = _a; \
        _a = _b; \
        _b = _jc_tmp_v; \
    }


/* Optionals --------------------------------------------------------------- */
#define opt(_type) struct opt ## _type


#define opt_typedef(_type) \
opt(_type) { \
    bool set; \
    _type v; \
}


#define opt_value(_v) { true, (_v) }


#define opt_none { .set = false }


#define opt_is_set(_opt) _opt.set


#define opt_get(_opt) \
    ({ \
        assert(_opt.set); \
        _opt.v; \
    })


#define opt_set(_opt, _v) \
    _opt = (typeof(_opt))opt_value(_v)


/* ------------------------------------------------------------------------- */
#endif
