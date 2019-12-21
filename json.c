#include <json.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

int created = 0;
int freed = 0;

void* test_malloc(size_t size) {
    created = created + 1;
    return malloc(size);
}

void test_free(void* ptr) {
    freed = freed + 1;
    free(ptr);
}

void TMP_DELETE() {
    printf("Created: %i Freed: %i\n", created, freed);
}

//!HINT Does not copy data, data needs to alloced beforehand because of reasons
json_value* json_value_create(void* data, json_value_type type, const char* name) {
    json_value* result;

    result = test_malloc(sizeof(json_value));
    
    if(result == NULL) {
        return NULL;
    }

    result->type = type;
    result->name = test_malloc(strlen(name) + 1);
    strcpy(result->name, name);

    /*switch(result->type) {
        case JSON_INT: {
            result->byte_size = sizeof(int);
            break;
        }
        case JSON_STRING: {
            result->byte_size = sizeof(strlen(data) + 1);
            break;
        }
    }*/

    result->value = data;

    return result;
}

void json_value_free(json_value* value) {
    if(value == NULL) {
        return;
    }

    test_free(value->value);
    test_free(value->name);
    test_free(value);
}

json_object* json_object_create(void) {
    json_object* result;

    result = test_malloc(sizeof(json_object));

    if(result == NULL) {
        return NULL;
    }

    result->map = hashmap_new();

    return result;
}

int free_hashmap(void* null, void* val) {
    json_value_free((json_value*)val);
    return MAP_OK;
}

void json_object_free(json_object* object) {
    if(object == NULL) {
        return;
    }

    hashmap_iterate(object->map, free_hashmap, NULL);

    hashmap_free(object->map);
    test_free(object);
}

char* parse_string(unsigned int begin, const char* str, unsigned int* end) {
    const unsigned int str_length = strlen(str);
   
    for(unsigned int i = begin; i < str_length; i++) {
        if(str[i] != ' ') {
            if(str[i] == '\"') {
                i = i + 1;
                char buffer[100];
                memset(&buffer, '\0', 100);
                for(unsigned int ix = 0; ix < 100; ix++) {
                    if(str[i + ix] == '\"') { // string ended
                        *end = i + ix + 1;
                        char* res = test_malloc(sizeof(char) * strlen(buffer) + 1);
                        if(res == NULL) {
                            return NULL;
                        }
                        strcpy(res, buffer);
                        return res;
                    } else if(str[i + ix] == '\\') { //if backslash then look if valid char is after
                        if(str[i + ix + 1] == '\"') {
                            buffer[ix] = '\"';
                        } else if(str[i + ix + 1] == '\\') {
                            buffer[ix] = '\\';
                        } else if(str[i + ix + 1] == '/') {
                            buffer[ix] = '/';
                        } else if(str[i + ix + 1] == 'b') {
                            buffer[ix] = '\b';
                        } else if(str[i + ix + 1] == 'f') {
                            buffer[ix] = '\f';
                        } else if(str[i + ix + 1] == 'n') {
                            buffer[ix] = '\n';
                        } else if(str[i + ix + 1] == 'r') {
                            buffer[ix] = '\r';
                        } else if(str[i + ix + 1] == 't') {
                            buffer[ix] = '\t';
                        } else if(str[i + ix + 1] == 'u') {
                            return NULL; // TODO
                            //buffer[ix] = '\u';//todo format to hexadecimal
                        }  else {
                            return NULL;
                        }
                            
                        i = i + 1; // skip one char
                        
                    } else { // any other char add to buffer
                        buffer[ix] = str[i + ix];
                    }
                }
            } else {
                return NULL;
            }
        }
    }
}

json_value* parse_value(const char* name, unsigned int begin, const char* str, unsigned int* end) {
    const unsigned int str_length = strlen(str);
    for(unsigned int i = begin; i < str_length; i++) {
        if(str[i] != ' ') {
            //Check which type of value it is
            if(str[i] == '{') { // it's an object
                //json_object* obj = parse_object();
            } else if(str[i] == '\"') { // it's an string
                char* string = parse_string(i, str, end);
                json_value* obj = json_value_create(string, JSON_STRING, name);
                return obj;
                //to object value
            } else if(str[i] == '-' || isdigit(str[i])) { // it's an number
                char number[100];
                memset(number, 0, 100);
                int doublevalue = 0;
                for(unsigned int ix = 0; ix + i< str_length; ix++) {
                    //go through each numbers
                    if(str[ix + i] == '.') {
                        doublevalue = 1;
                    }
                    if(str[ix + i] == '-' || isdigit(str[ix + i]) || str[ix + i] == '.' || str[ix + i] == '+') {
                        number[ix] = str[ix + i];
                    } else {
                        //end reached or error
                        *end = ix + i + 1;
                        //try to parse
                        if(doublevalue == 1) {
                            //double
                        } else {
                            //int todo long int
                            errno = 0;
                            int num_int = strtol(number, NULL, 10);
                        
                            if(errno == 0) {
                                // valid conversion
                                //create new memory for int and copy
                                int* res = test_malloc(sizeof(int));
                                memcpy(res, &num_int, sizeof(int));
                                json_value* obj = json_value_create(res, JSON_INT, name);
                               
                                return obj;
                            }
                        }
                    }
                }
            }
        }
    }
}

int parse_object(unsigned int begin, const char* str, json_object* obj) {
    //stops after parsing first object after begin
    const unsigned int str_length = strlen(str);
    if(begin > str_length) return 1;
    if(obj == NULL) return 1;
    unsigned int position = begin;
   
    for(position; position < str_length; position++) {
        if(str[position] != ' ') { // Skip whitespace
            if(str[position] == '{' || position + 1 < str_length) {
                // valid object continue
                position = position + 1; // skip cur char
                break;
            } else {
                //invalid object
                return 1;
            }
        }
    }
    //now iterate over every value aslong as } is not reached
    for(position; position < str_length; position++) {
        if(str[position] == '}') {
            //object ended
            break;
        }
        
        // Parse value('s)
        unsigned int end = 0;
        char* name = parse_string(position, str, &end); // Get object name
        if(name == NULL) {
            return 1;
        }
        position = end;
        json_value* val = parse_value(name, position, str, &end);
        test_free(name);
        position = end; 

        if(val == NULL) {
            return 1;
        }
        //Insert value into object
        json_value* ret = NULL;
        if(hashmap_get(obj->map, val->name, (void**)(&ret)) == MAP_OK) {
            return 1;
        }

        if(hashmap_put(obj->map, val->name, val) != MAP_OK) {
            //Insert errors
            json_value_free(val);
            return 1;
        }

        if(hashmap_get(obj->map, val->name, (void**)(&ret)) == MAP_OK) {
            printf("Value added %s %s\n", val->name, val->value);
        }

        
    }

    return 0;
}

json_object* json_object_from_str(const char* str) {
    
    json_object* result = json_object_create();
    if(parse_object(0, str, result) != 0) {
        printf("Parsing error\n");
        json_object_free(result);
        result = NULL;
    }

    return result;
}

#define CONVERT_TO(to, from)\
    {to res = (to)*(from*)value->value;\
    memcpy(result, &res, sizeof(to));\
    return result;}
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
                    char* result = test_malloc(strlen(value->value) + 1); // + 1 for\0
                    if(result == NULL) {
                        return NULL;
                    }
                    strcpy(result, value->value);
                 
                    return result;
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
                    memset(number, '\0', 32);
                    
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
                        sprintf(number, "%g", *((float*)value->value));
                    } else if(value->type == JSON_DOUBLE) {
                        sprintf(number, "%g", *((double*)value->value));
                    }
                    size = strlen(number);
                    char* result = test_malloc(sizeof(char) * (size + 1));
                    
                    if(result == NULL) {
                        return NULL;
                    }

                    memset(result, '\0', size);
                    strcpy(result, number);
                    return result;
                }
                default: {
                    return NULL;
                }
            }
            break;
        }

        case JSON_INT: { // convert to int
            result = test_malloc(sizeof(int));
            if(result == NULL) {
                return NULL;
            }
            switch(value->type) { //from
                case JSON_INT: {
                    //It's the same do memcpy
                    memcpy(result, value->value, sizeof(int));
                    return result;
                }

                case JSON_FLOAT: CONVERT_TO(int, float)
                case JSON_DOUBLE: CONVERT_TO(int, double)

                default: {
                    test_free(result);
                    result = NULL;
                    return NULL;
                }
            }
            break;
        }

        case JSON_FLOAT: { // convert to float
            result = test_malloc(sizeof(float));
            switch(value->type) {

                case JSON_FLOAT: {
                    memcpy(result, value->value, sizeof(float));
                    return result;
                }

                case JSON_INT: CONVERT_TO(float, int)
                case JSON_DOUBLE: CONVERT_TO(float, double)

                default: {
                    test_free(result);
                    result = NULL;
                    return NULL;
                }
            }
       
            break;
        }

        case JSON_DOUBLE: { // convert to double
            result = test_malloc(sizeof(double));
            switch(value->type) {
                
                case JSON_DOUBLE: {
                    memcpy(result, value->value, sizeof(double));
                    return result;
                }

                case JSON_INT: CONVERT_TO(double, int)
                case JSON_FLOAT: CONVERT_TO(double, float)

                default: {
                    test_free(result);
                    result = NULL;
                    return NULL;
                }
            }
       
            break;
        }

        default: {
            return NULL;
        }
    }

    return NULL;
}

int json_object_value_to_str_iterator(void* cont, void* val);

typedef struct iterator_container_t {
    char* text;
    int prev_elem;
} iterator_container;

char* json_object_to_str(const json_object* object) {
    char* result;
    if(object == NULL) {
        return NULL;
    }

    if(hashmap_length(object->map) > 0) {
        result = test_malloc(sizeof(char) * 2); // {} the third for \0 will be set by hashmap_iterate
        if(result == NULL) {
            return NULL;
        }

        result[0] = '{';
        result[1] = '\0';

        iterator_container ct;
        ct.text = result;
        ct.prev_elem = 0;
        printf("Begin ptr %p\n", result);
        if(hashmap_iterate(object->map, json_object_value_to_str_iterator, &ct) != MAP_OK) {
            test_free(result);
            return NULL;
        }
        result = ct.text;
        printf("End ptr %p\n", result);
        strcat(result, "}");
    } else {
        result = test_malloc(sizeof(char) * 3); // {} the third for \0 will be set by hashmap_iterate
        if(result == NULL) {
            return NULL;
        }

        result[0] = '{';
        result[1] = '}';
        result[2] = '\0';
       
    }
    return result;
}

char* json_value_to_str(json_value* value) {
    /*Create the string representation of the value*/
    char* result;
    char* value_str;
    unsigned int size;

    if(value == NULL) {
        return NULL;
    }

    if(value->name != NULL) {
        size = 4 + strlen(value->name);
    }
    
    result = NULL;
    value_str = NULL;

    switch(value->type) {
        case JSON_OBJECT: {
            value_str = json_object_to_str(value->value);
            break;     
        }
        case JSON_STRING: {
            value_str = json_value_convert(value, JSON_STRING);
            char* tmp = test_malloc(strlen(value_str) + 3); // 3 for "" and \}
            memset(tmp, '\0', strlen(value_str) + 3);
            strcat(tmp, "\"");
            strcat(tmp, value_str);
            strcat(tmp, "\"\0");
            test_free(value_str);
            value_str = tmp;
            break;
        }
        default: {
            value_str = json_value_convert(value, JSON_STRING);
            break;
        }
    }
    size = size + strlen(value_str);
    
    result = test_malloc(sizeof(char) * size);
    if(result == NULL) {
        return NULL;
    }
   

    memset(result, '\0', size);
    if(value->name != NULL) {
        result[0] = '\"';
        strcat(result, value->name);
        strcat(result, "\":");
    }
    strcat(result, value_str); // value string is null terminated so that terminates this string automatically
    test_free(value_str);
    return result;
}

//Implementation of map iterator func for parsing
int json_object_value_to_str_iterator(void* cont, void* val) {
    unsigned int size = 0;
    char* value_str = json_value_to_str((json_value*)val); // "name":value
    if(value_str == NULL) {
        return MAP_MISSING; // Error
    }

    iterator_container* ct = (iterator_container*)cont;
    size = strlen(ct->text) + strlen(value_str);

    if(ct->prev_elem != 0) {
        size = size + 1; // For the comma
    }
    void* tmp = realloc(ct->text, size + 1); // +1 for the \0
    if(tmp == NULL) {
        return MAP_MISSING; // Error
    }
    printf("realloc ptr %p to %p\n", ct->text, tmp);
    ct->text = tmp;
    
    if(ct->prev_elem != 0) {
        strcat(ct->text, ",");
    }
    strcat(ct->text, value_str);
    test_free(value_str);
    ct->prev_elem = 1;
    return MAP_OK;
}

int json_object_remove(json_object* object, const char* name) {
    json_value* ret;\
    if(hashmap_get(object->map, name, (void**)(&ret)) == MAP_OK) {
        hashmap_remove(object->map, name);
        json_value_free(ret);
        return 0;
    }

    return 1;
}

//Implementation for types
#define JSON_INTERNAL_MACRO(m_name, m_type, m_type_enum)\
    /*return value needs to be freed by user*/\
    int json_object_get_##m_name(const json_object* object, const char* name, m_type* result) {\
        json_value* element = NULL;\
        void* tmp;\
        if(object == NULL || result == NULL) {\
            return 1;\
        }\
\
        element = NULL;\
        if(hashmap_get(object->map, name, (void**)(&element)) != MAP_OK) {\
            return 1;\
        }\
        tmp = json_value_convert(element, m_type_enum);\
        if(tmp != NULL) {\
            printf("%s tmp is\n", *(m_type*)tmp);\
            printf("%s\n", *result);\
            *result = *(m_type*)tmp;\
            printf("%s\n", *result);\
            test_free(tmp);\
            return 0;\
        }\
        return 1;\
    }\
\
    int json_object_add_##m_name(json_object* object, const char* name, const m_type value) {\
        m_type* data;\
        json_value* element;\
        if(hashmap_get(object->map, name, (void**)(&element)) == MAP_OK) {/*Value already exists*/\
            return 1;\
        }\
\
        data = test_malloc(sizeof(m_type));\
\
        if(data == NULL) {\
            return 1;\
        }\
\
        *data = value;\
\
        json_value* val = json_value_create(data, m_type_enum, name);\
        if(val == NULL) {\
            test_free(data);\
            return 1;\
        }\
        if(hashmap_put(object->map, name, val) != MAP_OK) {\
            json_value_free(val);\
            return 1;\
        }\
\
        return 0;\
    }\
\
    int json_object_insert_##m_name(json_object* object, const char* name, const m_type value) {\
\
        if(json_object_add_##m_name(object, name, value) != 0) {\
            return json_object_change_##m_name(object, name, value);\
        }\
\
        return 0;\
    }\
\
    int json_object_change_##m_name(json_object* object, const char* name, const m_type value) {\
        m_type* data;\
\
        data = test_malloc(sizeof(m_type));\
\
        if(data == NULL) {\
            return 1;\
        }\
\
        *data = value;\
\
        json_object_remove(object, name);\
\
        return json_object_add_##m_name(object, name, value);\
    }

JSON_INTERNAL_TYPES

#undef JSON_INTERNAL_MACRO

//String implementation
int json_object_get_string(const json_object* object, const char* name, char** result) {
    json_value* element = NULL;
    void* tmp;
    if(object == NULL || result == NULL) {
        return 1;
    }

    element = NULL;
    if(hashmap_get(object->map, name, (void**)(&element)) != MAP_OK) {
        return 1;
    }

    tmp = json_value_convert(element, JSON_STRING);

    if(tmp != NULL) {
        *result = tmp;
        return 0;
    }
    return 1;
}

int json_object_add_string(json_object* object, const char* name, const char* value) {
    char* data;
    json_value* element;
    if(hashmap_get(object->map, name, (void**)(&element)) == MAP_OK) {/*Value already exists*/
        return 1;
    }

    data = test_malloc(strlen(value) + 1);

    if(data == NULL) {
        return 1;
    }

    strcpy(data, value);

    json_value* val = json_value_create(data, JSON_STRING, name);
    if(val == NULL) {
        test_free(data);
        return 1;
    }

    if(hashmap_put(object->map, name, val) != MAP_OK) {
        json_value_free(val);
        return 1;
    }

    return 0;
}

int json_object_insert_string(json_object* object, const char* name, const char* value) {

    if(json_object_add_string(object, name, value) != 0) {
        return json_object_change_string(object, name, value);
    }

    return 0;
}

int json_object_change_string(json_object* object, const char* name, const char* value) {\
    char* data;

    data = test_malloc(strlen(value) + 1);

    if(data == NULL) {
        return 1;
    }

    strcpy(data, value);

    json_object_remove(object, name);

    return json_object_add_string(object, name, value);
}