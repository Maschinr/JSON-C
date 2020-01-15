#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H
#include "json_value.h"

typedef struct json_array_t {
    unsigned int size;
    unsigned int memory_size;
    json_value** values; // Value array
} json_array;

extern json_array* json_array_create(void);
extern json_array* json_array_copy(json_array* array);
extern void json_array_iterate(json_array* array, void (*func)(json_value* value));
extern void json_array_free(json_array* array);
extern int json_array_add_json_value(json_array* array, json_value* value, int index);

extern int json_array_get_string(const json_array* array, const unsigned int index, char** result);
extern int json_array_add_string(json_array* array, char* value);
extern int json_array_change_string(json_array* array, const unsigned int index, char* value); 

extern int json_array_get_number(const json_array* array, const unsigned int index, int* result);
extern int json_array_add_number(json_array* array, int value);
extern int json_array_change_number(json_array* array, const unsigned int index, int value); 

extern int json_array_get_float_number(const json_array* array, const unsigned int index , double* result);
extern int json_array_add_float_number(json_array* array, double value);
extern int json_array_change_float_number(json_array* array, const unsigned int index,  double value);

extern int json_array_get_object(const json_array* array, const unsigned int index, json_object** result);
extern int json_array_add_object(json_array* array, json_object* value);

extern int json_array_get_array(const json_array* array, const unsigned int index, json_array** result);
extern int json_array_add_array(json_array* array, json_array* value);

extern void json_array_remove(json_array* array, const unsigned int index);

#define json_array_get(array, index, result) _Generic((result),\
    int*: json_array_get_number,\
    double*: json_array_get_float_number,\
    char**: json_array_get_string,\
    json_object**: json_array_get_object,\
    json_array**: json_array_get_array\
) (array, index, result)

#define json_array_add(array, value) _Generic((value),\
    int: json_array_add_number,\
    double: json_array_add_float_number,\
    char*: json_array_add_string,\
    json_object*: json_array_add_object,\
    json_array*: json_array_add_array\
) (array, value)

#define json_array_change(array, name, value) _Generic((value),\
    int: json_array_change_number,\
    double: json_array_change_float_number,\
    char*: json_array_change_string\
) (array, name, value)

#endif