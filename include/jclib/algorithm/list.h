/*
 * jclib/algorithm/list.h - Linked list generation file
 */
// Checks ---------------------------------------------------------------------
#ifndef JCLIST_TYPE
    #error "JCLIST_TYPE undefined"
#endif

#ifndef JCLIST_EQ
    #define JCLIST_EQ(_a, _b) (!memcmp(&_a, &_b, sizeof(JCLIST_TYPE)))
#endif


#ifndef JCLIST_WIPE
    #define JCLIST_WIPE(_a)
#endif


#ifndef JCLIST_COPY
    #define JCLIST_COPY(_a, _b) (_a = _b)
#endif


// Helpers --------------------------------------------------------------------
#define __TCONCAT(x, y) x ## _ ## y
#define _TCONCAT(x, y) __TCONCAT(x, y)
#define TYPE JCLIST_TYPE
#define TYPENAME _TCONCAT(TYPE, list)
#define TYPEDEF _TCONCAT(TYPENAME, t)
#define FUNC(_name) _TCONCAT(TYPENAME, _name)
#define TYPENAME_NODE _TCONCAT(TYPE, list_node)
#define TYPEDEF_NODE _TCONCAT(TYPENAME_NODE, t)
#define FUNC_NODE(_name) _TCONCAT(TYPENAME_NODE, _name)


// Prototype ------------------------------------------------------------------
#include <stddef.h>


typedef struct TYPENAME_NODE TYPEDEF_NODE;


typedef struct TYPENAME {
    TYPEDEF_NODE* root;
} TYPEDEF;


int FUNC(init)(TYPEDEF* list);


int FUNC(init_with)(TYPEDEF* list, const TYPE* data, size_t size);


TYPEDEF* FUNC(wipe)(TYPEDEF* list);


TYPE FUNC(get)(const TYPEDEF* list, size_t index);


TYPE* FUNC(getp)(TYPEDEF* list, size_t index);


const TYPE* FUNC(cgetp)(const TYPEDEF* list, size_t index);


size_t FUNC(length)(const TYPEDEF* list);


TYPE* FUNC(find)(TYPEDEF* list, TYPE v);


const TYPE* FUNC(cfind)(const TYPEDEF* list, TYPE v);


int FUNC(eq_with)(const TYPEDEF* list, const TYPE* data, size_t count);


int FUNC(insert)(TYPEDEF* list, const TYPE* at, TYPE v);


int FUNC(prepend)(TYPEDEF* list, TYPE v);


int FUNC(append)(TYPEDEF* list, TYPE v);


// Implementation -------------------------------------------------------------
#ifdef JCLIST_IMPLEMENTATION


#include <stdlib.h>
#include <string.h>


struct TYPENAME_NODE {
    TYPE v;
    struct TYPENAME_NODE* next;
};


static TYPEDEF_NODE* FUNC_NODE(new)(TYPE v) {
    TYPEDEF_NODE* n = malloc(sizeof(TYPEDEF_NODE));
    *n = (TYPEDEF_NODE){
        .v = v,
        .next = NULL,
    };
    return n;
}


static void FUNC_NODE(delete)(TYPEDEF_NODE* n) {
    JCLIST_WIPE(n->v);
    free(n);
}


static TYPE FUNC_NODE(value)(const TYPEDEF_NODE* node) {
    return node->v;
}


static TYPE* FUNC_NODE(value_ptr)(TYPEDEF_NODE* node) {
    return &node->v;
}


static const TYPE* FUNC_NODE(cvalue_ptr)(const TYPEDEF_NODE* node) {
    return &node->v;
}


__unused__
static TYPEDEF_NODE* FUNC(node_of)(const TYPE* addr) {
    return containerof(addr, TYPEDEF_NODE, v);
}


static TYPEDEF_NODE* FUNC(get_node)(TYPEDEF* list, size_t index) {
    TYPEDEF_NODE* node = list->root;
    while (index > 0 && node) {
        node = node->next;
        index--;
    }
    if (index > 0) {
        return NULL;
    }
    return node;
}


static const TYPEDEF_NODE* FUNC(cget_node)(const TYPEDEF* list, size_t index) {
    const TYPEDEF_NODE* node = list->root;
    while (index > 0 && node) {
        node = node->next;
        index--;
    }
    if (index > 0) {
        return NULL;
    }
    return node;
}


__unused__
static TYPEDEF_NODE* FUNC(back_node)(TYPEDEF* list) {
    TYPEDEF_NODE* node = list->root;
    if (!node) {
        return NULL;
    }
    while (node->next) {
        node = node->next;
    }
    return node;
}


__unused__
static const TYPEDEF_NODE* FUNC(cback_node)(const TYPEDEF* list) {
    const TYPEDEF_NODE* node = list->root;
    if (!node) {
        return NULL;
    }
    while (node->next) {
        node = node->next;
    }
    return node;
}


static TYPEDEF_NODE* FUNC(find_node)(TYPEDEF* list, TYPE v) {
    TYPEDEF_NODE* node = list->root;
    while (node) {
        if (JCLIST_EQ(v, node->v)) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}


static const TYPEDEF_NODE* FUNC(cfind_node)(const TYPEDEF* list, TYPE v) {
    const TYPEDEF_NODE* node = list->root;
    while (node) {
        if (JCLIST_EQ(v, node->v)) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}


int FUNC(init)(TYPEDEF* list) {
    *list = (TYPEDEF){
        .root = NULL,
    };
    return 0;
}


int FUNC(init_with)(TYPEDEF* list, const TYPE* data, size_t size) {
    if (FUNC(init)(list) < 0) {
        return -1;
    }
    TYPEDEF_NODE** node = &list->root;
    for (size_t i = 0; i < size; i++) {
        *node = FUNC_NODE(new)(data[i]);
        if (*node == NULL) {
            return -1;
        }
        node = &(*node)->next;
    }
    return 0;
}


TYPEDEF* FUNC(wipe)(TYPEDEF* list) {
    TYPEDEF_NODE* node = list->root;
    while (node) {
        TYPEDEF_NODE* tmp = node->next;
        FUNC_NODE(delete)(node);
        node = tmp;
    }
    list->root = NULL;
    return list;
}


TYPE FUNC(get)(const TYPEDEF* list, size_t index) {
    const TYPEDEF_NODE* node = FUNC(cget_node)(list, index);
    if (!node) {
        return (TYPE)0;
    }
    return FUNC_NODE(value)(node);
}


TYPE* FUNC(getp)(TYPEDEF* list, size_t index) {
    TYPEDEF_NODE* node = FUNC(get_node)(list, index);
    if (!node) {
        return NULL;
    }
    return FUNC_NODE(value_ptr)(node);
}


const TYPE* FUNC(cgetp)(const TYPEDEF* list, size_t index) {
    const TYPEDEF_NODE* node = FUNC(cget_node)(list, index);
    if (!node) {
        return NULL;
    }
    return FUNC_NODE(cvalue_ptr)(node);
}


size_t FUNC(length)(const TYPEDEF* list) {
    size_t res = 0;
    TYPEDEF_NODE* node = list->root;
    while (node) {
        node = node->next;
        res++;
    }
    return res;
}


TYPE* FUNC(find)(TYPEDEF* list, TYPE v) {
    TYPEDEF_NODE* node = FUNC(find_node)(list, v);
    if (!node) {
        return NULL;
    }
    return &node->v;
}


const TYPE* FUNC(cfind)(const TYPEDEF* list, TYPE v) {
    const TYPEDEF_NODE* node = FUNC(cfind_node)(list, v);
    if (!node) {
        return NULL;
    }
    return &node->v;
}


int FUNC(eq_with)(const TYPEDEF* list, const TYPE* data, size_t count) {
    size_t i = 0;
    const TYPEDEF_NODE* node = list->root;
    while (node && i < count) {
        if (!JCLIST_EQ(node->v, data[i])) {
            return 0;
        }
        node = node->next;
        i++;
    }
    return node == NULL && i == count;
}


int FUNC(insert)(TYPEDEF* list, const TYPE* at, TYPE v) {
    TYPEDEF_NODE* node = FUNC(node_of)(at);
    if (!node) {
        return -1;
    }
    TYPEDEF_NODE* next = FUNC_NODE(new)(node->v);
    next->next = node->next;
    node->v = v;
    node->next = next;
    return 0;
}


int FUNC(prepend)(TYPEDEF* list, TYPE v) {
    TYPEDEF_NODE* node = FUNC_NODE(new)(v);
    if (!node) {
        return -1;
    }
    node->next = list->root;
    list->root = node;
    return 0;
}


int FUNC(append)(TYPEDEF* list, TYPE v) {
    TYPEDEF_NODE* node = FUNC(back_node)(list);
    if (!node) {
        list->root = FUNC_NODE(new)(v);
        if (!list->root) {
            return -1;
        }
    } else {
        node->next = FUNC_NODE(new)(v);
        if (!node->next) {
            return -1;
        }
    }
    return 0;
}


#undef JCLIST_IMPLEMENTATION
#endif


// Common macros --------------------------------------------------------------
#ifndef JCLIST_COMMON
#define JCLIST_COMMON


#define list_apply(_list_ptr, _value_name, _expr) \
{ \
    typeof((_list_ptr)->root) _it = (_list_ptr)->root; \
    while (_it) { \
        typeof((_it->v))* _value_name = &_it->v; \
        _expr; \
        _it = _it->next; \
    } \
}


#endif


// Cleanup --------------------------------------------------------------------
#undef JCLIST_TYPE
#undef JCLIST_EQ
#undef JCLIST_WIPE
#undef JCLIST_COPY
#undef __TCONCAT
#undef _TCONCAT
#undef TYPE
#undef TYPENAME
#undef TYPEDEF
#undef FUNC
#undef TYPENAME_NODE
#undef TYPEDEF_NODE
#undef FUNC_NODE


// ----------------------------------------------------------------------------
