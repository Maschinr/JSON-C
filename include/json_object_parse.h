#ifndef JSON_OBJECT_PARSE_H
#define JSON_OBJECT_PARSE_H
#include "json_object.h"

extern json_object* json_object_from_file(const char* path);
extern json_object* json_object_from_str(const char* string);

extern int json_object_to_file(const json_object* object, const char* path);
extern char* json_object_to_str(const json_object* object);

#endif