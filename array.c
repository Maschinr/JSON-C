#include <array.h>

#define TYPE(type)\
type##_array* type##_array_create(unsigned int start_size) {\
    type##_array* t;\
    t = NULL;\
    t = malloc(sizeof(type##_array));\
    if(t != NULL) {\
        t->data = NULL;\
        t->data = malloc(sizeof(type) * start_size);\
        if(t->data != NULL) {\
            t->mem_size = start_size;\
            t->size = 0;\
        } else {\
            free(t);\
            t = NULL;\
        }\
    }\
    \
    return t;\
}\
\
void type##_array_free(type##_array* array) {\
    free(array->data);\
    free(array);\
}\
\
void type##_array_add(type value) {\
\
}


ARRAY_TYPES;

#undef TYPE