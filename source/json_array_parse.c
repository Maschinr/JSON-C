#include <json_array_parse.h>
#include <json_value.h>
#include <json_value_parse.h>
#include <json_parse.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

json_array* json_array_from_str(const char* str) {
    json_array* result = json_array_create();
    unsigned int end = 0;
    if(parse_array(0, str, result, &end) != 0) {
        json_array_free(result);
        result = NULL;
    }

    return result;
}

/*char* json_object_to_str(const json_object* object) {
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
}*/

int parse_array(unsigned int begin, const char* str, json_array* arr, unsigned int* end) {
    //stops after parsing first array after begin
    const unsigned int str_length = strlen(str);
    if(begin > str_length) return 1;
    if(arr == NULL) return 1;
    unsigned int position = begin;
   
    for(position; position < str_length; position++) {
        if(str[position] != ' ') { // Skip whitespace
            if(str[position] == '[' || position + 1 < str_length) {
                // valid array continue
                position = position + 1; // skip cur char
                break;
            } else {
                //invalid array
                return 1;
            }
        }
    }
    //now iterate over every value aslong as } is not reached
    for(position; position < str_length; position++) {
        //printf("Iter atart %c\n", str[position]);
        if(str[position] == ']') {
            //object ended
            break;
        }

        if(str[position] == ' ') {
            continue;
        }
        
        // Parse value('s)
        char name[30];
        itoa(arr->size, name, 10);
        json_value* val = parse_value(name, str, position, end);
        position = *end; 
        printf("OUT\n");
        if(val == NULL) {
            return 1;
        }

        //Insert value into object
        printf("AAAAAAAAAAADD %p %i\n", arr, *(int*)val->value);
        if(json_array_add_json_value(arr, val, -1) != 0) {
            json_value_free(val);
            return 1;
        }
        printf("arr vall added %s\n", val->name);
    }

    return 0;
}

