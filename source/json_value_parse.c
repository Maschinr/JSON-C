#include <json_value_parse.h>
#include <json_object.h>
#include <json_object_parse.h>
#include <json_array_parse.h>
#include <json_parse.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>

char* json_value_to_str(json_value* value) {
    char* result;
    char* value_str;
    unsigned int size;

    if(value == NULL) {
        return NULL;
    }

    if(value->name != NULL) {
        size = 3 + strlen(value->name);
    }
    
    result = NULL;
    value_str = NULL;

    switch(value->type) {
        case JSON_OBJECT: {
            value_str = json_object_to_str((json_object*)value->value);
            break;     
        }
        case JSON_STRING: {
            value_str = json_value_convert(value, JSON_STRING);
            char* tmp = malloc(strlen(value_str) + 3);
            if(tmp == NULL) {
                return NULL;
            }
            memset(tmp, '\0', strlen(value_str) + 3);
            strcat(tmp, "\"");
            strcat(tmp, value_str);
            strcat(tmp, "\"\0");
            free(value_str);
            value_str = tmp;
            break;
        }
        default: {
            value_str = json_value_convert(value, JSON_STRING);
            break;
        }
    }
    
    size = size + strlen(value_str);
    
    result = malloc(size + 1);
    if(result == NULL) {
        return NULL;
    }

    if(value->name != NULL) {
        strcpy(result, "\"");
        strcat(result, value->name);
        strcat(result, "\":");
    }
    strcat(result, value_str);
    free(value_str);
    return result;
}

//TODO review this function
json_value* parse_value(const char* name,  const char* str, unsigned int begin, unsigned int* end) {
    const unsigned int str_length = strlen(str);
    printf("Parse start\n");
    for(unsigned int i = begin; i < str_length; i++) {
        if(str[i] != ' ') {
            //Check which type of value it is
            printf("Parse %c\n", str[i]);
            if(str[i] == '{') { // it's an object
                json_object* obj = json_object_create();
                if(parse_object(i, str, obj, end) != 0) {
                    return NULL;
                }
                json_value* val = json_value_create(name, obj, sizeof(json_object*), JSON_OBJECT);
                json_object_free(obj);
                return val;
            }  else if(str[i] == '[') { // it's an array
                json_array* arr = json_array_create();
                if(parse_array(i, str, arr, end) != 0) {
                    return NULL;
                }
                json_value* val = json_value_create(name, arr, sizeof(json_array*), JSON_OBJECT);
                json_array_free(arr);
                return val;
            } else if(str[i] == '\"') { // it's an string
                char* string = parse_string(i, str, end);// does not need to be is held by json_value
                json_value* val = json_value_create(name, string, strlen(string) + 1,  JSON_STRING);
                free(string);
                return val;
            } else if(str[i] == '-' || str[i] == '+' || isdigit(str[i])) { // it's an number
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
                    } else if(str[ix + i] == ',' || str[ix + i] == '}' || str[ix + i] == ']' || str[ix + i] == ' ') {
                        //end reached 
                        *end = ix + i;
                        //try to parse
                        if(doublevalue == 1) {
                            //double
                            errno = 0;
                            double num = strtod(number, NULL);
                        
                            if(errno == 0) {
                                json_value* val = json_value_create(name, &num, sizeof(double), JSON_FLOAT_NUMBER);
                               
                                return val;
                            } else {
                                return NULL;
                            }
                        } else {
                            //int todo long int
                            errno = 0;
                            int num = strtol(number, NULL, 10);
                        
                            if(errno == 0) {
                                json_value* val = json_value_create(name, &num, sizeof(int), JSON_NUMBER);
                                return val;
                            } else {
                                return NULL;
                            }
                        }
                    } else {
                        //error
                        return NULL;
                    }
                }
            }
        }
    }
}