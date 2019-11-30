#include <json.h>
#include <stdio.h> // TODO delete

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
            result->value = data;
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

char* json_value_to_str(json_value* value) {
    /*Create the string represantation of the value*/
    char* result;
    switch(value->type) {
        case JSON_INT: {
            char number[32];
            unsigned int size;
            sprintf(number, "%i", *((int*)value->value));
            size = strlen(number) + 1;
            result = malloc(sizeof(char) * size + 1);
            if(result == NULL) {
                return NULL;
            }
            memset(result, 0, size);
            strncpy(result, number, size);  
            result[size - 1] = '\0';      
        } break;
    }
    return result;
}

char* json_object_value_to_str(json_object_value* value) {
    /*Create the string "name": + json_value_to_str() */
    char* result;
    unsigned int size;
    if(value == NULL) {
        return NULL;
    }

    char* value_str = json_value_to_str(value->value);
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

//TODO work on this, implement formatting make it more functional so that json_object from str and so can also use the while loop content and implement all types
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
        char* value_str = json_object_value_to_str(element); // "name":value
    
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

int json_object_get_int(const json_object* object, const char* name, int* result) {
    json_object_value* element;
    *result = 0;

    if(object == NULL) {
        return -1;
    }
    
    element = object->first;

    while(element != NULL) {
        if(strcmp(element->name, name) == 0) {

            if(element->value == NULL) {
                return -1;
            }

            if(element->value->type != JSON_INT) {
                return -1;
            }

            //TODO irrelevant?
            if(sizeof(result) != element->value->byte_size) {
                return -1;
            }

            memcpy(result, element->value->value, element->value->byte_size);

            return 0;
        }
        element = element->next;
    }

    return -1;
}

int json_object_add_int(json_object* object, const char* name, int value) {
    json_object_value* element;
    json_object_value* last_element;
    int* data;

    if(object == NULL) {
        return -1;
    }

    data = malloc(sizeof(int));

    if(data == NULL) {
        return -1;
    }

    *data = value;

    if(object->first == NULL) {
        object->first = json_object_value_create(name, data, JSON_INT);
        if(object->first == NULL) {
            free(data);
            return -1;
        }
        return 0;
    }
    
    element = object->first;
    last_element = NULL;

    while(element != NULL) {
        if(strcmp(element->name, name) == 0) {
            return -1; //Key already in object
        }
        last_element = element;
        element = element->next;
    }

    //Append new element
    last_element->next = json_object_value_create(name, data, JSON_INT);
    if(last_element->next == NULL) {
        free(data);
        return -1;
    }

    return 0;
}