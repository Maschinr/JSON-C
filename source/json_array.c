#include <json_array.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*Local Functions*/
int json_array_add_value(json_array* array, void* data, unsigned int data_size, json_value_type type, int index) {
    char name[30];
    if(index == -1) {
        sprintf(name, "%i", array->size);
    } else {
        sprintf(name, "%i", index);
    }
    
    json_value* value = json_value_create(name, data, data_size, type);
    return json_array_add_json_value(array, value, index);
}
/*Local Functions End*/

int json_array_add_json_value(json_array* array, json_value* value, int index) {
    if(value == NULL || array == NULL) {
        return 1;
    }

    if(index != -1 && index > array->size) {
        return 1;
    }
    if(array->size + 1 == array->memory_size) {
        void* tmp = realloc(array->values, sizeof(json_value*) * (array->memory_size + 10));

        if(tmp == NULL) {
            return 1;
        }

        array->values = tmp;

        array->memory_size = array->memory_size + 10;
    }
    
    if(index == -1) {
        array->values[array->size] = value;
        array->size = array->size + 1;
    } else {
        array->values[index] = value;
    }
    return 0;
}


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

void json_array_free(json_array* array) {
    if(array == NULL) {
        return;
    }

    for(unsigned int i = 0; i < array->size; i++) {
        json_value_free(array->values[i]);
    }

    free(array);
}

void json_array_iterate(json_array* array, void (*func)(json_value* value)) {
    if(array == NULL || func == NULL) {
        return;
    }
    for(int i = 0; i < array->size; i++) {
        func(array->values[i]);
    }
}

json_array* json_array_copy(json_array* array) {
    if(array == NULL) {
        return NULL;
    }

    json_array* result = json_array_create();

    if(result == NULL) {
        return NULL;
    }

    for(unsigned int i = 0; i < array->size; i++) {
        if(json_array_add_value(result, array->values[i]->value, array->values[i]->data_size, array->values[i]->type, -1) != 0) {
            json_array_free(result);
            return NULL;
        }
    }
    return result;
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
    if(array == NULL || value == NULL) {
        return 1;
    }

    return json_array_add_value(array, value, strlen(value) + 1, JSON_STRING, -1);
}

int json_array_change_string(json_array* array, const unsigned int index, char* value) {
    if(array == NULL || value == NULL || index < array->size) {
        return 1;
    }

    json_array_remove(array, index);
    return json_array_add_value(array, value, strlen(value) + 1, JSON_STRING, index);
}

int json_array_get_number(const json_array* array, const unsigned int index, int* result) {
    void* tmp;

    if(array == NULL || result == NULL) {
        return 1;
    }

    if(array->size < index) {
        return 1;
    }

    tmp = json_value_convert(array->values[index], JSON_NUMBER);

    if(tmp != NULL) {
        *result = *(int*)tmp;
        free(tmp);
        return 0;
    }
    return 1;
}

int json_array_add_number(json_array* array, int value) {
    if(array == NULL) {
        return 1;
    }

    return json_array_add_value(array, &value, sizeof(int), JSON_NUMBER, -1);
}

int json_array_change_number(json_array* array, const unsigned int index, int value) {
    if(array == NULL || index < array->size) {
        return 1;
    }

    json_array_remove(array, index);
    return json_array_add_value(array, &value, sizeof(int), JSON_NUMBER, index);
}

int json_array_get_float_number(const json_array* array, const unsigned int index, double* result) {
    void* tmp;

    if(array == NULL || result == NULL) {
        return 1;
    }

    if(array->size < index) {
        return 1;
    }

    tmp = json_value_convert(array->values[index], JSON_FLOAT_NUMBER);

    if(tmp != NULL) {
        *result = *(double*)tmp;
        free(tmp);
        return 0;
    }
    return 1;
}

int json_array_add_float_number(json_array* array, double value) {
    if(array == NULL) {
        return 1;
    }

    return json_array_add_value(array, &value, sizeof(double), JSON_FLOAT_NUMBER, -1);
}

int json_array_change_float_number(json_array* array, const unsigned int index, double value) {
    if(array == NULL || index < array->size) {
        return 1;
    }

    json_array_remove(array, index);
    return json_array_add_value(array, &value, sizeof(double), JSON_FLOAT_NUMBER, index);
}

int json_array_get_object(const json_array* array, const unsigned int index, json_object** result) {
    void* tmp;

    if(array == NULL || result == NULL || result == NULL) {
        return 1;
    }

    if(array->size < index) {
        return 1;
    }

    tmp = json_value_convert(array->values[index], JSON_OBJECT);

    if(tmp != NULL) {
        *result = tmp;
        return 0;
    }
    return 1;
}

int json_array_add_object(json_array* array, json_object* value) {
    if(array == NULL) {
        return 1;
    }

    return json_array_add_value(array, value, sizeof(json_object*), JSON_OBJECT, -1);
}

int json_array_get_array(const json_array* array, const unsigned int index, json_array** result) {
    void* tmp;

    if(array == NULL || result == NULL || result == NULL) {
        return 1;
    }

    if(array->size < index) {
        return 1;
    }

    tmp = json_value_convert(array->values[index], JSON_ARRAY);
    if(tmp != NULL) {
        *result =  (json_array*)tmp;
        return 0;
    }
    return 1;
}

int json_array_add_array(json_array* array, json_array* value) {
    if(array == NULL) {
        return 1;
    }

    return json_array_add_value(array, value, sizeof(json_array*), JSON_ARRAY, -1);
}

void json_array_remove(json_array* array, const unsigned int index) {
    if(array == NULL || index > array->size) {
        return;
    }

    json_value_free(array->values[index]);
    array->values[index] = NULL;
}