/*
 * jclib/algorithm/rbtree.h - Red-black tree generation file
 */
// Checks ---------------------------------------------------------------------
#ifndef JCRBTREE_TYPE
    #error "JCRBTREE_TYPE undefined"
#endif


// Helpers --------------------------------------------------------------------
#define __TCONCAT(x, y) x ## _ ## y
#define _TCONCAT(x, y) __TCONCAT(x, y)
#define TYPE JCRBTREE_TYPE
#ifndef JCRBTREE_NAME
#   define TYPENAME _TCONCAT(TYPE, rbtree)
#else
#   define TYPENAME JCRBTREE_NAME
#endif
#define TYPEDEF _TCONCAT(TYPENAME, t)
#define FUNC(_name) _TCONCAT(TYPENAME, _name)


// Common macros --------------------------------------------------------------
#ifndef JCRBTREE_COMMON
#define JCRBTREE_COMMON


typedef enum rbtree_color {
    RBTREE_COLOR_BLACK,
    RBTREE_COLOR_RED,
} rbtree_color_t;


#endif


// Prototype ------------------------------------------------------------------
#include <stddef.h>


typedef struct TYPENAME {
    TYPE v;
    rbtree_color_t color;
    struct TYPENAME *p, *l, *r;
} TYPEDEF;


TYPEDEF* FUNC(new)(TYPE v);


void FUNC(delete)(TYPEDEF* node);


TYPE* FUNC(find)(TYPEDEF* root, TYPE v);


const TYPE* FUNC(cfind)(const TYPEDEF* root, TYPE v);


TYPEDEF* FUNC(node_of)(const TYPE* addr);


const TYPE* FUNC(cfind)(const TYPEDEF* root, TYPE v);


int FUNC(insert)(TYPEDEF** root, TYPE v);


size_t FUNC(size)(const TYPEDEF* root);


int FUNC(remove)(TYPEDEF** root, TYPE v);


// Implementation -------------------------------------------------------------
#ifdef JCRBTREE_IMPLEMENTATION


#ifndef JCRBTREE_EQ
    #define JCRBTREE_EQ(_a, _b) (!memcmp(&_a, &_b, sizeof(JCRBTREE_TYPE)))
#endif


#ifndef JCRBTREE_CMP
    #define JCRBTREE_CMP(_a, _b) (memcmp(&_a, &_b, sizeof(JCRBTREE_TYPE)))
#endif


#ifndef JCRBTREE_WIPE
    #define JCRBTREE_WIPE(_a)
#endif


#ifndef JCRBTREE_COPY
    #define JCRBTREE_COPY(_a, _b) (_a = _b)
#endif


#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#include "jclib/jclib.h"


// https://en.wikipedia.org/wiki/Red%E2%80%93black_tree


TYPEDEF* FUNC(new)(TYPE v) {
    TYPEDEF* node = malloc(sizeof(TYPEDEF));
    if (!node) {
        return NULL;
    }
    *node = (TYPEDEF){
        .v = v,
        .color = RBTREE_COLOR_BLACK,
        .p = NULL,
        .l = NULL,
        .r = NULL
    };
    return node;
}


void FUNC(delete)(TYPEDEF* node) {
    if (!node) {
        return;
    }
    FUNC(delete)(node->l);
    FUNC(delete)(node->r);
    JCRBTREE_WIPE(&node->v);
    free(node);
}


TYPEDEF* FUNC(node_of)(const TYPE* addr) {
    return containerof(addr, TYPEDEF, v);
}


static int FUNC(is_leaf)(const TYPEDEF* node) {
    return !node->l && !node->r;
}


static TYPEDEF* FUNC(parent)(TYPEDEF* node) {
    return node->p;
}


static TYPEDEF* FUNC(grandparent)(TYPEDEF* node) {
    TYPEDEF* p = FUNC(parent)(node);
    if (!p) {
        return NULL;
    }
    return FUNC(parent)(p);
}


static TYPEDEF* FUNC(sibling)(TYPEDEF* node) {
    TYPEDEF* p = FUNC(parent)(node);
    if (!p) {
        return NULL;
    }
    if (node == p->l) {
        return p->r;
    } else {
        return p->l;
    }
}


static TYPEDEF* FUNC(uncle)(TYPEDEF* node) {
    TYPEDEF* g = FUNC(grandparent)(node);
    if (!g) {
        return NULL;
    }
    // XXX Check node has parent ?
    return FUNC(sibling)(FUNC(parent)(node));
}


static void FUNC(link_left)(TYPEDEF* root, TYPEDEF* node) {
    node->p = root;
    root->l = node;
}


static void FUNC(link_right)(TYPEDEF* root, TYPEDEF* node) {
    node->p = root;
    root->r = node;
}


static TYPEDEF* FUNC(unlink)(TYPEDEF* node) {
    if (node->p) {
        if (node == node->p->l) {
            node->p->l = NULL;
        } else {
            node->p->r = NULL;
        }
    }
    node->p = NULL;
    return node;
}


//    1           2              3
//    n           n*         n       r
//   / \         / \        / \     /
//  l   r       l   r*     l   rr rl
//     / \         / \ .
//   rl   rr      rl  rr
//
//      4             5           6
//    n   r       n       r       r*
//   / \   \       \     / \     / \ .
//  l   rr  rl      rr  l   rl  n*  rr
//                             / \ .
//                            l   rl
//
// At the exit of the function node is pointing to
// the new root (r)
//
static TYPEDEF* FUNC(rotate_left)(TYPEDEF* node) {
    assert(node->r);

    swap(node->v, node->r->v);
    swap(node->color, node->r->color);
    TYPEDEF* r = FUNC(unlink)(node->r);
    if (r->r) {
        FUNC(link_right)(node, FUNC(unlink)(r->r));
    }
    if (r->l) {
        FUNC(link_right)(r, FUNC(unlink)(r->l));
    }
    if (node->l) {
        FUNC(link_left)(r, FUNC(unlink)(node->l));
    }
    FUNC(link_left)(node, r);
    return node->l;
}


// After the rotation, node is node->right
static TYPEDEF* FUNC(rotate_right)(TYPEDEF* node) {
    assert(node->l);

    swap(node->v, node->l->v);
    swap(node->color, node->l->color);
    TYPEDEF* l = FUNC(unlink)(node->l);
    if (l->l) {
        FUNC(link_left)(node, FUNC(unlink)(l->l));
    }
    if (l->r) {
        FUNC(link_left)(l, FUNC(unlink)(l->r));
    }
    if (node->r) {
        FUNC(link_right)(l, FUNC(unlink)(node->r));
    }
    FUNC(link_right)(node, l);
    return node->r;
}


__unused__
static TYPEDEF* FUNC(min_node)(TYPEDEF* node) {
    if (!node) {
        return NULL;
    }
    while (node->l) {
        node = node->l;
    }
    return node;
}


__unused__
static TYPEDEF* FUNC(max_node)(TYPEDEF* node) {
    if (!node) {
        return NULL;
    }
    while (node->r) {
        node = node->r;
    }
    return node;
}


// ---- Insertion -------------------------------------------------------------
static void FUNC(insert_repair_tree)(TYPEDEF* node);


static void FUNC(insert_case_3)(TYPEDEF* node) {
    node->p->color = RBTREE_COLOR_BLACK;
    FUNC(uncle)(node)->color = RBTREE_COLOR_BLACK;
    node->p->p->color = RBTREE_COLOR_RED;
    FUNC(insert_repair_tree)(node->p->p); 
}


static void FUNC(insert_case_4_2)(TYPEDEF* node) {
    TYPEDEF* p = node->p;
    TYPEDEF* g = node->p->p;
    if (node == p->l) {
        FUNC(rotate_right)(g);
    } else {
        FUNC(rotate_left)(g);
    }
    // p is now grandparent, and g the parent
    p->color = RBTREE_COLOR_RED;
    g->color = RBTREE_COLOR_BLACK;
}


static void FUNC(insert_case_4)(TYPEDEF* node) {
    TYPEDEF* p = node->p;
    TYPEDEF* g = node->p->p;
    if (node == p->r && p == g->l) {
        FUNC(rotate_left)(p);
    } else
    if (node == p->l && p == g->r) {
        FUNC(rotate_right)(p);
    }
    FUNC(insert_case_4_2)(node);
}


static void FUNC(insert_repair_tree)(TYPEDEF* node) {
    if (!node->p) {
        node->color = RBTREE_COLOR_BLACK;
    } else
    if (node->p->color == RBTREE_COLOR_BLACK) {
        // All is right my captain !
    } else
    if (FUNC(uncle)(node) && FUNC(uncle)(node)->color == RBTREE_COLOR_RED) {
        FUNC(insert_case_3)(node);
    } else {
        FUNC(insert_case_4)(node);
    }
}


int FUNC(insert)(TYPEDEF** root, TYPE v) {
    TYPEDEF* p = NULL;
    while (*root != NULL) {
        p = *root;
        if (JCRBTREE_CMP(v, (*root)->v) <= 0) {
            root = &(*root)->l;
        } else {
            root = &(*root)->r;
        }
    }
    *root = FUNC(new)(v);
    if (!(*root)) {
        return -1;
    }
    (*root)->p = p;
    (*root)->color = RBTREE_COLOR_RED;
    FUNC(insert_repair_tree)(*root);
    return 1;
}


// ---- Removal ---------------------------------------------------------------
static void FUNC(replace_node)(TYPEDEF* node, TYPEDEF* with) {
    with->p = node->p;
    if (node == node->p->l) {
        node->p->l = with;
    } else {
        node->p->r = with;
    }
}


static void FUNC(remove_case_1)(TYPEDEF* n);
static void FUNC(remove_case_2)(TYPEDEF* n, int test);
static void FUNC(remove_case_3)(TYPEDEF* n);
static void FUNC(remove_case_4)(TYPEDEF* n);
static void FUNC(remove_case_5)(TYPEDEF* n);
static void FUNC(remove_case_6)(TYPEDEF* n);


static void FUNC(remove_case_1)(TYPEDEF* n) {
    if (n->p) {
        FUNC(remove_case_2)(n, 0);
    }
}


static void FUNC(remove_case_2)(TYPEDEF* n, int test) {
    TYPEDEF* s = FUNC(sibling)(n);
    if (s->color == RBTREE_COLOR_RED) {
        __unused__
        TYPE s_v = s->v;
        n->p->color = RBTREE_COLOR_RED;
        s->color = RBTREE_COLOR_BLACK;
        if (n == n->p->l) {
            FUNC(rotate_left)(n->p);
        } else {
            FUNC(rotate_right)(n->p);
        }
        assert(!memcmp(&n->p->p->v, &s_v, sizeof(s_v)));
    }
    if (!test) FUNC(remove_case_3)(n);
}


static void FUNC(remove_case_3)(TYPEDEF* n) {
    TYPEDEF* s = FUNC(sibling)(n);
    if (n->p->color == RBTREE_COLOR_BLACK
    &&  s->color == RBTREE_COLOR_BLACK
    &&  (!s->l || s->l->color == RBTREE_COLOR_BLACK)
    &&  (!s->r || s->r->color == RBTREE_COLOR_BLACK))
    {
        s->color = RBTREE_COLOR_RED;
        FUNC(remove_case_1)(n->p);   
    } else {
        FUNC(remove_case_4)(n);
    }
}


static void FUNC(remove_case_4)(TYPEDEF* n) {
    TYPEDEF* s = FUNC(sibling)(n);
    if (n->p->color == RBTREE_COLOR_RED
    &&  s->color == RBTREE_COLOR_BLACK
    &&  (!s->l || s->l->color == RBTREE_COLOR_BLACK)
    &&  (!s->r || s->r->color == RBTREE_COLOR_BLACK))
    {
        s->color = RBTREE_COLOR_RED;
        n->p->color = RBTREE_COLOR_BLACK;
    } else {
        FUNC(remove_case_5)(n);
    }
}


static void FUNC(remove_case_5)(TYPEDEF* n) {
    TYPEDEF* s = FUNC(sibling)(n);
    if (s->color == RBTREE_COLOR_BLACK) {
        if (n == n->p->l
        &&  (!s->r || s->r->color == RBTREE_COLOR_BLACK)
        &&  s->l->color == RBTREE_COLOR_RED)
        {
            s->color = RBTREE_COLOR_RED;
            s->l->color = RBTREE_COLOR_BLACK;
            FUNC(rotate_right)(s);
        } else
        if (n == n->p->r
        &&  (!s->l || s->l->color == RBTREE_COLOR_BLACK)
        &&  s->r->color == RBTREE_COLOR_RED)
        {
            s->color = RBTREE_COLOR_RED;
            s->r->color = RBTREE_COLOR_BLACK;
            FUNC(rotate_left)(s);
        }
    }
    FUNC(remove_case_6)(n);
}


static void FUNC(remove_case_6)(TYPEDEF* n) {
    TYPEDEF* s = FUNC(sibling)(n);
    s->color = n->p->color;
    n->p->color = RBTREE_COLOR_BLACK;
    if (n == n->p->l) {
        if (s->r) {
            s->r->color = RBTREE_COLOR_BLACK;
        }
        FUNC(rotate_left)(n->p);
    } else {
        if (s->l) {
            s->l->color = RBTREE_COLOR_BLACK;
        }
        FUNC(rotate_right)(n->p);
    }
}


static void FUNC(_remove)(TYPEDEF* n) {
    TYPEDEF* c = (n->l) ? n->l : n->r;
    assert(c);
    FUNC(replace_node)(n, c);
    if (n->color == RBTREE_COLOR_BLACK) {
        if (c->color == RBTREE_COLOR_RED) {
            c->color = RBTREE_COLOR_BLACK;
        } else {
            FUNC(remove_case_1)(c);
        }
    }
    n->l = n->r = NULL;
    FUNC(delete)(n);
}


int FUNC(remove)(TYPEDEF** root, TYPE v) {
    while (*root != NULL) {
        if (JCRBTREE_CMP(v, (*root)->v) == 0) {
            break;
        } else
        if (JCRBTREE_CMP(v, (*root)->v) < 0) {
            root = &(*root)->l;
        } else {
            root = &(*root)->r;
        }
    }
    if (!(*root)) {
        return 0;
    }

    // In case the node has two childs or none, removal is trivial, and no
    // tree fixing is needed.
    if ((*root)->l && (*root)->r) {
        TYPEDEF* rl = FUNC(min_node)((*root)->r);
        JCRBTREE_WIPE((*root)->v);
        (*root)->v = rl->v;
        if ((*root)->r == rl) {
            root = &(*root)->r;
        } else {
            assert(rl->p->l == rl);
            root = &rl->p->l;
        }
    }
    if (FUNC(is_leaf)(*root)) {
        int was_black = (*root)->color == RBTREE_COLOR_BLACK;
        TYPEDEF* p = (*root)->p;
        FUNC(delete)(*root);
        *root = NULL;
        if (FUNC(is_leaf)(p) && was_black) {
            if (p->color == RBTREE_COLOR_RED) {
                p->color = RBTREE_COLOR_BLACK;
            } else {
                FUNC(remove_case_1)(p);
            }
        }
    } else {
        FUNC(_remove)(*root);
    }
    return 1;
}


// ---- Integrity checks (for tests) ------------------------------------------
#ifndef RBTREE_NODE_STACK
    #undef __TCONCAT
    #undef _TCONCAT
    #undef TYPE
    #undef TYPENAME
    #undef TYPEDEF
    #undef FUNC

    #define JCARRAY_NAME    node_stack
    #define JCARRAY_TYPE    const void*
    #define JCARRAY_PROTOTYPE
    #define JCARRAY_IMPLEMENTATION
    #include "jclib/algorithm/array.h"

    #define __TCONCAT(x, y) x ## _ ## y
    #define _TCONCAT(x, y) __TCONCAT(x, y)
    #define TYPE JCRBTREE_TYPE
    #define TYPENAME _TCONCAT(TYPE, rbtree)
    #define TYPEDEF _TCONCAT(TYPENAME, t)
    #define FUNC(_name) _TCONCAT(TYPENAME, _name)

    #define RBTREE_NODE_STACK
#endif


__unused__
static void FUNC(check_integrity)(const TYPEDEF* node) {
    if (!node) {
        return;
    }
    if (node->l) {
        assert(JCRBTREE_CMP(node->v, node->l->v) >= 0);
        assert(node->l->p == node);
        FUNC(check_integrity)(node->l);
    }
    if (node->r) {
        assert(JCRBTREE_CMP(node->v, node->r->v) <= 0);
        assert(node->r->p == node);
        FUNC(check_integrity)(node->r);
    }
}


__unused__
static size_t FUNC(count_black_node_asc)(const TYPEDEF* root,
                                         const TYPEDEF* node)
{
    size_t res = 0;
    while (node != root->p) {
        if (node->color == RBTREE_COLOR_BLACK) {
            res++;
        }
        node = node->p;
    }
    return res;
}


__unused__
static void FUNC(check_rb_black_nodes)(const TYPEDEF* node) {
    if (!node) {
        return;
    }

    size_t current_count = SIZE_MAX; 
    size_t previous_count = SIZE_MAX;

    node_stack_t stack; 
    node_stack_init(&stack, 0);

    node_stack_append(&stack, node);
    while (node_stack_length(&stack) > 0) {
        const TYPEDEF* n = node_stack_pop_back(&stack);
        if (FUNC(is_leaf)(n)) {
            current_count = FUNC(count_black_node_asc(node, n));
            if (previous_count == SIZE_MAX) {
                previous_count = current_count;
            } else {
                assert(current_count == previous_count);
            }
        } else {
            if (n->l) {
                node_stack_append(&stack, n->l);
            }
            if (n->r) {
                node_stack_append(&stack, n->r);
            }
        }
    }

    node_stack_wipe(&stack);

    // We don't need to recurse. If the result is true for the whole tree, it
    // will be true for its subtrees.
    // Indeed, if it could be false for one of the subtree, then the
    // number of black nodes on every path to the left leaves would be
    // different for the right subtree, meaning the result would be false for
    // the whole tree, that is absurd assuming the previous result.
}


__unused__
static void FUNC(check_rb_integrity)(const TYPEDEF* node) {
    // Root is always black
    if (!node->p) {
        assert(node->color == RBTREE_COLOR_BLACK);
        FUNC(check_rb_black_nodes(node));
    }

    // Both childs of a red node are black
    if (node->color == RBTREE_COLOR_RED) {
        assert(!node->l || node->l->color == RBTREE_COLOR_BLACK);
        assert(!node->r || node->r->color == RBTREE_COLOR_BLACK);
    }

    if (node->l) {
        FUNC(check_rb_integrity)(node->l);
    }
    if (node->r) {
        FUNC(check_rb_integrity)(node->r);
    }
}



// ----------------------------------------------------------------------------


TYPE* FUNC(find)(TYPEDEF* root, TYPE v) {
    if (!root) {
        return NULL;
    }
    if (JCRBTREE_CMP(v, root->v) < 0) {
        return FUNC(find)(root->l, v);
    } else
    if (JCRBTREE_CMP(v, root->v) > 0) {
        return FUNC(find)(root->r, v);
    }
    return &root->v;
}


const TYPE* FUNC(cfind)(const TYPEDEF* root, TYPE v) {
    return FUNC(find)((TYPEDEF*)root, v);
}


size_t FUNC(size)(const TYPEDEF* root) {
    if (!root) {
        return 0;
    }
    return 1 + FUNC(size)(root->l) + FUNC(size)(root->r);
}


#undef JCRBTREE_IMPLEMENTATION
#endif


// Cleanup --------------------------------------------------------------------
#undef JCRBTREE_TYPE
#undef JCRBTREE_NAME
#undef JCRBTREE_EQ
#undef JCRBTREE_CMP
#undef JCRBTREE_WIPE
#undef JCRBTREE_COPY
#undef __TCONCAT
#undef _TCONCAT
#undef TYPE
#undef TYPENAME
#undef TYPEDEF
#undef FUNC


// ----------------------------------------------------------------------------
