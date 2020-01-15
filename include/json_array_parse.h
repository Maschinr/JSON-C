#ifndef JSON_ARRAY_PARSE_H
#define JSON_ARRAY_PARSE_H
#include "json_array.h"

extern json_array* json_array_from_file(const char* path);
extern json_array* json_array_from_str(const char* string);

extern int json_array_to_file(const json_array* object, const char* path);
extern char* json_array_to_str(const json_array* object);

extern int parse_array(unsigned int begin, const char* str, json_array* obj, unsigned int* end);

#endif