/*
 * jclib - the fabulous C Jesus Christ' library. Get in touch !
 */
#ifndef _jclib_h_
#define _jclib_h_


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


#endif
