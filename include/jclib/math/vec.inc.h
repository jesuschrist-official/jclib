#ifndef JCVEC_TYPE
    #error "JCVEC_TYPE undefined"
#endif


#ifndef JCVEC_DIM
    #error "JCVEC_TYPE undefined"
#endif
#if JCVEC_DIM == 1
    #error "You're not deserving my daddy's love"
#endif


#ifndef JCVEC_NAME
    #error "JCVEC_NAME undefined"
#endif


/* Helpers ----------------------------------------------------------------- */
#define __TCONCAT(x, y) x ## _ ## y
#define _TCONCAT(x, y) __TCONCAT(x, y)
#define TYPE JCVEC_TYPE
#define TYPEDEF _TCONCAT(JCVEC_NAME, t)
#define FUNC(_name) _TCONCAT(JCVEC_NAME, _name)


/* Prototype --------------------------------------------------------------- */
#ifdef JCVEC_PROTOTYPE


#include <stdbool.h>


typedef struct JCVEC_NAME {
    union {
        TYPE v[JCVEC_DIM];
        struct {
#if JCVEC_DIM >= 1
            TYPE x;
#endif
#if JCVEC_DIM >= 2
            TYPE y;
#endif
#if JCVEC_DIM >= 3
            TYPE z;
#endif
#if JCVEC_DIM >= 4
            TYPE w;
#endif
        };
    };
} TYPEDEF;


TYPEDEF FUNC(add)(TYPEDEF u, TYPEDEF v);


TYPEDEF FUNC(sub)(TYPEDEF u, TYPEDEF v);


TYPEDEF FUNC(mul)(TYPEDEF u, TYPE k);


TYPEDEF FUNC(div)(TYPEDEF u, TYPE k);


TYPE FUNC(dotp)(TYPEDEF u, TYPEDEF v);


TYPE FUNC(det)(TYPEDEF u, TYPEDEF v);


TYPE FUNC(sqlen)(TYPEDEF u);


double FUNC(len)(TYPEDEF u);


TYPEDEF FUNC(normalize)(TYPEDEF u);


bool FUNC(eq)(TYPEDEF u, TYPEDEF v);


#if JCVEC_DIM == 2
double FUNC(angle)(TYPEDEF u);


TYPEDEF FUNC(rot)(TYPEDEF u, double a);


#elif JCVEC_DIM == 3 || JCVEC_DIM == 4
#endif


#undef JCVEC_PROTOTYPE
#endif


/* Implementation ---------------------------------------------------------- */
#ifdef JCVEC_IMPLEMENTATION


#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "jclib/math/math.h"


TYPEDEF FUNC(add)(TYPEDEF u, TYPEDEF v) {
    TYPEDEF w;
    for (size_t i = 0; i < JCVEC_DIM; i++) {
        w.v[i] = u.v[i] + v.v[i];
    }
    return w;
}


TYPEDEF FUNC(sub)(TYPEDEF u, TYPEDEF v) {
    TYPEDEF w;
    for (size_t i = 0; i < JCVEC_DIM; i++) {
        w.v[i] = u.v[i] - v.v[i];
    }
    return w;
}


TYPEDEF FUNC(mul)(TYPEDEF u, TYPE k) {
    TYPEDEF w;
    for (size_t i = 0; i < JCVEC_DIM; i++) {
        w.v[i] = u.v[i] * k;
    }
    return w;
}


TYPEDEF FUNC(div)(TYPEDEF u, TYPE k) {
    TYPEDEF w;
    for (size_t i = 0; i < JCVEC_DIM; i++) {
        w.v[i] = u.v[i] / k;
    }
    return w;
}


TYPE FUNC(dotp)(TYPEDEF u, TYPEDEF v) {
    TYPE d = 0;
    for (size_t i = 0; i < JCVEC_DIM; i++) {
        d += u.v[i] * v.v[i];
    }
    return d;
}


TYPE FUNC(det)(TYPEDEF u, TYPEDEF v) {
#if JCVEC_DIM == 2
   return u.x * v.y - u.y * v.x;
#else
    assert(0); // Fuck
    return 0;
#endif
}


TYPE FUNC(sqlen)(TYPEDEF u) {
    return FUNC(dotp)(u, u);
}


double FUNC(len)(TYPEDEF u) {
    return sqrt(FUNC(sqlen)(u));
}


TYPEDEF FUNC(normalize)(TYPEDEF u) {
    TYPE len = FUNC(len)(u);
    TYPEDEF w;
    for (size_t i = 0; i < JCVEC_DIM; i++) {
        w.v[i] = u.v[i] / (double)len;
    }
    return w;
}


bool FUNC(eq)(TYPEDEF u, TYPEDEF v) {
    for (size_t i = 0; i < JCVEC_DIM; i++) {
#if JCVEC_TYPE == float || JCVEC_TYPE == double
        if (!APPROX_EQ(u.v[i], v.v[i])) {
            return false;
        }
#else
        if (u.v[i] != v.v[i]) {
            return false;
        }
#endif
    }
    return true;
}


#if JCVEC_DIM == 2
double FUNC(angle)(TYPEDEF u) {
    return atan2(u.y, u.x) - atan2(0, 1);
}


TYPEDEF FUNC(rot)(TYPEDEF u, double a) {
    return (TYPEDEF)VEC(
        u.x * cos(a) - u.y * sin(a),
        u.x * sin(a) + u.y * cos(a)
    );
}


#endif


#undef JCVEC_IMPLEMENTATION
#endif


/* Clean-up ---------------------------------------------------------------- */
#undef __TCONCAT
#undef _TCONCAT
#undef TYPE
#undef TYPEDEF
#undef FUNC
#undef JCVEC_TYPE
#undef JCVEC_NAME
#undef JCVEC_DIM


/* ------------------------------------------------------------------------- */
