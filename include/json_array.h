#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H
#include "json_value.h"

typedef struct json_array_t {
    unsigned int size;
    unsigned int memory_size;
    json_value* values; // Value array
} json_array;

#endif