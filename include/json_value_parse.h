#ifndef JSON_VALUE_PARSE_H
#define JSON_VALUE_PARSE_H
#include "json_value.h"

extern char* json_value_to_str(json_value* value);
extern json_value* parse_value(const char* name, const char* str, unsigned int begin, unsigned int* end);
#endif