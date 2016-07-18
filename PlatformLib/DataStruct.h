#ifndef __LYNX_PLATFORM_LIB_ALGORITHM_H__
#define __LYNX_PLATFORM_LIB_ALGORITHM_H__

#include "Compat.h"

#ifdef __cplusplus
extern "C" {
#endif

#define LYNX_FREELIST_HEAD(name, size)                                                     \
struct name {                                                                              \
        unsigned mSize;                                                                    \
        unsigned mCount;                                                                   \
        void*    mNodes[size];                                                             \
}

#define LYNX_FREELIST_INIT(head, size)                                                     \
do {                                                                                       \
        (head)->mSize = size; (head)->mCount = 0;                                          \
} while (0)
        
#define LYNX_FREELIST_PUSH(head, node)                                                     \
        ((head)->mCount == (head)->mSize ? 0 :                                             \
        ((head)->mNodes[(head)->mCount++] = node))

#define LYNX_FREELIST_POP(head)                                                            \
        ((head)->mCount == 0 ? NULL : (head)->mNodes[--(head)->mCount])

#define LYNX_FREELIST_EMPTY(head) ((head)->mCount == 0)

/**
 * Single List Define
 */

#define LYNX_LIST_HEAD(name, type)                                                         \
struct name {                                                                              \
    struct type *mFirst;    /* first element */                                            \
    void* mUsrPtr;          /* reversed for user */                                        \
}
 
#define LYNX_LIST_ENTRY(type)                                                              \
struct {                                                                                   \
    struct type *mNext;     /* next element */                                             \
}

#define	LYNX_LIST_FIRST(head) ((head)->mFirst)
#define	LYNX_LIST_END(head) NULL
#define	LYNX_LIST_EMPTY(head) (LYNX_LIST_FIRST(head) == LYNX_LIST_END(head))
#define	LYNX_LIST_NEXT(elm, field) ((elm)->mNext)

#define	LYNX_LIST_FOREACH(var, head, field)                                               \
    for((var) = LYNX_LIST_FIRST(head);  (var) != LYNX_LIST_END(head);                     \
    (var) = LYNX_LIST_NEXT(var, field))

#define	LYNX_LIST_INIT(head) {                                                            \
    LYNX_LIST_FIRST(head) = LYNX_LIST_END(head);                                          \
    (head)->mUsrPtr = NULL;                                                               \
}

#define	LYNX_LIST_INSERT_AFTER(slistelm, elm, field) do {                                 \
    (elm)->mNext = (slistelm)->mNext;                                         \
     (slistelm)->mNext = (elm);                                                     \
} while (0)

#define	LYNX_LIST_INSERT_HEAD(head, elm, field) do {                                      \
    (elm)->mNext = (head)->mFirst;                                                  \
    (head)->mFirst = (elm);                                                               \
} while (0)

#define	LYNX_LIST_REMOVE_HEAD(head, field) do {                                           \
    (head)->mFirst = (head)->mFirst->mNext;                                         \
} while (0)

/**
 * Double List Define
 */

#define LYNX_DLIST_HEAD(name, type)                                                       \
struct name {                                                                             \
    struct type *mFirst;    /* first element */                                           \
    struct type **mLast;    /* address of last next element */                            \
}

#define LYNX_DLIST_ENTRY(type)                                                            \
struct {                                                                                  \
   struct type *mNext;     /* next element */                                             \
   struct type **mPrev;    /* address of previous next element */                         \
}

#define	LYNX_DLIST_FIRST(head) ((head)->mFirst)
#define	LYNX_DLIST_END(head) NULL
#define	LYNX_DLIST_NEXT(elm, field) ((elm)->mNext)
#define LYNX_DLIST_LAST(head, headname)                                                   \
    (*(((struct headname *)((head)->mLast))->mLast))
#define LYNX_DLIST_PREV(elm, headname, field)                                             \
    (*(((struct headname *)((elm)->mPrev))->mLast))
#define	LYNX_DLIST_EMPTY(head)                                                            \
    (LYNX_DLIST_FIRST(head) == LYNX_DLIST_END(head))

#define LYNX_DLIST_FOREACH(var, head, field)                                              \
    for((var) = LYNX_DLIST_FIRST(head); (var) != LYNX_DLIST_END(head);                    \
    (var) = LYNX_DLIST_NEXT(var, field))

#define	LYNX_DLIST_INIT(head) do {                                                        \
    (head)->mFirst = NULL;                                                                \
    (head)->mLast = &(head)->mFirst;                                                      \
} while (0)

#define LYNX_DLIST_INSERT_HEAD(head, elm, field) do {                                     \
   if (((elm)->mNext = (head)->mFirst) != NULL)                                           \
        (head)->mFirst->mPrev = &(elm)->mNext;                                            \
    else                                                                                  \
        (head)->mLast = &(elm)->mNext;                                                    \
    (head)->mFirst = (elm);                                                               \
    (elm)->mPrev = &(head)->mFirst;                                                       \
} while (0)

#define LYNX_DLIST_INSERT_TAIL(head, elm, field) do {                                     \
    (elm)->mNext = NULL;                                                                  \
    (elm)->mPrev = (head)->mLast;                                                         \
    *(head)->mLast = (elm);                                                               \
    (head)->mLast = &(elm)->mNext;                                                        \
} while (0)

#define LYNX_DLIST_INSERT_AFTER(head, listelm, elm, field) do {                           \
    if (((elm)->mNext = (listelm)->mNext) != NULL)                                        \
        (elm)->mNext->mPrev = &(elm)->mNext;                                              \
    else                                                                                  \
        (head)->mLast = &(elm)->mNext;                                                    \
    (listelm)->mNext = (elm);                                                             \
    (elm)->mPrev = &(listelm)->mNext;                                                     \
} while (0)

#define	LYNX_DLIST_INSERT_BEFORE(listelm, elm, field) do {                                \
    (elm)->mPrev = (listelm)->mPrev;                                                      \
    (elm)->mNext = (listelm);                                                             \
    *(listelm)->mPrev = (elm);                                                            \
    (listelm)->mPrev = &(elm)->mNext;                                                     \
} while (0)

#define LYNX_DLIST_REMOVE(head, elm, field) do {                                          \
    if (((elm)->mNext) != NULL)                                                           \
        (elm)->mNext->mPrev = (elm)->mPrev;                                               \
    else                                                                                  \
        (head)->mLast = (elm)->mPrev;                                                     \
    *(elm)->mPrev = (elm)->mNext;                                                         \
} while (0)


/**
 * RB Tree Define
 */

#define LYNX_RB_HEAD(name, type)                                                          \
struct name {                                                                             \
struct type *rbh_root; /* root of the tree */                                             \
}

#define LYNX_RB_INITIALIZER(root)                                                         \
    { NULL }

#define LYNX_RB_INIT(root) do {                                                           \
    (root)->rbh_root = NULL;                                                              \
} while (0)

#define LYNX_RB_BLACK	0
#define LYNX_RB_RED		1
#define LYNX_RB_ENTRY(type)                                                               \
struct {                                                                                  \
struct type *rbe_left;          /* left element */                                        \
struct type *rbe_right;	        /* right element */                                       \
struct type *rbe_parent;        /* parent element */                                      \
int rbe_color;                  /* node color */                                          \
}

#define LYNX_RB_LEFT(elm, field)             (elm)->rbe_left
#define LYNX_RB_RIGHT(elm, field)            (elm)->rbe_right
#define LYNX_RB_PARENT(elm, field)           (elm)->rbe_parent
#define LYNX_RB_COLOR(elm, field)            (elm)->rbe_color
#define LYNX_RB_ROOT(head)                   (head)->rbh_root
#define LYNX_RB_EMPTY(head)                  (LYNX_RB_ROOT(head) == NULL)

#define LYNX_RB_SET(elm, parent, field) do {                                              \
    LYNX_RB_PARENT(elm, field) = parent;                                                  \
    LYNX_RB_LEFT(elm, field) = LYNX_RB_RIGHT(elm, field) = NULL;                          \
    LYNX_RB_COLOR(elm, field) = LYNX_RB_RED;                                              \
} while (0)

#define LYNX_RB_SET_BLACKRED(black, red, field) do {                                      \
    LYNX_RB_COLOR(black, field) = LYNX_RB_BLACK;                                          \
    LYNX_RB_COLOR(red, field) = LYNX_RB_RED;                                              \
} while (0)

#ifndef LYNX_RB_AUGMENT
#define LYNX_RB_AUGMENT(x) {} // Hack by Hailong.Xia
#endif

#define LYNX_RB_ROTATE_LEFT(head, elm, tmp, field) do {                                   \
    (tmp) = LYNX_RB_RIGHT(elm, field);                                                    \
    if ((LYNX_RB_RIGHT(elm, field) = LYNX_RB_LEFT(tmp, field))) {                         \
    LYNX_RB_PARENT(LYNX_RB_LEFT(tmp, field), field) = (elm);                              \
    }                                                                                     \
    LYNX_RB_AUGMENT(elm);                                                                 \
    if ((LYNX_RB_PARENT(tmp, field) = LYNX_RB_PARENT(elm, field))) {                      \
    if ((elm) == LYNX_RB_LEFT(LYNX_RB_PARENT(elm, field), field))                         \
    LYNX_RB_LEFT(LYNX_RB_PARENT(elm, field), field) = (tmp);                              \
        else                                                                              \
        LYNX_RB_RIGHT(LYNX_RB_PARENT(elm, field), field) = (tmp);                         \
    } else                                                                                \
    (head)->rbh_root = (tmp);                                                             \
    LYNX_RB_LEFT(tmp, field) = (elm);                                                     \
    LYNX_RB_PARENT(elm, field) = (tmp);                                                   \
    LYNX_RB_AUGMENT(tmp);                                                                 \
    if ((LYNX_RB_PARENT(tmp, field)))                                                     \
    LYNX_RB_AUGMENT(LYNX_RB_PARENT(tmp, field));                                          \
} while (0)

#define LYNX_RB_ROTATE_RIGHT(head, elm, tmp, field) do {                                  \
    (tmp) = LYNX_RB_LEFT(elm, field);                                                     \
    if ((LYNX_RB_LEFT(elm, field) = LYNX_RB_RIGHT(tmp, field))) {                         \
    LYNX_RB_PARENT(LYNX_RB_RIGHT(tmp, field), field) = (elm);                             \
    }                                                                                     \
    LYNX_RB_AUGMENT(elm);                                                                 \
    if ((LYNX_RB_PARENT(tmp, field) = LYNX_RB_PARENT(elm, field))) {                      \
    if ((elm) == LYNX_RB_LEFT(LYNX_RB_PARENT(elm, field), field))                         \
    LYNX_RB_LEFT(LYNX_RB_PARENT(elm, field), field) = (tmp);                              \
        else                                                                              \
        LYNX_RB_RIGHT(LYNX_RB_PARENT(elm, field), field) = (tmp);                         \
    } else                                                                                \
    (head)->rbh_root = (tmp);                                                             \
    LYNX_RB_RIGHT(tmp, field) = (elm);                                                    \
    LYNX_RB_PARENT(elm, field) = (tmp);                                                   \
    LYNX_RB_AUGMENT(tmp);                                                                 \
    if ((LYNX_RB_PARENT(tmp, field)))                                                     \
    LYNX_RB_AUGMENT(LYNX_RB_PARENT(tmp, field));                                          \
} while (0)

/* Generates prototypes and inline functions */
#define LYNX_RB_PROTOTYPE(name, type, field, cmp)                                         \
void name##_LYNX_RB_INSERT_COLOR(struct name *, struct type *);                           \
void name##_LYNX_RB_REMOVE_COLOR(struct name *, struct type *, struct type *);            \
struct type *name##_LYNX_RB_REMOVE(struct name *, struct type *);                         \
struct type *name##_LYNX_RB_INSERT(struct name *, struct type *);                         \
struct type *name##_LYNX_RB_FIND(struct name *, struct type *);                           \
struct type *name##_LYNX_RB_PREV(struct type *);                                          \
struct type *name##_LYNX_RB_NEXT(struct type *);                                          \
struct type *name##_LYNX_RB_MINMAX(struct name *, int);                                   \
                                                                                          \

/* Main rb operation.
* Moves node close to the key of elm to top
*/
#define LYNX_RB_GENERATE(name, type, field, cmp)                                          \
void                                                                                      \
name##_LYNX_RB_INSERT_COLOR(struct name *head, struct type *elm)                          \
{                                                                                         \
    struct type *parent, *gparent, *tmp;                                                  \
    while ((parent = LYNX_RB_PARENT(elm, field)) &&                                       \
        LYNX_RB_COLOR(parent, field) == LYNX_RB_RED) {                                    \
        gparent = LYNX_RB_PARENT(parent, field);                                          \
        if (parent == LYNX_RB_LEFT(gparent, field)) {                                     \
            tmp = LYNX_RB_RIGHT(gparent, field);                                          \
            if (tmp && LYNX_RB_COLOR(tmp, field) == LYNX_RB_RED) {                        \
                LYNX_RB_COLOR(tmp, field) = LYNX_RB_BLACK;                                \
                LYNX_RB_SET_BLACKRED(parent, gparent, field);                             \
                elm = gparent;                                                            \
                continue;                                                                 \
            }                                                                             \
            if (LYNX_RB_RIGHT(parent, field) == elm) {                                    \
                LYNX_RB_ROTATE_LEFT(head, parent, tmp, field);                            \
                tmp = parent;                                                             \
                parent = elm;                                                             \
                elm = tmp;                                                                \
            }                                                                             \
            LYNX_RB_SET_BLACKRED(parent, gparent, field);	                              \
            LYNX_RB_ROTATE_RIGHT(head, gparent, tmp, field);	                          \
       } else {                                                                           \
           tmp = LYNX_RB_LEFT(gparent, field);                                            \
           if (tmp && LYNX_RB_COLOR(tmp, field) == LYNX_RB_RED) {	                      \
               LYNX_RB_COLOR(tmp, field) = LYNX_RB_BLACK;	                              \
               LYNX_RB_SET_BLACKRED(parent, gparent, field);                              \
               elm = gparent;                                                             \
               continue;                                                                  \
           }                                                                              \
           if (LYNX_RB_LEFT(parent, field) == elm) {                                      \
               LYNX_RB_ROTATE_RIGHT(head, parent, tmp, field);                            \
               tmp = parent;                                                              \
               parent = elm;                                                              \
               elm = tmp;                                                                 \
           }                                                                              \
           LYNX_RB_SET_BLACKRED(parent, gparent, field);                                  \
           LYNX_RB_ROTATE_LEFT(head, gparent, tmp, field);                                \
       }                                                                                  \
    }                                                                                     \
    LYNX_RB_COLOR(head->rbh_root, field) = LYNX_RB_BLACK;                                 \
}                                                                                         \
                                                                                          \
void                                                                                      \
name##_LYNX_RB_REMOVE_COLOR(struct name *head, struct type *parent, struct type *elm)     \
{                                                                                         \
    struct type *tmp;                                                                     \
    while ((elm == NULL || LYNX_RB_COLOR(elm, field) == LYNX_RB_BLACK) &&	              \
        elm != LYNX_RB_ROOT(head)) {                                                      \
        if (LYNX_RB_LEFT(parent, field) == elm) {                                         \
            tmp = LYNX_RB_RIGHT(parent, field);                                           \
            if (LYNX_RB_COLOR(tmp, field) == LYNX_RB_RED) {                               \
                LYNX_RB_SET_BLACKRED(tmp, parent, field);                                 \
                LYNX_RB_ROTATE_LEFT(head, parent, tmp, field);                            \
                tmp = LYNX_RB_RIGHT(parent, field);                                       \
            }                                                                             \
            if ((LYNX_RB_LEFT(tmp, field) == NULL ||                                      \
                LYNX_RB_COLOR(LYNX_RB_LEFT(tmp, field), field) == LYNX_RB_BLACK) &&       \
                (LYNX_RB_RIGHT(tmp, field) == NULL ||                                     \
                LYNX_RB_COLOR(LYNX_RB_RIGHT(tmp, field), field) == LYNX_RB_BLACK)) {      \
                LYNX_RB_COLOR(tmp, field) = LYNX_RB_RED;                                  \
                elm = parent;                                                             \
                parent = LYNX_RB_PARENT(elm, field);                                      \
            } else {                                                                      \
                if (LYNX_RB_RIGHT(tmp, field) == NULL ||                                  \
                    LYNX_RB_COLOR(LYNX_RB_RIGHT(tmp, field), field) == LYNX_RB_BLACK) {   \
                    struct type *oleft;                                                   \
                    if ((oleft = LYNX_RB_LEFT(tmp, field)))                               \
                        LYNX_RB_COLOR(oleft, field) = LYNX_RB_BLACK;                      \
                    LYNX_RB_COLOR(tmp, field) = LYNX_RB_RED;                              \
                    LYNX_RB_ROTATE_RIGHT(head, tmp, oleft, field);                        \
                    tmp = LYNX_RB_RIGHT(parent, field);                                   \
                }                                                                         \
                LYNX_RB_COLOR(tmp, field) = LYNX_RB_COLOR(parent, field);                 \
                LYNX_RB_COLOR(parent, field) = LYNX_RB_BLACK;                             \
                if (LYNX_RB_RIGHT(tmp, field))                                            \
                    LYNX_RB_COLOR(LYNX_RB_RIGHT(tmp, field), field) = LYNX_RB_BLACK;      \
                LYNX_RB_ROTATE_LEFT(head, parent, tmp, field);                            \
                elm = LYNX_RB_ROOT(head);                                                 \
                break;                                                                    \
            }                                                                             \
        } else {                                                                          \
            tmp = LYNX_RB_LEFT(parent, field);                                            \
            if (LYNX_RB_COLOR(tmp, field) == LYNX_RB_RED) {                               \
                LYNX_RB_SET_BLACKRED(tmp, parent, field);                                 \
                LYNX_RB_ROTATE_RIGHT(head, parent, tmp, field);                           \
                tmp = LYNX_RB_LEFT(parent, field);                                        \
            }                                                                             \
            if ((LYNX_RB_LEFT(tmp, field) == NULL ||                                      \
                LYNX_RB_COLOR(LYNX_RB_LEFT(tmp, field), field) == LYNX_RB_BLACK) &&       \
                (LYNX_RB_RIGHT(tmp, field) == NULL ||                                     \
                LYNX_RB_COLOR(LYNX_RB_RIGHT(tmp, field), field) == LYNX_RB_BLACK)) {      \
                LYNX_RB_COLOR(tmp, field) = LYNX_RB_RED;                                  \
                elm = parent;                                                             \
                parent = LYNX_RB_PARENT(elm, field);                                      \
            } else {                                                                      \
                if (LYNX_RB_LEFT(tmp, field) == NULL ||                                   \
                    LYNX_RB_COLOR(LYNX_RB_LEFT(tmp, field), field) == LYNX_RB_BLACK) {    \
                    struct type *oright;                                                  \
                    if ((oright = LYNX_RB_RIGHT(tmp, field)))                             \
                        LYNX_RB_COLOR(oright, field) = LYNX_RB_BLACK;                     \
                    LYNX_RB_COLOR(tmp, field) = LYNX_RB_RED;                              \
                    LYNX_RB_ROTATE_LEFT(head, tmp, oright, field);                        \
                    tmp = LYNX_RB_LEFT(parent, field);                                    \
                }                                                                         \
                LYNX_RB_COLOR(tmp, field) = LYNX_RB_COLOR(parent, field);                 \
                LYNX_RB_COLOR(parent, field) = LYNX_RB_BLACK;                             \
                if (LYNX_RB_LEFT(tmp, field))                                             \
                    LYNX_RB_COLOR(LYNX_RB_LEFT(tmp, field), field) = LYNX_RB_BLACK;       \
                LYNX_RB_ROTATE_RIGHT(head, parent, tmp, field);                           \
                elm = LYNX_RB_ROOT(head);                                                 \
                break;                                                                    \
            }                                                                             \
        }                                                                                 \
    }                                                                                     \
    if (elm)                                                                              \
        LYNX_RB_COLOR(elm, field) = LYNX_RB_BLACK;                                        \
}                                                                                         \
                                                                                          \
struct type *                                                                             \
name##_LYNX_RB_REMOVE(struct name *head, struct type *elm)                                \
{                                                                                         \
    struct type *child, *parent, *old = elm;                                              \
    int color;                                                                            \
    if (LYNX_RB_LEFT(elm, field) == NULL)                                                 \
        child = LYNX_RB_RIGHT(elm, field);                                                \
    else if (LYNX_RB_RIGHT(elm, field) == NULL)                                           \
        child = LYNX_RB_LEFT(elm, field);                                                 \
    else {                                                                                \
        struct type *left;                                                                \
        elm = LYNX_RB_RIGHT(elm, field);                                                  \
        while ((left = LYNX_RB_LEFT(elm, field)))                                         \
            elm = left;                                                                   \
        child = LYNX_RB_RIGHT(elm, field);                                                \
        parent = LYNX_RB_PARENT(elm, field);                                              \
        color = LYNX_RB_COLOR(elm, field);                                                \
        if (child)                                                                        \
            LYNX_RB_PARENT(child, field) = parent;                                        \
        if (parent) {                                                                     \
            if (LYNX_RB_LEFT(parent, field) == elm)                                       \
                LYNX_RB_LEFT(parent, field) = child;                                      \
            else                                                                          \
                LYNX_RB_RIGHT(parent, field) = child;                                     \
            LYNX_RB_AUGMENT(parent);                                                      \
        } else                                                                            \
            LYNX_RB_ROOT(head) = child;                                                   \
        if (LYNX_RB_PARENT(elm, field) == old)                                            \
            parent = elm;                                                                 \
        (elm)->rbe_left = (old)->rbe_left;                                                \
        (elm)->rbe_right = (old)->rbe_right;                                              \
        (elm)->rbe_parent = (old)->rbe_parent;                                            \
        (elm)->rbe_color = (old)->rbe_color;                                              \
        if (LYNX_RB_PARENT(old, field)) {                                                 \
            if (LYNX_RB_LEFT(LYNX_RB_PARENT(old, field), field) == old)                   \
                LYNX_RB_LEFT(LYNX_RB_PARENT(old, field), field) = elm;                    \
            else                                                                          \
                LYNX_RB_RIGHT(LYNX_RB_PARENT(old, field), field) = elm;                   \
            LYNX_RB_AUGMENT(LYNX_RB_PARENT(old, field));                                  \
        } else                                                                            \
            LYNX_RB_ROOT(head) = elm;                                                     \
        LYNX_RB_PARENT(LYNX_RB_LEFT(old, field), field) = elm;                            \
        if (LYNX_RB_RIGHT(old, field))                                                    \
            LYNX_RB_PARENT(LYNX_RB_RIGHT(old, field), field) = elm;                       \
        if (parent) {                                                                     \
            left = parent;                                                                \
            do {                                                                          \
                LYNX_RB_AUGMENT(left);                                                    \
            } while ((left = LYNX_RB_PARENT(left, field)));                               \
        }                                                                                 \
        goto color;                                                                       \
    }                                                                                     \
    parent = LYNX_RB_PARENT(elm, field);                                                  \
    color = LYNX_RB_COLOR(elm, field);                                                    \
    if (child)                                                                            \
        LYNX_RB_PARENT(child, field) = parent;                                            \
    if (parent) {                                                                         \
        if (LYNX_RB_LEFT(parent, field) == elm)                                           \
            LYNX_RB_LEFT(parent, field) = child;                                          \
        else                                                                              \
            LYNX_RB_RIGHT(parent, field) = child;                                         \
        LYNX_RB_AUGMENT(parent);                                                          \
    } else                                                                                \
        LYNX_RB_ROOT(head) = child;                                                       \
color:                                                                                    \
    if (color == LYNX_RB_BLACK)                                                           \
    name##_LYNX_RB_REMOVE_COLOR(head, parent, child);                                     \
    return (old);                                                                         \
}                                                                                         \
                                                                                          \
/* Inserts a node into the RB tree */                                                     \
struct type *                                                                             \
name##_LYNX_RB_INSERT(struct name *head, struct type *elm)                                \
{                                                                                         \
    struct type *tmp;                                                                     \
    struct type *parent = NULL;                                                           \
    int comp = 0;                                                                         \
    tmp = LYNX_RB_ROOT(head);                                                             \
    while (tmp) {                                                                         \
        parent = tmp;                                                                     \
        comp = (cmp)(elm, parent);                                                        \
        if (comp < 0)                                                                     \
            tmp = LYNX_RB_LEFT(tmp, field);                                               \
        else if (comp > 0)                                                                \
            tmp = LYNX_RB_RIGHT(tmp, field);                                              \
        else                                                                              \
            return (tmp);                                                                 \
    }                                                                                     \
    LYNX_RB_SET(elm, parent, field);                                                      \
    if (parent != NULL) {                                                                 \
        if (comp < 0)                                                                     \
            LYNX_RB_LEFT(parent, field) = elm;                                            \
        else                                                                              \
            LYNX_RB_RIGHT(parent, field) = elm;                                           \
        LYNX_RB_AUGMENT(parent);                                                          \
    } else                                                                                \
        LYNX_RB_ROOT(head) = elm;                                                         \
    name##_LYNX_RB_INSERT_COLOR(head, elm);                                               \
    return (NULL);                                                                        \
}                                                                                         \
                                                                                          \
/* Finds the node with the same key as elm */                                             \
struct type *                                                                             \
name##_LYNX_RB_FIND(struct name *head, struct type *elm)                                  \
{                                                                                         \
    struct type *tmp = LYNX_RB_ROOT(head);                                                \
    int comp;                                                                             \
    while (tmp) {                                                                         \
        comp = cmp(elm, tmp);                                                             \
        if (comp < 0)                                                                     \
            tmp = LYNX_RB_LEFT(tmp, field);                                               \
        else if (comp > 0)                                                                \
            tmp = LYNX_RB_RIGHT(tmp, field);                                              \
        else                                                                              \
            return (tmp);                                                                 \
    }                                                                                     \
    return (NULL);                                                                        \
}                                                                                         \
                                                                                          \
struct type *                                                                             \
name##_LYNX_RB_NEXT(struct type *elm) const                                               \
{                                                                                         \
    if (LYNX_RB_RIGHT(elm, field)) {                                                      \
        elm = LYNX_RB_RIGHT(elm, field);                                                  \
        while (LYNX_RB_LEFT(elm, field))                                                  \
            elm = LYNX_RB_LEFT(elm, field);                                               \
    } else {                                                                              \
        if (LYNX_RB_PARENT(elm, field) &&                                                 \
            (elm == LYNX_RB_LEFT(LYNX_RB_PARENT(elm, field), field)))                     \
            elm = LYNX_RB_PARENT(elm, field);                                             \
        else {                                                                            \
            while (LYNX_RB_PARENT(elm, field) &&                                          \
                (elm == LYNX_RB_RIGHT(LYNX_RB_PARENT(elm, field), field)))                \
                elm = LYNX_RB_PARENT(elm, field);                                         \
            elm = LYNX_RB_PARENT(elm, field);                                             \
        }                                                                                 \
    }                                                                                     \
    return (elm);                                                                         \
}                                                                                         \
struct type *                                                                             \
name##_LYNX_RB_PREV(struct type *elm) const                                               \
{                                                                                         \
    if (LYNX_RB_LEFT(elm, field)) {                                                       \
        elm = LYNX_RB_LEFT(elm, field);                                                   \
        while (LYNX_RB_RIGHT(elm, field))                                                 \
            elm = LYNX_RB_RIGHT(elm, field);                                              \
    } else {                                                                              \
        if (LYNX_RB_PARENT(elm, field) &&                                                 \
            (elm == LYNX_RB_RIGHT(LYNX_RB_PARENT(elm, field), field)))                    \
            elm = LYNX_RB_PARENT(elm, field);                                             \
        else {                                                                            \
            while (LYNX_RB_PARENT(elm, field) &&                                          \
                (elm == LYNX_RB_LEFT(LYNX_RB_PARENT(elm, field), field)))                 \
                elm = LYNX_RB_PARENT(elm, field);                                         \
            elm = LYNX_RB_PARENT(elm, field);                                             \
        }                                                                                 \
    }                                                                                     \
    return (elm);                                                                         \
}                                                                                         \
                                                                                          \
struct type *                                                                             \
name##_LYNX_RB_MINMAX(const struct name *head, int val) const                             \
{                                                                                         \
    struct type *tmp = LYNX_RB_ROOT(head);                                                \
    struct type *parent = NULL;                                                           \
    while (tmp) {                                                                         \
        parent = tmp;                                                                     \
        if (val < 0)                                                                      \
            tmp = LYNX_RB_LEFT(tmp, field);                                               \
        else                                                                              \
            tmp = LYNX_RB_RIGHT(tmp, field);                                              \
    }                                                                                     \
    return (parent);                                                                      \
}

#define LYNX_RB_NEGINF	-1
#define LYNX_RB_INF	1

#define LYNX_RB_INSERT(name, x, y)	name##_LYNX_RB_INSERT(x, y)
#define LYNX_RB_REMOVE(name, x, y)	name##_LYNX_RB_REMOVE(x, y)
#define LYNX_RB_FIND(name, x, y)	name##_LYNX_RB_FIND(x, y)
#define LYNX_RB_PREV(name, x, y)   name##_LYNX_RB_PREV(y)
#define LYNX_RB_NEXT(name, x, y)	name##_LYNX_RB_NEXT(y)
#define LYNX_RB_MIN(name, x)		name##_LYNX_RB_MINMAX(x, LYNX_RB_NEGINF)
#define LYNX_RB_MAX(name, x)		name##_LYNX_RB_MINMAX(x, LYNX_RB_INF)

#define LYNX_RB_FOREACH(x, name, head)                                                    \
    for ((x) = LYNX_RB_MIN(name, head);                                                   \
    (x) != NULL;                                                                          \
    (x) = name##_LYNX_RB_NEXT(x))


#ifdef __cplusplus
}
#endif

#endif // __LYNX_PLATFORM_LIB_ALGORITHM_H__



