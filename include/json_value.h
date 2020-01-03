#ifndef JSON_VALUE_H
#define JSON_VALUE_H

typedef enum json_value_type_e {
    JSON_STRING,
    JSON_NUMBER,
    JSON_FLOAT_NUMBER,
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_NULL
} json_value_type;

typedef struct json_value_t {
    json_value_type type;
    void* value;
    char* name;
} json_value;

extern json_value* json_value_create(const char* name, void* data, unsigned int data_size, json_value_type type);
extern void json_value_free(json_value* value);

extern void* json_value_convert(json_value* value, json_value_type type);

#endif