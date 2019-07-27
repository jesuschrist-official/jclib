/*
 * jclib/math/vec.c - Vectors
 *
 * Vectors code is generated with the magic header "jclib/math/vec.inc.h"
 * That's allow to not have to decline the same code times and times.
 * See that file for some "documentation".
 */
#ifndef _jclib_math_vec_h_
#define _jclib_math_vec_h_


#define VEC(...) { .v = { __VA_ARGS__ } }


#define JCVEC_TYPE float
#define JCVEC_DIM 2
#define JCVEC_NAME vec2f
#define JCVEC_PROTOTYPE
#include "jclib/math/vec.inc.h"


#define JCVEC_TYPE float
#define JCVEC_DIM 3
#define JCVEC_NAME vec3f
#define JCVEC_PROTOTYPE
#include "jclib/math/vec.inc.h"


#endif
