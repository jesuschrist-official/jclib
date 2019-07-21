#include "jclib/jclib.h"
#include "jclib/test/test.h"


#define JCLIST_TYPE int
#define JCLIST_IMPLEMENTATION
#include "jclib/algorithm/list.h"


JCTEST("jclib/algorithm/list")


JCTEST_FUNC("list_init") {
    int_list_t list cleanup(int_list_wipe) = {0};
    JCTEST_ASSERT(int_list_init(&list) >= 0);
    JCTEST_ASSERT(int_list_length(&list) == 0);
}


JCTEST_FUNC("list_init_with") {
    int_list_t list cleanup(int_list_wipe) = {0};

    JCTEST_ASSERT(
        int_list_init_with(&list, (int[]){1, 2, 3, 4, 5}, 5) >= 0
    );
    JCTEST_ASSERT(int_list_length(&list) == 5);
    for (size_t i = 0; i < 5; i++) {
        JCTEST_ASSERT(int_list_get(&list, i) == i + 1);
    }
    JCTEST_ASSERT(int_list_get(&list, 5) == 0);
}


JCTEST_FUNC("list_find") {
    int_list_t list cleanup(int_list_wipe) = {0};
    int_list_init_with(&list, (int[]){1, 1, 2, 3, 5, 8}, 6);

    JCTEST_ASSERT(int_list_find(&list, 1) != NULL);
    JCTEST_ASSERT(int_list_find(&list, 5) != NULL);
    JCTEST_ASSERT(int_list_find(&list, 8) != NULL);
    JCTEST_ASSERT(int_list_find(&list, 10) == NULL);

    *int_list_find(&list, 8) = 9;
    JCTEST_ASSERT(int_list_find(&list, 8) == NULL);
    JCTEST_ASSERT(int_list_find(&list, 9) != NULL);
}


JCTEST_FUNC("list_prepend") {
    int_list_t list cleanup(int_list_wipe) = {0};

    int_list_init_with(&list, (int[]){1, 2, 3, 4, 5}, 5);
    int_list_prepend(&list, 0);
    int_list_prepend(&list, -1);
    JCTEST_ASSERT(
        int_list_eq_with(&list, (int[]){-1, 0, 1, 2, 3, 4, 5}, 7)
    );
    int_list_wipe(&list);
}


JCTEST_FUNC("list_append") {
    int_list_t list cleanup(int_list_wipe) = {0};

    JCTEST_ASSERT(
        int_list_init_with(&list, (int[]){1, 2, 3, 4, 5}, 5) >= 0
    );
    int_list_append(&list, 6);
    int_list_append(&list, 7);
    JCTEST_ASSERT(int_list_length(&list) == 7);
    JCTEST_ASSERT(int_list_get(&list, 5) == 6);
    JCTEST_ASSERT(int_list_get(&list, 6) == 7);
    JCTEST_ASSERT(int_list_get(&list, 7) == 0);
}


JCTEST_FUNC("list_insert") {
    int_list_t list cleanup(int_list_wipe) = {0};

    int_list_init_with(&list, (int[]){1, 2, 3, 4, 5}, 5);

    JCTEST_ASSERT(int_list_insert(&list, NULL, -1) < 0);

    JCTEST_ASSERT(int_list_insert(&list, int_list_find(&list, 5), 6) >= 0);
    JCTEST_ASSERT(int_list_eq_with(&list, (int[]){1, 2, 3, 4, 6, 5}, 6));

    JCTEST_ASSERT(int_list_insert(&list, int_list_find(&list, 1), 0) >= 0);
    JCTEST_ASSERT(int_list_eq_with(&list, (int[]){0, 1, 2, 3, 4, 6, 5}, 7));

    JCTEST_ASSERT(int_list_insert(&list, int_list_find(&list, 4), 9) >= 0);
    JCTEST_ASSERT(int_list_eq_with(&list, (int[]){0, 1, 2, 3, 9, 4, 6, 5}, 8));
}


JCTEST_END
