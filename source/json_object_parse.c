#include <json_object_parse.h>
#include <json_value.h>
#include <json_value_parse.h>
#include <json_parse.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*Local Declarations*/
typedef struct iterator_container_t {
    char** text;
    int prev_elem;
} iterator_container;

int json_object_value_to_str_iterator(void* cont, void* val) {
    unsigned int size = 0;
    char* value_str = json_value_to_str((json_value*)val); // "name":value
    
    if(value_str == NULL) {
        return MAP_MISSING; // Error
    }
    iterator_container* ct = (iterator_container*)cont;
    size = strlen(*ct->text) + strlen(value_str);

    if(ct->prev_elem != 0) {
        size = size + 1; // For the comma
    }
    void* tmp = realloc(*ct->text, size + 1); // +1 for the \0
    if(tmp == NULL) {
        return MAP_MISSING; // Error
    }
    *ct->text = tmp;
   
    if(ct->prev_elem != 0) {
        strcat(*ct->text, ",");
    }
    strcat(*ct->text, value_str);
    free(value_str);
    ct->prev_elem = 1;
    return MAP_OK;
}
/*Local Declarations End*/

json_object* json_object_from_str(const char* str) {
    json_object* result = json_object_create();
    unsigned int end = 0;
    if(parse_object(0, str, result, &end) != 0) {
        json_object_free(result);
        result = NULL;
    }

    return result;
}

char* json_object_to_str(const json_object* object) {
    char* result;
    if(object == NULL) {
        return NULL;
    }

    if(hashmap_length(object->map) > 0) {

        result = malloc(sizeof(char) * 2); // {} the third for \0 will be set by hashmap_iterate
        if(result == NULL) {
            return NULL;
        }

        result[0] = '{';
        result[1] = '\0';

        iterator_container ct;
        ct.text = &result;
        ct.prev_elem = 0;

        if(hashmap_iterate(object->map, json_object_value_to_str_iterator, &ct) != MAP_OK) {
            free(result);
            return NULL;
        }

        void* tmp = realloc(result, strlen(result) + 2); // +2 for the \0 and the }
        if(tmp == NULL) {
            //TODO free result or is freed automatically if realloc fails?
            return NULL;
        }
        result = tmp;
        strcat(result, "}");
    } else {
        result = malloc(sizeof(char) * 3); // {} the third for \0 will be set by hashmap_iterate
        if(result == NULL) {
            return NULL;
        }

        result[0] = '{';
        result[1] = '}';
        result[2] = '\0';
       
    }
    return result;
}

int parse_object(unsigned int begin, const char* str, json_object* obj, unsigned int* end) {
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

        if(str[position] == ' ') {
            continue;
        }
        
        // Parse value('s)
        char* name = parse_string(position, str, end); // Get object name
        
        if(name == NULL) {
            return 1;
        }
        position = *end;
        json_value* val = parse_value(name, str, position, end);
        free(name);
        position = *end; 

        if(val == NULL) {
            return 1;
        }
        //Insert value into object
        json_value* ret;
        if(hashmap_get(obj->map, val->name, (void**)(&ret)) == MAP_OK) {
            return 1;
        }
        if(hashmap_put(obj->map, val->name, val) != MAP_OK) {
            //Insert errors
            json_value_free(val);
            return 1;
        }
    }

    return 0;
}

