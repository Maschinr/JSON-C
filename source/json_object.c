#include <json_object.h>
#include <json_array.h>
#include <json_value.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*Local Functions*/
int free_hashmap_iterator(void* null, void* value) {
    json_value_free((json_value*)value);
    return MAP_OK;
}

int copy_object_iterator(void* new_object, void* value) {
    json_object* object = (json_object*)new_object;
    json_value* jvalue = (json_value*)value;

    json_value* new_value = json_value_create(jvalue->name, jvalue->value, jvalue->data_size, jvalue->type);

    if(object == NULL ||jvalue == NULL) {
        return MAP_MISSING;
    }
    return hashmap_put(object->map, new_value->name, new_value);
}

int object_iterator_iterator(void* func, void* value) {
    void(*funct)(json_value*) = func;
    json_value* jvalue = (json_value*)value;

    if(jvalue == NULL || funct == NULL) {
        return MAP_MISSING;
    }

    funct(jvalue);

    return MAP_OK;
}
/*Local Functions End*/

json_object* json_object_create(void) {
    json_object* result;

    result = malloc(sizeof(json_object));

    if(result == NULL) {
        return NULL;
    }

    result->map = hashmap_new();

    if(result->map == NULL) {
        free(result);
        return NULL;
    }

    return result;
}

json_object* json_object_copy(json_object* object) {
    json_object* result;

    result = malloc(sizeof(json_object));

    if(result == NULL) {
        return NULL;
    }

    result->map = hashmap_new();

    if(result->map == NULL) {
        free(result);
        return NULL;
    }

    if(hashmap_iterate(object->map, copy_object_iterator, result) != 0) {

        free(result);
        hashmap_free(result->map);
        return NULL;
    }
    
    return result;
}

void json_object_iterate(json_object* object, void (*func)(json_value* value)) {
    if (object == NULL || func == NULL) {
        return;
    }

    hashmap_iterate(object->map, object_iterator_iterator, func);
}

void json_object_free(json_object* object) {
    if(object == NULL) {
        return;
    }

    hashmap_iterate(object->map, free_hashmap_iterator, NULL);

    hashmap_free(object->map);
    free(object);
}

int json_object_get_string(const json_object* object, const char* name, char** result) {
    json_value* element;
    void* tmp;

    if(object == NULL || result == NULL || name == NULL) {
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

int json_object_add_string(json_object* object, const char* name, char* value) {
    json_value* element;

    if(object == NULL || value == NULL || name == NULL) {
        return 1;
    }

    if(hashmap_get(object->map, name, (void**)(&element)) == MAP_OK) { // Check if element already exists
        return 1;
    }

    element = json_value_create(name, value, strlen(value) + 1, JSON_STRING);

    if(element == NULL) {
        return 1;
    }

    if(hashmap_put(object->map, name, element) != MAP_OK) {
        json_value_free(element);
        return 1;
    }

    return 0;
}

int json_object_insert_string(json_object* object, const char* name, char* value) {
    if(json_object_add_string(object, name, value) != 0) {
        return json_object_change_string(object, name, value);
    }
    return 0;
}

int json_object_change_string(json_object* object, const char* name, char* value) {
    json_object_remove(object, name);
    return json_object_add_string(object, name, value);
}

int json_object_get_number(const json_object* object, const char* name, int* result) {
    json_value* element;
    void* tmp;

    if(object == NULL || name == NULL || result == NULL) {
        return 1;
    }

    element = NULL;

    if(hashmap_get(object->map, name, (void**)(&element)) != MAP_OK) {
        return 1;
    }

    tmp = json_value_convert(element, JSON_NUMBER);
    if(tmp != NULL) {
        memcpy(result, tmp, sizeof(int));
        free(tmp);
        return 0;
    }
    return 1;
}

int json_object_add_number(json_object* object, const char* name, int value) {
    json_value* element;

    if(name == NULL || object == NULL) {
        return 1;
    }

    if(hashmap_get(object->map, name, (void**)(&element)) == MAP_OK) {/*Value already exists*/
        return 1;
    }

    element = json_value_create(name, &value, sizeof(int), JSON_NUMBER);

    if(element == NULL) {
        return 1;
    }

    if(hashmap_put(object->map, name, element) != MAP_OK) {
        json_value_free(element);
        return 1;
    }

    return 0;
}

int json_object_insert_number(json_object* object, const char* name, int value) {
    if(json_object_add_number(object, name, value) != 0) {
        return json_object_change_number(object, name, value);
    }
    return 0;
}

int json_object_change_number(json_object* object, const char* name, int value) {
    json_object_remove(object, name);
    return json_object_add_number(object, name, value);
}

int json_object_get_float_number(const json_object* object, const char* name, double* result) {
    json_value* element;
    void* tmp;

    if(object == NULL || name == NULL || result == NULL) {
        return 1;
    }

    element = NULL;

    if(hashmap_get(object->map, name, (void**)(&element)) != MAP_OK) {
        return 1;
    }

    tmp = json_value_convert(element, JSON_FLOAT_NUMBER);
    if(tmp != NULL) {
        memcpy(result, tmp, sizeof(double));
        free(tmp);
        return 0;
    }
    return 1;
}

int json_object_add_float_number(json_object* object, const char* name, double value) {
    json_value* element;

    if(name == NULL || object == NULL) {
        return 1;
    }

    if(hashmap_get(object->map, name, (void**)(&element)) == MAP_OK) {/*Value already exists*/
        return 1;
    }

    element = json_value_create(name, &value, sizeof(double), JSON_FLOAT_NUMBER);

    if(element == NULL) {
        return 1;
    }

    if(hashmap_put(object->map, name, element) != MAP_OK) {
        json_value_free(element);
        return 1;
    }

    return 0;
}

int json_object_insert_float_number(json_object* object, const char* name, double value) {
    if(json_object_add_float_number(object, name, value) != 0) {
        return json_object_change_float_number(object, name, value);
    }
    return 0;
}

int json_object_change_float_number(json_object* object, const char* name, double value) {
    json_object_remove(object, name);
    return json_object_add_float_number(object, name, value);
}

int json_object_get_object(const json_object* object, const char* name, json_object** result) {
    json_value* element;
    void* tmp;

    if(object == NULL || name == NULL || result == NULL) {
        return 1;
    }

    element = NULL;

    if(hashmap_get(object->map, name, (void**)(&element)) != MAP_OK) {
        return 1;
    }

    tmp = json_value_convert(element, JSON_OBJECT);
    if(tmp != NULL) {
        *result = (json_object*)tmp;
        return 0;
    }
    return 1;
}

int json_object_add_object(json_object* object, const char* name, json_object* value) {
    json_value* element;

    if(name == NULL || object == NULL) {
        return 1;
    }

    if(hashmap_get(object->map, name, (void**)(&element)) == MAP_OK) {/*Value already exists*/
        return 1;
    }

    element = json_value_create(name, value, sizeof(json_object*), JSON_OBJECT);
    if(element == NULL) {
        return 1;
    }

    if(hashmap_put(object->map, name, element) != MAP_OK) {
        json_value_free(element);
        return 1;
    }

    return 0;
}

int json_object_insert_object(json_object* object, const char* name, json_object* value) {
    if(json_object_add_object(object, name, value) != 0) {
        json_object_remove(object, name);
        return json_object_add_object(object, name, value);
    }
    return 0;
}

int json_object_get_array(const json_object* object, const char* name, json_array** result) {
    json_value* element;
    void* tmp;

    if(object == NULL || name == NULL || result == NULL) {
        return 1;
    }

    element = NULL;

    if(hashmap_get(object->map, name, (void**)(&element)) != MAP_OK) {
        return 1;
    }

    tmp = json_value_convert(element, JSON_ARRAY);
    if(tmp != NULL) {
        *result = *(json_array**)tmp;
        return 0;
    }
    return 1;
}

int json_object_add_array(json_object* object, const char* name, json_array* value) {
 json_value* element;

    if(name == NULL || object == NULL) {
        return 1;
    }

    if(hashmap_get(object->map, name, (void**)(&element)) == MAP_OK) {/*Value already exists*/
        return 1;
    }

    element = json_value_create(name, value, sizeof(json_array*), JSON_ARRAY);

    if(element == NULL) {
        return 1;
    }

    if(hashmap_put(object->map, name, element) != MAP_OK) {
        json_value_free(element);
        return 1;
    }

    return 0;
}

int json_object_insert_array(json_object* object, const char* name, json_array* value) {
    if(json_object_add_array(object, name, value) != 0) {
        json_object_remove(object, name);
        return json_object_add_array(object, name, value);
    }
    return 0;
}

int json_object_remove(json_object* object, const char* name) {
    json_value* result;

    if(object == NULL) {
        return 1;
    }
    
    if(hashmap_get(object->map, name, (void**)(&result)) == MAP_OK) {
        hashmap_remove(object->map, name);
        json_value_free(result);
        return 0;
    }

    return 1;
}