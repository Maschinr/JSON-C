/* Header Information
 * In this header a typesafe dynamically sized array is implemented.
 * Because this implementation should also be able to use dynamic
 * types the complete Array implementation is created in one Macro,
 * with this solution an Array of a new Type can be created by
 * just calling the macro in this Header with the new Type.
 */
#ifndef ARRAY_H
#define ARRAY_H
#include <stdlib.h>

#define ARRAY_TYPES\
    TYPE(int)

#define TYPE(type)\
typedef struct type##_array_t {\
    unsigned int size;\
    unsigned int mem_size;\
    type *data;\
} type##_array;\
\
extern type##_array* type##_array_create(unsigned int start_size);\
extern void type##_array_free(type##_array* array);

ARRAY_TYPES;

#undef TYPE

#endif