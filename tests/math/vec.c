#include <math.h>
#include <stdint.h>

#include "jclib/jclib.h"
#include "jclib/test/test.h"
#include "jclib/math/math.h"
#include "jclib/math/vec.h"


JCTEST("jclib/math/vec")


/* vec2f ------------------------------------------------------------------- */
JCTEST_FUNC("vec2f") {
    vec2f_t u = VEC(2, 3);
    JCTEST_ASSERT(u.v[0] == 2);
    JCTEST_ASSERT(u.v[1] == 3);
    JCTEST_ASSERT(u.v[0] == u.x);
    JCTEST_ASSERT(u.v[1] == u.y);
}


JCTEST_FUNC("vec2f_add") {
    vec2f_t u = VEC(2, 3);
    vec2f_t v = VEC(5, 7);
    vec2f_t w = vec2f_add(u, v);
    JCTEST_ASSERT(w.x = u.x + v.x);
    JCTEST_ASSERT(w.y = u.y + v.y);
}


JCTEST_FUNC("vec2f_sub") {
    vec2f_t u = VEC(2, 3);
    vec2f_t v = VEC(5, 7);
    vec2f_t w = vec2f_sub(u, v);
    JCTEST_ASSERT(w.x = u.x - v.x);
    JCTEST_ASSERT(w.y = u.y - v.y);
}


JCTEST_FUNC("vec2f_mul") {
    vec2f_t u = VEC(2, 3);
    vec2f_t w = vec2f_mul(u, 2.3);
    JCTEST_ASSERT(w.x = u.x * 2.3);
    JCTEST_ASSERT(w.y = u.y * 2.3);
}


JCTEST_FUNC("vec2f_div") {
    vec2f_t u = VEC(2, 3);
    vec2f_t w = vec2f_div(u, 2.3);
    JCTEST_ASSERT(w.x = u.x / 2.3);
    JCTEST_ASSERT(w.y = u.y / 2.3);
}


JCTEST_FUNC("vec2f_dotp") {
    vec2f_t u = VEC(2, 3);
    vec2f_t v = VEC(5, 7);
    float r = vec2f_dotp(u, v);
    JCTEST_ASSERT(r == 2 * 5 + 3 * 7);
}


JCTEST_FUNC("vec2f_det") {
    vec2f_t u = VEC(2, 3);
    vec2f_t v = VEC(5, 7);
    float r = vec2f_det(u, v);
    JCTEST_ASSERT(r == 2 * 7 - 3 * 5);
}


JCTEST_FUNC("vec2f_sqlen") {
    vec2f_t u = VEC(2, 3);
    float r = vec2f_sqlen(u);
    JCTEST_ASSERT(APPROX_EQ(r, 2.0f * 2.0f + 3.0f * 3.0f));
}


JCTEST_FUNC("vec2f_len") {
    vec2f_t u = VEC(2, 3);
    float r = vec2f_len(u);
    JCTEST_ASSERT(APPROX_EQ(r, sqrt(2 * 2 + 3 * 3)));
}


JCTEST_FUNC("vec2f_normalize") {
    vec2f_t u = VEC(2, 3);
    vec2f_t v = vec2f_normalize(u);
    JCTEST_ASSERT(APPROX_EQ(vec2f_len(v), 1.0f));
    JCTEST_ASSERT(APPROX_EQ(v.x, 2.0f / vec2f_len(u)));
    JCTEST_ASSERT(APPROX_EQ(v.y, 3.0f / vec2f_len(u)));
}


JCTEST_FUNC("vec2f_angle") {
    JCTEST_ASSERT(
        ANGLE_EQ(vec2f_angle((vec2f_t)VEC(1.0f, 0.0f)), 0.0f)
    );
    JCTEST_ASSERT(
        ANGLE_EQ(vec2f_angle((vec2f_t)VEC(1.0f, 1.0f)), 0.25f * M_PI)
    );
    JCTEST_ASSERT(
        ANGLE_EQ(vec2f_angle((vec2f_t)VEC(0.0f, 1.0f)), 0.5f * M_PI)
    );
    JCTEST_ASSERT(
        ANGLE_EQ(vec2f_angle((vec2f_t)VEC(-1.0f, 1.0f)), 0.75f * M_PI)
    );
    JCTEST_ASSERT(
        ANGLE_EQ(vec2f_angle((vec2f_t)VEC(-1.0f, 0.0f)), 1.0f * M_PI)
    );
    JCTEST_ASSERT(
        ANGLE_EQ(vec2f_angle((vec2f_t)VEC(-1.0f, -1.0f)), 1.25f * M_PI)
    );
    JCTEST_ASSERT(
        ANGLE_EQ(vec2f_angle((vec2f_t)VEC(-0.0f, -1.0f)), 1.5f * M_PI)
    );
    JCTEST_ASSERT(
        ANGLE_EQ(vec2f_angle((vec2f_t)VEC(1.0f, -1.0f)), 1.75f * M_PI)
    );
}


JCTEST_FUNC("vec2f_rot") {
    float values[] = {
        1.0f, 0.0f, 0.0f,         1.0f, 0.0f,
        1.0f, 0.0f, M_PI * 0.25f, 0.707107f, 0.707107f,
        1.0f, 0.0f, M_PI * 0.5f,  0.0f, 1.0f,
        1.0f, 0.0f, M_PI * 0.75f, -0.707107f, 0.707107f,
        1.0f, 0.0f, M_PI * 1.0f,  -1.0f, 0.0f,
        1.0f, 0.0f, M_PI * 1.25f, -0.707107f, -0.707107f,
        1.0f, 0.0f, M_PI * 1.5f,  0.0f, -1.0f,
        1.0f, 0.0f, M_PI * 1.25f, -0.707107f, -0.707107f,
        1.0f, 0.0f, M_PI * 1.75f, 0.707107f, -0.707107f,
        1.0f, 0.0f, M_PI * 2.0f,  1.0f, 0.0f,
    };

    for (size_t i = 0; i < countof(values); i += 5) {
        JCTEST_ASSERT(
            vec2f_eq(
                vec2f_rot(
                    (vec2f_t)VEC(values[i], values[i + 1]), values[i + 2]
                ),
                (vec2f_t)VEC(values[i + 3], values[i + 4])
            )
        );
    }
}


/* vec3f ------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */


JCTEST_END
