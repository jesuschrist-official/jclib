#include "jclib/jclib.h"
#include "jclib/test/test.h"


#define JCRBTREE_TYPE int
#define JCRBTREE_CMP(_a, _b) (_a - _b)
#define JCRBTREE_IMPLEMENTATION
#include "jclib/algorithm/rbtree.h"


struct test {
    char key[256];
    int  value;
};

#define JCRBTREE_NAME test_map
#define JCRBTREE_TYPE struct test
#define JCRBTREE_CMP(_a, _b) strcmp(_a.key, _b.key)
#define JCRBTREE_IMPLEMENTATION
#include "jclib/algorithm/rbtree.h"


JCTEST("jclib/algorithm/rbtree")


JCTEST_FUNC("rbtree_new") {
    int_rbtree_t* root = int_rbtree_new(5);
    JCTEST_ASSERT(root->v == 5);
    JCTEST_ASSERT(root->p == NULL);
    JCTEST_ASSERT(root->l == NULL);
    JCTEST_ASSERT(root->r == NULL);
    int_rbtree_delete(root);
}


JCTEST_FUNC("rbtree_rotate_left") {
    // Right is a leaf, no left
    {
        int_rbtree_t* root = int_rbtree_new(0);
        int_rbtree_link_right(root, int_rbtree_new(2));
        int_rbtree_check_integrity(root);

        int_rbtree_rotate_left(root);
        int_rbtree_check_integrity(root);

        JCTEST_ASSERT(root->v == 2);
        JCTEST_ASSERT(root->l->v == 0);

        int_rbtree_delete(root);
    }
    // Right is a leaf
    {
        int_rbtree_t* root = int_rbtree_new(0);
        int_rbtree_link_left(root, int_rbtree_new(-1));
        int_rbtree_link_right(root, int_rbtree_new(2));
        int_rbtree_check_integrity(root);

        int_rbtree_rotate_left(root);
        int_rbtree_check_integrity(root);

        JCTEST_ASSERT(root->v == 2);
        JCTEST_ASSERT(root->l->v == 0);
        JCTEST_ASSERT(root->l->l->v == -1);

        int_rbtree_delete(root);
    }
    // Right has a left child
    {
        int_rbtree_t* root = int_rbtree_new(0);
        int_rbtree_link_left(root, int_rbtree_new(-1));
        int_rbtree_link_right(root, int_rbtree_new(2));
        int_rbtree_link_left(root->r, int_rbtree_new(1));
        int_rbtree_check_integrity(root);

        int_rbtree_rotate_left(root);
        int_rbtree_check_integrity(root);

        JCTEST_ASSERT(root->v == 2);
        JCTEST_ASSERT(root->l->v == 0);
        JCTEST_ASSERT(root->l->l->v == -1);
        JCTEST_ASSERT(root->l->r->v == 1);

        int_rbtree_delete(root);
    }
    // Complete case
    {
        int_rbtree_t* root = int_rbtree_new(0);
        int_rbtree_link_left(root, int_rbtree_new(-2));
        int_rbtree_link_left(root->l, int_rbtree_new(-3));
        int_rbtree_link_right(root->l, int_rbtree_new(-1));
        int_rbtree_link_right(root, int_rbtree_new(2));
        int_rbtree_link_right(root->r, int_rbtree_new(3));
        int_rbtree_link_left(root->r, int_rbtree_new(1));
        int_rbtree_check_integrity(root);

        int_rbtree_rotate_left(root);
        int_rbtree_check_integrity(root);

        JCTEST_ASSERT(root->v == 2);
        JCTEST_ASSERT(root->l->v == 0);
        JCTEST_ASSERT(root->l->l->v == -2);
        JCTEST_ASSERT(root->l->l->l->v == -3);
        JCTEST_ASSERT(root->l->l->r->v == -1);
        JCTEST_ASSERT(root->l->r->v == 1);
        JCTEST_ASSERT(root->r->v == 3);

        int_rbtree_delete(root);
    }
}


JCTEST_FUNC("rbtree_rotate_right") {
    // Left is a leaf, no right
    {
        int_rbtree_t* root = int_rbtree_new(0);
        int_rbtree_link_left(root, int_rbtree_new(-2));
        int_rbtree_check_integrity(root);

        int_rbtree_rotate_right(root);
        int_rbtree_check_integrity(root);

        JCTEST_ASSERT(root->v == -2);
        JCTEST_ASSERT(root->r->v == 0);

        int_rbtree_delete(root);
    }
    // Left is a leaf
    {
        int_rbtree_t* root = int_rbtree_new(0);
        int_rbtree_link_right(root, int_rbtree_new(1));
        int_rbtree_link_left(root, int_rbtree_new(-2));
        int_rbtree_check_integrity(root);

        int_rbtree_rotate_right(root);
        int_rbtree_check_integrity(root);

        JCTEST_ASSERT(root->v == -2);
        JCTEST_ASSERT(root->r->v == 0);
        JCTEST_ASSERT(root->r->r->v == 1);

        int_rbtree_delete(root);
    }
    // Left has a right child
    {
        int_rbtree_t* root = int_rbtree_new(0);
        int_rbtree_link_right(root, int_rbtree_new(1));
        int_rbtree_link_left(root, int_rbtree_new(-2));
        int_rbtree_link_right(root->l, int_rbtree_new(-1));
        int_rbtree_check_integrity(root);

        int_rbtree_rotate_right(root);
        int_rbtree_check_integrity(root);

        JCTEST_ASSERT(root->v == -2);
        JCTEST_ASSERT(root->r->v == 0);
        JCTEST_ASSERT(root->r->r->v == +1);
        JCTEST_ASSERT(root->r->l->v == -1);

        int_rbtree_delete(root);
    }
    // Complete case
    {
        int_rbtree_t* root = int_rbtree_new(0);
        int_rbtree_link_right(root, int_rbtree_new(+2));
        int_rbtree_link_right(root->r, int_rbtree_new(+3));
        int_rbtree_link_left(root->r, int_rbtree_new(+1));
        int_rbtree_link_left(root, int_rbtree_new(-2));
        int_rbtree_link_left(root->l, int_rbtree_new(-3));
        int_rbtree_link_right(root->l, int_rbtree_new(-1));
        int_rbtree_check_integrity(root);

        int_rbtree_rotate_right(root);
        int_rbtree_check_integrity(root);

        JCTEST_ASSERT(root->v == -2);
        JCTEST_ASSERT(root->r->v == 0);
        JCTEST_ASSERT(root->r->r->v == 2);
        JCTEST_ASSERT(root->r->r->r->v == 3);
        JCTEST_ASSERT(root->r->r->l->v == 1);
        JCTEST_ASSERT(root->r->l->v == -1);
        JCTEST_ASSERT(root->l->v == -3);

        int_rbtree_delete(root);
    }
}


JCTEST_FUNC("insert") {
    int values[] = {3, 8, 7, 1, 6, 5, 2, 3};
    int_rbtree_t* root = NULL;

    for (size_t i = 0; i < countof(values); i++) {
        int_rbtree_insert(&root, values[i]);
    }
    int_rbtree_check_integrity(root);
    int_rbtree_check_rb_integrity(root);

    int_rbtree_delete(root);
}


JCTEST_FUNC("insert_random") {
#define INSERT_RANDOM_N     100000
    int* values = malloc(INSERT_RANDOM_N * sizeof(int));
    for (size_t i = 0; i < INSERT_RANDOM_N; i++) {
        values[i] = rand();
    }

    int_rbtree_t* root = NULL;
    for (size_t i = 0; i < INSERT_RANDOM_N; i++) {
        int_rbtree_insert(&root, values[i]);
    }
    int_rbtree_check_integrity(root);
    int_rbtree_check_rb_integrity(root);

    JCTEST_ASSERT(int_rbtree_size(root) == INSERT_RANDOM_N);
    for (size_t i = 0; i < INSERT_RANDOM_N; i++) {
        JCTEST_ASSERT(int_rbtree_find(root, values[i]) != NULL);
    }

    int_rbtree_delete(root);
    free(values);
}


JCTEST_FUNC("remove_trivial_cases") {
    // Remove the root
    //    n        r
    //   / \  ->  /
    //  l   r    l
    {
        int_rbtree_t* root = int_rbtree_new(0);
        int_rbtree_link_left(root, int_rbtree_new(-1));
        int_rbtree_link_right(root, int_rbtree_new(+1));
        int_rbtree_remove(&root, 0);
        JCTEST_ASSERT(root->v == 1);
        JCTEST_ASSERT(root->r == NULL);
        JCTEST_ASSERT(root->l->v == -1);
        int_rbtree_delete(root);
    }

    // Remove the left leaf
    //      n         n
    //     / \  ->   / \ .
    //    l   r    lr   r
    //   / \       /
    //  ll lr     ll
    {
        int_rbtree_t* root = int_rbtree_new(0);
        int_rbtree_link_left(root, int_rbtree_new(-2));
        int_rbtree_link_left(root->l, int_rbtree_new(-3));
        int_rbtree_link_right(root->l, int_rbtree_new(-1));
        int_rbtree_link_right(root, int_rbtree_new(+1));
        int_rbtree_remove(&root, -2);
        JCTEST_ASSERT(root->v == 0);
        JCTEST_ASSERT(root->l->v == -1);
        JCTEST_ASSERT(root->l->l->v == -3);
        JCTEST_ASSERT(root->l->r == NULL);
        JCTEST_ASSERT(root->r->v == +1);
        int_rbtree_delete(root);
    }

    // Remove the right leaf
    //      n          n
    //     / \  ->    / \ .
    //    l   r      l   rrl
    //       / \        / \ .
    //     rl   rr     rl  rr
    //         / \          \ .
    //       rrl rrr        rrr
    {
        int_rbtree_t* root = int_rbtree_new(0);
        int_rbtree_link_left(root, int_rbtree_new(-1));
            int_rbtree_link_right(root, int_rbtree_new(2));
                int_rbtree_link_left(root->r, int_rbtree_new(1));
                int_rbtree_link_right(root->r, int_rbtree_new(5));
                    int_rbtree_link_left(root->r->r, int_rbtree_new(4));
                    int_rbtree_link_right(root->r->r, int_rbtree_new(6));
        int_rbtree_remove(&root, 2);
        JCTEST_ASSERT(root->v == 0);
        JCTEST_ASSERT(root->l->v == -1);
            JCTEST_ASSERT(root->r->v == 4);
                JCTEST_ASSERT(root->r->l->v == 1);
                JCTEST_ASSERT(root->r->r->v == 5);
                JCTEST_ASSERT(root->r->r->r->v == 6);
        int_rbtree_delete(root);
    }

    // Remove an internal node with two childs
    {
        int_rbtree_t* root = int_rbtree_new(0);
        int_rbtree_link_left(root, int_rbtree_new(-1));
        int_rbtree_link_right(root, int_rbtree_new(+2));
        int_rbtree_link_left(root->r, int_rbtree_new(+1));
        int_rbtree_link_right(root->r, int_rbtree_new(+3));
        int_rbtree_remove(&root, +2);
        JCTEST_ASSERT(root->v == 0);
        JCTEST_ASSERT(root->r->v == +3);
        JCTEST_ASSERT(root->r->r == NULL);
        JCTEST_ASSERT(root->r->l->v == 1);
        int_rbtree_delete(root);
    }
}


enum {
    P = 100, N = 50, S = 200, Sl = 150, Sr = 250
};


JCTEST_FUNC("remove_case_2") {
    int_rbtree_t* root = int_rbtree_new(P);
    int_rbtree_link_left(root, int_rbtree_new(N));
    int_rbtree_link_right(root, int_rbtree_new(S));
    int_rbtree_link_left(root->r, int_rbtree_new(Sl));
    int_rbtree_link_right(root->r, int_rbtree_new(Sr));
    root->r->color = RBTREE_COLOR_RED;

    int_rbtree_t* n = root->l;
    int_rbtree_remove_case_2(root->l, 1);
    JCTEST_ASSERT(root->v == S);
    JCTEST_ASSERT(root->color == RBTREE_COLOR_BLACK);
    JCTEST_ASSERT(root->r->v == Sr);
    JCTEST_ASSERT(root->l->v == P);
    JCTEST_ASSERT(root->l->color == RBTREE_COLOR_RED);
    JCTEST_ASSERT(root->l->l->v == N);
    JCTEST_ASSERT(root->l->r->v == Sl);

    JCTEST_ASSERT(n->p->v == P);

    int_rbtree_delete(root);
}


JCTEST_FUNC("remove_random") {
#define REMOVE_RANDOM_N     100000
    int* values = malloc(REMOVE_RANDOM_N * sizeof(int));
    for (size_t i = 0; i < REMOVE_RANDOM_N; i++) {
        values[i] = rand();
    }

    int_rbtree_t* root = NULL;
    for (size_t i = 0; i < REMOVE_RANDOM_N; i++) {
        int_rbtree_insert(&root, values[i]);
    }
    assert(int_rbtree_size(root) == REMOVE_RANDOM_N);

    // Shuffle
    for (size_t i = 0; i < REMOVE_RANDOM_N; i++) {
        int idx_a = rand() % REMOVE_RANDOM_N;
        int idx_b = rand() % REMOVE_RANDOM_N;
        swap(values[idx_a], values[idx_b]);
    }

    // Pre-check
    for (size_t i = 0;i < REMOVE_RANDOM_N; i++) {
        assert(int_rbtree_find(root, values[i]) != NULL);
    }
    int_rbtree_check_integrity(root);
    int_rbtree_check_rb_integrity(root);

    // Removal
    size_t rm_count = REMOVE_RANDOM_N / 10;
    for (size_t i = 0; i < REMOVE_RANDOM_N / 10; i++) {
        int_rbtree_remove(&root, values[i]);
    }

    // Post check
    int_rbtree_check_integrity(root);
    int_rbtree_check_rb_integrity(root);

    JCTEST_ASSERT(int_rbtree_size(root) == REMOVE_RANDOM_N - rm_count);
    for (size_t i = rm_count; i < REMOVE_RANDOM_N; i++) {
        JCTEST_ASSERT(int_rbtree_find(root, values[i]) != NULL);
    }

    int_rbtree_delete(root);
    free(values);
}


JCTEST_FUNC("with struct") {
    test_map_t* root = test_map_new(
        (struct test){.key = "foo", .value = 32}
    );
    test_map_insert(&root, (struct test){
        .key = "zzz", .value = 64
    });
    test_map_delete(root);
}


JCTEST_END
