#include "jclib/jclib.h"
#include "jclib/test/test.h"


#define JCARRAY_TYPE int
#define JCARRAY_IMPLEMENTATION
#include "jclib/algorithm/array.h"


JCTEST("jclib/algorithm/array")


JCTEST_FUNC("array_init_empty") {
    int_array_t array;
    JCTEST_ASSERT(int_array_init(&array, 0) >= 0);
    JCTEST_ASSERT(array.length == 0);
    JCTEST_ASSERT(array.reserved == 0);
    JCTEST_ASSERT(array.data == NULL);
    int_array_wipe(&array);
}


JCTEST_FUNC("array_init") {
    int_array_t array;
    JCTEST_ASSERT(int_array_init(&array, 10) >= 0);
    JCTEST_ASSERT(array.length == 0);
    JCTEST_ASSERT(array.reserved == 10);
    JCTEST_ASSERT(array.data != NULL);
    int_array_wipe(&array);
}


JCTEST_FUNC("array_init_with") {
    int_array_t array;
    JCTEST_ASSERT(int_array_init_with(&array, (int[]){1, 2, 3, 4}, 4) >= 0);
    JCTEST_ASSERT(array.length == 4);
    JCTEST_ASSERT(array.reserved >= 4);
    JCTEST_ASSERT(array.data != NULL);
    JCTEST_ASSERT(array.data[0] == 1);
    JCTEST_ASSERT(array.data[1] == 2);
    JCTEST_ASSERT(array.data[2] == 3);
    JCTEST_ASSERT(array.data[3] == 4);
    int_array_wipe(&array);
}


JCTEST_FUNC("array_length") {
    int_array_t array;
    JCTEST_ASSERT(int_array_init_with(&array, (int[]){1, 2, 3, 4}, 4) >= 0);
    JCTEST_ASSERT(int_array_length(&array) == 4);
    int_array_wipe(&array);
}


JCTEST_FUNC("array_resize") {
    int_array_t array;
    JCTEST_ASSERT(int_array_init(&array, 0) == 0);
    JCTEST_ASSERT(int_array_resize(&array) >= 0);
    JCTEST_ASSERT(array.reserved == 1);
    JCTEST_ASSERT(int_array_resize(&array) >= 0);
    JCTEST_ASSERT(array.reserved == 3);
    JCTEST_ASSERT(int_array_resize(&array) >= 0);
    JCTEST_ASSERT(array.reserved == 7);
    int_array_wipe(&array);
}


JCTEST_FUNC("array_rshift") {
    int_array_t array;
    int_array_init_with(&array, (int[]){1, 2, 3, 4, 5}, 5);

    int_array_rshift(&array, 2, 3);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){1, 2, 0, 0, 0, 3, 4, 5}, 8)
    );

    int_array_rshift(&array, 7, 2);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){1, 2, 0, 0, 0, 3, 4, 0, 0, 5}, 10)
    );

    int_array_rshift(&array, 0, 2);
    JCTEST_ASSERT(
        int_array_eq_with(
            &array, (int[]){0, 0, 1, 2, 0, 0, 0, 3, 4, 0, 0, 5}, 12
        )
    );

    int_array_wipe(&array);
}


JCTEST_FUNC("array_lshift") {
    int_array_t array;
    int_array_init_with(
        &array, (int[]){0, 0, 1, 2, 0, 0, 0, 3, 4, 0, 0, 5}, 12
    );

    int_array_lshift(&array, 0, 2);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){1, 2, 0, 0, 0, 3, 4, 0, 0, 5}, 10)
    );

    int_array_lshift(&array, 2, 3);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){1, 2, 3, 4, 0, 0, 5}, 7)
    );

    int_array_lshift(&array, 4, 2);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){1, 2, 3, 4, 5}, 5)
    );

    int_array_wipe(&array);
}


JCTEST_FUNC("array_prepend") {
    int_array_t array;
    int_array_init_with(&array, (int[]){1, 2, 3, 4, 5}, 5);

    int_array_prepend(&array, 0);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){0, 1, 2, 3, 4, 5}, 6)
    );

    int_array_prepend(&array, -1);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){-1, 0, 1, 2, 3, 4, 5}, 7)
    );

    int_array_wipe(&array);
}


JCTEST_FUNC("array_append") {
    int_array_t array;
    int_array_init_with(&array, (int[]){1, 2, 3, 4, 5}, 5);

    int_array_append(&array, 6);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){1, 2, 3, 4, 5, 6}, 6)
    );

    int_array_append(&array, 7);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){1, 2, 3, 4, 5, 6, 7}, 7)
    );

    int_array_wipe(&array);
}


JCTEST_FUNC("array_insert") {
    int_array_t array;
    int_array_init_with(&array, (int[]){1, 2, 3, 4, 5}, 5);

    int_array_insert(&array, 2, -1);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){1, 2, -1, 3, 4, 5}, 6)
    );

    int_array_insert(&array, 4, -2);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){1, 2, -1, 3, -2, 4, 5}, 7)
    );

    int_array_wipe(&array);
}


JCTEST_FUNC("array_remove") {
    int_array_t array;
    int_array_init_with(&array, (int[]){1, 2, 3, 4, 5}, 5);

    JCTEST_ASSERT(int_array_remove(&array, 0) == 1);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){2, 3, 4, 5}, 4)
    );

    JCTEST_ASSERT(int_array_remove(&array, 2) == 4);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){2, 3, 5}, 3)
    );

    JCTEST_ASSERT(int_array_remove(&array, 2) == 5);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){2, 3}, 2)
    );

    int_array_wipe(&array);
}


JCTEST_FUNC("array_pop_front") {
    int_array_t array;
    int_array_init_with(&array, (int[]){1, 2, 3, 4, 5}, 5);

    JCTEST_ASSERT(int_array_pop_front(&array) == 1);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){2, 3, 4, 5}, 4)
    );

    JCTEST_ASSERT(int_array_pop_front(&array) == 2);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){3, 4, 5}, 3)
    );

    int_array_wipe(&array);
}


JCTEST_FUNC("array_pop_back") {
    int_array_t array;
    int_array_init_with(&array, (int[]){1, 2, 3, 4, 5}, 5);

    JCTEST_ASSERT(int_array_pop_back(&array) == 5);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){1, 2, 3, 4}, 4)
    );

    JCTEST_ASSERT(int_array_pop_back(&array) == 4);
    JCTEST_ASSERT(
        int_array_eq_with(&array, (int[]){1, 2, 3}, 3)
    );

    int_array_wipe(&array);
}


JCTEST_FUNC("array_index_of") {
    int_array_t array;
    int_array_init_with(&array, (int[]){1, 2, 3, 4, 5}, 5);

    JCTEST_ASSERT(int_array_index_of(&array, 1) == 0);
    JCTEST_ASSERT(int_array_index_of(&array, 2) == 1);
    JCTEST_ASSERT(int_array_index_of(&array, 3) == 2);
    JCTEST_ASSERT(int_array_index_of(&array, 4) == 3);
    JCTEST_ASSERT(int_array_index_of(&array, 5) == 4);
    JCTEST_ASSERT(int_array_index_of(&array, 6) == -1);

    int_array_wipe(&array);
}


JCTEST_END
