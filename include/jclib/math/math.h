/*
 * jclib/math/math.h - Jesus Christ's math library.
 */
#ifndef _jclib_math_math_h_
#define _jclib_math_math_h_


#include <math.h>


#define MAX(_a, _b) (((_a) > (_b)) ? (_a) : (_b))
#define MIN(_a, _b) (((_a) < (_b)) ? (_a) : (_b))


/*
 * Floating point comparison material
 * https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
 */
#define EPSILON 1E-6


/*
 * As randomascii says it, there is no universal way to compare for float
 * equality. Daddy has done its best, but, as he often says "you cannot store
 * unlimited data in limited memory". So, here is a rough solution to solve
 * the float comparison problem.
 */
#define APPROX_EQ(_a, _b) \
    (fabs((_a) - (_b)) <= EPSILON)


/*
 * Normalize a radiant angle between [0, 2 * PI[
 */
#define ANGLE(_a) \
    ({ \
        double _tmpangle = fmod((_a), 2 * M_PI); \
        if (_tmpangle < 0) _tmpangle += 2 * M_PI; \
        _tmpangle; \
    })


/*
 * Check for radiant angles equality
 */
#define ANGLE_EQ(_a, _b) \
    APPROX_EQ(ANGLE(_a), ANGLE(_b))


#endif
