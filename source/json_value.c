#include <json_value.h>
#include <json_object.h>
#include <json_array.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

json_value* json_value_create(const char* name, void* data, unsigned int data_size, json_value_type type) {
    json_value* result;

    if(data == NULL ||name == NULL) {
        return NULL;
    }

    result = malloc(sizeof(json_value));
    
    if(result == NULL) {
        return NULL;
    }

    result->type = type;
    result->data_size = data_size;
    result->name = malloc(strlen(name) + 1);

    if(result->name == NULL) {
        free(result);
        return NULL;
    }

    strcpy(result->name, name);

    result->value = malloc(data_size);

    if(result->value == NULL) {
        free(result->name);
        free(result);
        return NULL;
    }

    if(type == JSON_OBJECT) {
        result->value = json_object_copy(data);
    } else if(type == JSON_ARRAY) {
        result->value = json_array_copy(data);
    } else {
        memcpy(result->value, data, data_size);
        
    }

    if(result->value == NULL) {
        free(result);
        return NULL;
    }

    return result;
}

void json_value_free(json_value* value) {
    if(value == NULL) {
        return;
    }

    free(value->name);

    if(value->type == JSON_OBJECT) {
        json_object_free(value->value);
    } else if(value->type == JSON_ARRAY) {
        json_array_free(value->value);  
    } else {
        free(value->value);
    }
    free(value);
}

void* json_value_convert(json_value* value, json_value_type type) {
    void* result;

    if (value == NULL) {
        return NULL;
    }
   
    result = NULL;

    switch(type) { // To convert to
        case JSON_STRING: { // Convert to string
            switch(value->type) { // from
                case JSON_STRING: { // String
                    result = malloc(strlen(value->value) + 1);

                    if(result == NULL) {
                        return NULL;
                    }

                    strcpy(result, value->value);
                 
                    return result;
                }

                case JSON_NUMBER:
                case JSON_FLOAT_NUMBER: { // Number
                    char number[32];
                    unsigned int size;
                    memset(number, '\0', 32);
                    
                    if(value->type == JSON_NUMBER) {
                        sprintf(number, "%i", *((int*)value->value));
                    } else {
                        sprintf(number, "%g", *((double*)value->value));
                    }

                    size = strlen(number);
                    result = malloc(sizeof(char) * (size + 1));
                    
                    if(result == NULL) {
                        return NULL;
                    }

                    strcpy(result, number);

                    return result;
                }

                default: {
                    return NULL;
                }
            }
            break;
        }

        case JSON_NUMBER: { // Convert to number
            result = malloc(sizeof(int));

            if(result == NULL) {
                return NULL;
            }

            switch(value->type) { //from
                case JSON_NUMBER: { // Number
                    memcpy(result, value->value, sizeof(int));
                    return result;
                }

                case JSON_FLOAT_NUMBER: { // Double
                    int tmp = *(double*)value->value;
                    memcpy(result, &tmp, sizeof(int));
                    return result;
                }

                case JSON_STRING: { // String
                    char* tmp = (char*)value->value;
                    int tmp2 = atoi(tmp);
                    memcpy(result, &tmp2, sizeof(int));
                    return result;
                }

                default: {
                    free(result);
                    return NULL;
                }
            }
            break;
        }

        case JSON_FLOAT_NUMBER: { // Convert to float number
            result = malloc(sizeof(double));

            if(result == NULL) {
                return NULL;
            }

            switch(value->type) { // from
                
                case JSON_NUMBER: {
                    double tmp = *(int*)value->value;
                    memcpy(result, &tmp, sizeof(double));
                    return result;
                }

                case JSON_FLOAT_NUMBER: {
                    memcpy(result, value->value, sizeof(double));
                    return result;
                }

                case JSON_STRING: {
                    char* tmp = (char*)value->value;
                    int tmp2 = atof(tmp);
                    memcpy(result, &tmp2, sizeof(double));
                    return result;
                }

                default: {
                    free(result);
                    return NULL;
                }
            }
       
            break;
        }

        case JSON_OBJECT: { // Convert to object
            if(value->type == JSON_OBJECT) {
                return value->value;
            } else {
                return NULL;
            }
            break;
        }

        case JSON_ARRAY: { // Convert to Array
            if(value->type == JSON_ARRAY) {
                return value->value;
            } else {
                return NULL;
            }
            break;
        }

        default: {
            return NULL;
        }
    }

    return NULL;
}