#include <json_array.h>
#include <string.h>

/*Local Functions*/
int json_array_add_value_index(json_array** array, void* data, unsigned int data_size, json_value_type type, unsigned int index) {
    json_value* value;
    
    if(array == NULL || *array == NULL) {
        return 1;
    }

    value = json_value_create(NULL, data, data_size, type);

    if(value == NULL) {
        return 1;
    }

    if((*array)->size + 1 == (*array)->memory_size) {
        void* tmp = realloc((*array)->values, sizeof(json_value*) * ((*array)->memory_size + 10));

        if(tmp == NULL) {
            return 1;
        }

        *array = tmp;

        (*array)->memory_size = (*array)->memory_size + 10;
    }

    (*array)->values[index] = value;

    if(index == (*array)->size) {
        (*array)->size = (*array)->size + 1;
    }
    return 0;
}

int json_array_add_value(json_array** array, void* data, unsigned int data_size, json_value_type type) {
    return json_array_add_value_index(array,data, data_size, type, (*array)->size);
}
/*Local Functions End*/


json_array* json_array_create(void) {
    json_array* result;

    result = malloc(sizeof(json_array));

    if(result == NULL) {
        return NULL;
    }

    result->size = 0;
    result->values = malloc(sizeof(json_value*) * 10);

    if(result->values == NULL) {
        free(result);
        return NULL;
    }

    result->memory_size = 10;

    return result;
}

json_array* json_array_copy(json_array* array) {
    json_array* result;

    if(array == NULL) {
        return NULL;
    }

    result = json_array_create();

    if(result == NULL) {
        return NULL;
    }

    for(unsigned int i = 0; i < array->size; i++) {
        if(json_array_add_value(&result, array->values[i]->value, array->values[i]->data_size, array->values[i]->type) != 0) {
            json_array_free(result);
            return NULL;
        }
    }
    return result;
}

void json_array_free(json_array* array) {
    if(array == NULL) {
        return;
    }

    for(unsigned int i = 0; i < array->size; i++) {
        json_value_free(array->values[i]);
    }

    free(array);
}

int json_array_get_string(const json_array* array, const unsigned int index, char** result) {
    void* tmp;

    if(array == NULL || result == NULL) {
        return 1;
    }

    if(array->size < index) {
        return 1;
    }

    tmp = json_value_convert(array->values[index], JSON_STRING);

    if(tmp != NULL) {
        *result = tmp;
        return 0;
    }
    return 1;
}

int json_array_add_string(json_array* array, char* value) {
    json_value* element;

    if(array == NULL || value == NULL) {
        return 1;
    }

    return json_array_add_value(&array, value, strlen(value) + 1, JSON_STRING);
}

//TODO here
int json_array_change_string(json_array* array, const unsigned int index, char* value) {
    if(array == NULL || value == NULL || index < array->size) {
        return 1;
    }

    json_array_remove(array, index);
    return json_array_add_value_index(&array, value, strlen(value) + 1, JSON_STRING, index);
}

void json_array_remove(json_array* array, const unsigned int index) {

}