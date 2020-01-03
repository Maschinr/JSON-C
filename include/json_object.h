#ifndef JSON_OBJECT_H
#define JSON_OBJECT_H
#include <hashmap.h>

typedef struct json_array_t json_array;
typedef struct json_object_t {
    map_t map;
} json_object;

extern json_object* json_object_create(void);
extern void json_object_free(json_object* object);

extern int json_object_get_string(const json_object* object, const char* name, char** result);
extern int json_object_add_string(json_object* object, const char* name, const char* value);
extern int json_object_insert_string(json_object* object, const char* name, const char* value);
extern int json_object_change_string(json_object* object, const char* name, const char* value); 

extern int json_object_get_number(const json_object* object, const char* name, int* result);
extern int json_object_add_number(json_object* object, const char* name, const int value);
extern int json_object_insert_number(json_object* object, const char* name, const int value);
extern int json_object_change_number(json_object* object, const char* name, const int value); 

extern int json_object_get_float_number(const json_object* object, const char* name, double* result);
extern int json_object_add_float_number(json_object* object, const char* name, const double value);
extern int json_object_insert_float_number(json_object* object, const char* name, const double value);
extern int json_object_change_float_number(json_object* object, const char* name, const double value);

extern int json_object_get_object(const json_object* object, const char* name, json_object** result);
extern int json_object_add_object(json_object* object, const char* name, const json_object* value);
extern int json_object_insert_object(json_object* object, const char* name, const json_object* value);

extern int json_object_get_array(const json_object* object, const char* name, json_array** result);
extern int json_object_add_array(json_object* object, const char* name, const json_array* value);
extern int json_object_insert_array(json_object* object, const char* name, const json_array* value);

extern int json_object_remove(json_object* object, const char* name);

#define json_object_get(object, name, result) _Generic((result),\
    int*: json_object_get_number,\
    double*: json_object_get_float_number,\
    char**: json_object_get_string,\
    json_object**: json_object_get_object,\
    json_array**: json_object_get_array\
) (object, name, result)

#define json_object_add(object, name, value) _Generic((value),\
    int: json_object_add_number,\
    double: json_object_add_float_number,\
    char*: json_object_add_string,\
    json_object**: json_object_add_object,\
    json_array**: json_object_add_array\
) (object, name, value)

#define json_object_insert(object, name, value) _Generic((value),\
    int: json_object_insert_number,\
    double: json_object_insert_float_number,\
    char*: json_object_insert_string,\
    json_object**: json_object_insert_object,\
    json_array**: json_object_insert_array\
) (object, name, value)

#define json_object_change(object, name, value) _Generic((value),\
    int: json_object_change_number,\
    double: json_object_change_float_number,\
    char*: json_object_change_string\
) (object, name, value)

#endif