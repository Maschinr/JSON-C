/* Header Information
 * In this header a typesafe linked list is implemented.
 * Because this implementation should also be able to use dynamic
 * types the complete Linked List implementation is created in one Macro,
 * with this solution an Linked List of a new Type can be created by
 * just calling the macro in this Header with the new Type.
 */
#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include <stdlib.h>
#include <stdbool.h>

typedef struct json_value_t json_value;

#define LINKED_LIST_TYPES\
    TYPE(int, int)\
    TYPE(json_value*, json_value)

#define TYPE(type, name)\
typedef struct name##_list_element_t {\
    type data;\
    struct name##_list_element_t *next;\
} name##_list_element;\
\
typedef struct name##_list_t {\
    unsigned int size;\
    struct name##_list_element_t *first;\
} name##_list;\
\
extern name##_list* name##_list_create();\
extern void name##_list_free(name##_list* list);\
extern void name##_list_add_end(name##_list* list, type value);\
extern void name##_list_add_first(name##_list* list, type value);\
extern bool name##_list_add(name##_list* list, type value, unsigned int position);\
extern type* name##_list_get(name##_list* list, unsigned int position);

LINKED_LIST_TYPES;

#undef TYPE

#endif