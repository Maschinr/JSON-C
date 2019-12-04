#include <json.h>
#include <stdio.h>

json_value* json_value_create(void* data, json_value_type type) {
    json_value* result;

    result = malloc(sizeof(json_value));
    
    if(result == NULL) {
        return NULL;
    }

    result->type = type;

    //TODO cleaner way?
    switch(result->type) {
        case JSON_INT:
            result->byte_size = sizeof(int);
            result->value = malloc(sizeof(int));
            memcpy(result->value, data, sizeof(int));
            break;
    }

    return result;
}

void json_value_free(json_value* value) {
    if(value == NULL) {
        return;
    }

    free(value->value);
    free(value);
}

json_object_value* json_object_value_create(const char* name, void* data, json_value_type type) {
    json_object_value* result;

    //TODO check if valid name
    if(strstr(name, "\"") != NULL) {
        return NULL;
    }

    result = malloc(sizeof(json_object_value));

    if(result == NULL) {
        return NULL;
    }

    result->name = NULL;
    result->next = NULL;
    result->value = NULL;

    result->name = malloc(sizeof(char) * strlen(name));

    if(result->name == NULL) {
        free(result);
        return NULL;
    }

    strcpy(result->name, name);

    result->value = json_value_create(data, type);
    
    if(result->value == NULL) {
        free(result->name);
        free(result);
        return NULL;
    }

    return result;
}

void json_object_value_free(json_object_value* value) {
    if(value == NULL) {
        return;
    }

    free(value->name);
    json_value_free(value->value);
    free(value);
}

json_object* json_object_create(void) {
    json_object* result;

    result = malloc(sizeof(json_object));

    if(result == NULL) {
        return NULL;
    }

    result->first = NULL;

    return result;
}

json_object* json_object_from_str(const char* str) {
    return NULL;
}

//try to convert value to type and return it if possible, the caller then casts it
void* json_value_convert(json_value* value, json_value_type type) {
    void* result;

    if (value == NULL) {
        return NULL;
    }
   
    result = NULL;

    switch(type) { // To convert to
        case JSON_STRING: { // Convert to string
            switch(value->type) { // from
                case JSON_STRING: { // String, just strcpy
                    result = malloc(strlen(value->value) + 1); // + 1 for\0
                    strcpy(result, value->value);
                    return 0;
                }

                case JSON_CHAR:
                case JSON_SHORT:
                case JSON_INT:
                case JSON_LONG:
                case JSON_LONG_LONG:
                case JSON_FLOAT:
                case JSON_DOUBLE: { // All numbers with sprintf
                    char number[32];
                    unsigned int size;
                    if(value->type == JSON_CHAR) {
                        sprintf(number, "%c", *((char*)value->value));
                    } else if(value->type == JSON_SHORT) {
                        sprintf(number, "%hi", *((short*)value->value));
                    } else if(value->type == JSON_INT) {
                        sprintf(number, "%i", *((int*)value->value));
                    } else if(value->type == JSON_LONG) {
                        sprintf(number, "%li", *((long*)value->value));
                    } else if(value->type == JSON_LONG_LONG) {
                        sprintf(number, "%lli", *((long long*)value->value));
                    } else if(value->type == JSON_FLOAT) {
                        sprintf(number, "%f", *((float*)value->value));
                    } else if(value->type == JSON_DOUBLE) {
                        sprintf(number, "%g", *((double*)value->value));
                    }
                    size = strlen(number);
                    result = malloc(sizeof(char) * (size + 1));
                    if(result == NULL) {
                        return NULL;
                    }
                    memset(result, 0, size);
                    strncpy(result, number, size);  
                    ((char*)result)[size] = '\0';
                    return result;
                }
                default: {
                    return NULL;
                }
            }
            break;
        }

        case JSON_INT: { // convert to int
            result = malloc(sizeof(int));
            if(result == NULL) {
                return NULL;
            }
            switch(value->type) {
                case JSON_INT: {
                    //It's the same do memcpy
                    memcpy(result, value->value, sizeof(int));
                    return result;
                }

                default: {
                    free(result);
                    result = NULL;
                    return NULL;
                }
            }
            break;
        }

        case JSON_FLOAT: { // convert to float
            result = malloc(sizeof(float));
            switch(value->type) {
                case JSON_INT: {
                    float res = (float)*(int*)value->value;
                    memcpy(result, &res, sizeof(float));
                    return result;
                }

                case JSON_FLOAT: {
                    memcpy(result, value->value, sizeof(float));
                    return result;
                }

                default: {
                    free(result);
                    result = NULL;
                    return NULL;
                }
            }
       
            break;
        }

        default: {
            return NULL;;
        }
    }

    return NULL;
}

char* json_value_to_str(json_value* value, int formatted) {
    /*Create the string representation of the value*/
    char* result;
    result = NULL;
    switch(value->type) {
        case JSON_OBJECT: {
            result = json_object_to_str(value->value, formatted);
            break;     
        } 
        default: {
            result = json_value_convert(value, JSON_STRING);
            break;
        }
    }
    
    return result;
}

char* json_object_value_to_str(json_object_value* value, int formatted) {
    /*Create the string "name": + json_value_to_str() */
    char* result;
    unsigned int size;
    if(value == NULL) {
        return NULL;
    }

    char* value_str = json_value_to_str(value->value, formatted); // TODO change to convert func
    if(value_str == NULL) {
        return NULL;
    }

    size = 4 + strlen(value->name) + strlen(value_str); // 4 for the 2 "" the : and the \0

    result = malloc(sizeof(char) * size);
    if(result == NULL) {
        return NULL;
    }
    memset(result, 0, size);
    result[0] = '\"';
    strcat(result, value->name);
    strcat(result, "\":");
    strcat(result, value_str); // value string is null terminated so that terminates this string automatically
    free(value_str);

    return result;
}

//TODO work on this, implement formatting, implement all types
char* json_object_to_str(const json_object* object, int formatted) {
    char* result;
    json_object_value* element;
    unsigned int size;
    char number[32]; //Enough for all possible numeric values that could be read

    if(object == NULL) {
        return NULL;
    }

    result = malloc(sizeof(char) * 3);
    if(result == NULL) {
        return NULL;
    }
    size = 3;
    element = object->first;

    result[0] = '{';
    result[1] = 0;
    result[2] = 0;

    while(element != NULL) {
        char* value_str = json_object_value_to_str(element, formatted); // "name":value
    
        if(value_str == NULL) {
            free(result);
            return NULL;
        }

        size = size + strlen(value_str);

        if(element->next != NULL) {
            size = size + 1; // For the comma
        }

        void* tmp = realloc(result, size);
        if(tmp == NULL) {
            free(result);
            return NULL;
        }
        result = tmp;

        strcat(result, value_str);
        free(value_str);

        if(element->next != NULL) {
            strcat(result, ",");
        }

        element = element->next;
    }
    strcat(result, "}\0");

    return result;
}

void json_object_free(json_object* object) {
    json_object_value* element;
    json_object_value* next_element;
    if(object == NULL) {
        return;
    }

    element = object->first;
    next_element = NULL;

    while(element != NULL) {
        next_element = element->next;

        json_object_value_free(element);

        element = next_element;
    }

    free(object);
}

int json_object_get_value(const json_object* object, const char* name, json_object_value** result) {
    if(object == NULL) {
        return -1;
    }

    (*result) = object->first;
    while((*result) != NULL) {
        if(strcmp((*result)->name, name) == 0) {
            return 0;
        }

        *result = (*result)->next;
    }

    return -1;
}

int json_object_remove(json_object* object, const char* name) {
    json_object_value* element;
    json_object_value* last_element;

    if(object == NULL) {
        return -1;
    }

    element = object->first;
    last_element = NULL;

    while(element != NULL) {
        if(strcmp(element->name, name) == 0) {
            //Element found delete it and set last_element next to element next if last element is not null, otherwise set object first to null

            if(last_element != NULL) {
                last_element->next = element->next;
            } else {
                object->first = NULL;
            }

            free(element);

            return 0;
        }

        last_element = element;
        element = element->next;
    }

    return -1;
}

//Implementation for types
#define JSON_INTERNAL_MACRO(m_type, m_type_enum)\
    int json_object_get_##m_type(const json_object* object, const char* name, m_type *result) {\
        json_object_value* element;\
        void* tmp;\
        if(object == NULL || result == NULL) {\
            return -1;\
        }\
\
        element = NULL;\
\
        if(json_object_get_value(object, name, &element) == 0) {\
            tmp = json_value_convert(element->value, m_type_enum);\
            if(tmp != NULL) {\
                *result = *(m_type*)tmp;\
                return 0;\
            }\
            return -1;\
        }\
        return -1;\
    }\
\
    int json_object_add_##m_type(json_object* object, const char* name, const m_type value) {\
        json_object_value* element;\
        json_object_value* last_element;\
        int* data;\
        m_type tmp;\
\
        if(object == NULL) {\
            return -1;\
        }\
\
        if(json_object_get_##m_type(object, name, &tmp) == 0) {\
            return -1;\
        }\
\
        data = malloc(sizeof(m_type));\
\
        if(data == NULL) {\
            return -1;\
        }\
\
        *data = value;\
        if(object->first == NULL) {\
            object->first = json_object_value_create(name, data, m_type_enum);\
            if(object->first == NULL) {\
                free(data);\
                return -1;\
            }\
            return 0;\
        }\
        element = object->first;\
        last_element = NULL;\
\
        while(element != NULL) {\
            if(strcmp(element->name, name) == 0) {\
                return -1;\
            }\
            last_element = element;\
            element = element->next;\
        }\
\
        last_element->next = json_object_value_create(name, data, m_type_enum);\
        if(last_element->next == NULL) {\
            free(data);\
            return -1;\
        }\
        return 0;\
    }\
\
    int json_object_insert_##m_type(json_object* object, const char* name, const m_type value) {\
\
        if(json_object_add_##m_type(object, name, value) != 0) {\
            return json_object_change_##m_type(object, name, value);\
        }\
\
        return 0;\
    }\
\
    int json_object_change_##m_type(const json_object* object, const char* name, const m_type value) {\
        json_object_value* element;\
\
        if(object == NULL) {\
            return -1;\
        }\
\
        if(json_object_get_value(object, name, &element) == 0) {\
\
            if(element->value == NULL) {\
                return -1;\
            }\
\
            if(element->value->type != m_type_enum) {\
                return -1;\
            }\
\
            if(sizeof(value) != element->value->byte_size) {\
                return -1;\
            }\
\
            memcpy(element->value->value, &value, element->value->byte_size);\
\
            return 0;\
        }\
\
        return -1;\
    }

JSON_INTERNAL_TYPES

#undef JSON_INTERNAL_MACRO
