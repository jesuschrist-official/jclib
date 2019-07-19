#include "jclib/string.h"
#include "jclib/test/test.h"


JCTEST("jclib/algorithm/array")


JCTEST_FUNC("test") {
    string_t s;
    string_fmt(&s, "%s: %d %f\n", "blablah", 32, 0.5);
    JCTEST_ASSERT(string_eq(&s, &STRING("blablah: 32 0.5")));
    string_wipe(&s);
}


JCTEST_END
