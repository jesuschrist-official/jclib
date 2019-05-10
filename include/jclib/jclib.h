/*
 * jclib - the fabulous C Jesus Christ' library. Get in touch !
 */
#ifndef _jclib_h_
#define _jclib_h_


#define offsetof(st, m) __builtin_offsetof(st, m)


#define containerof(ptr, st, m) \
    ((st*)((char*)ptr) - offsetof(st, m))


#define __unused__  __attribute__((unused))


#endif
