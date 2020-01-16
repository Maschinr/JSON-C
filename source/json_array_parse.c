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

json_array* json_array_from_file(const char* path) {
    FILE *file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *string = malloc(fsize + 1);
    if(string == NULL) {
        return NULL;
    }
    fread(string, 1, fsize, file);
    fclose(file);
    string[fsize] = 0;

    return json_array_from_str(string);
}

char* json_array_to_str(const json_array* arr) {
    char* result;
    if(arr == NULL) {
        return NULL;
    }

    if(arr->size > 0) {

        result = malloc(sizeof(char) * 2); // []
        if(result == NULL) {
            return NULL;
        }

        result[0] = '[';
        result[1] = '\0';

        int prev_elem = 0;
        for(int i = 0; i < arr->size; i++) {
            unsigned int size = 0;
            char* value_str = json_value_to_str(arr->values[i], 0); // value
            printf("Val %s\n", value_str);
            if(value_str == NULL) {
                free(result);
                return NULL; // Error
            }
            

            size = strlen(result) + strlen(value_str);

            if(prev_elem != 0) {
                size = size + 1; // For the comma
            }

            void* tmp = realloc(result, size + 1); // +1 for the \0
            if(tmp == NULL) {
                free(value_str);
                return NULL; // Error
            }
            result = tmp;
        
            if(prev_elem != 0) {
                strcat(result, ",");
            }
            strcat(result, value_str);
            free(value_str);
            prev_elem = 1;
        }

        void* tmp = realloc(result, strlen(result) + 2); // +2 for the \0 and the ]
        if(tmp == NULL) {
            //TODO free result or is freed automatically if realloc fails?
            return NULL;
        }
        result = tmp;
        strcat(result, "]");
    } else {
        result = malloc(sizeof(char) * 3); // [] the third for \0
        if(result == NULL) {
            return NULL;
        }

        result[0] = '[';
        result[1] = ']';
        result[2] = '\0';
       
    }
    return result;
}

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
        if(str[position] == ']') {
            break;
        }

        if(str[position] == ' ') {
            continue;
        }
        
        // Parse value('s)
        char name[30];
        sprintf(name, "%i", arr->size);

        json_value* val = parse_value(name, str, position, end);
        position = *end; 
        
        if(val == NULL) {
            return 1;
        }

        //Insert value into object
        if(json_array_add_json_value(arr, val, -1) != 0) {
            json_value_free(val);
            return 1;
        }

        position -= 1;
    }

    *end = position + 1;

    return 0;
}

