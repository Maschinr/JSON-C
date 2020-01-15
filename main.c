#include <stdio.h>
#include <stdlib.h>
#include <json.h>

void test3(json_value* val) {
    char* val_str = json_value_convert(val, JSON_STRING);
    printf("Inside: %s %s\n", val->name, val_str);
    free(val_str);
}

int main(int argc, char** argv) {
    /*json_object* json = json_object_from_str("{\
                                                \"String\": \"String\",\
                                                \"Number:\": 35,\
                                                \"Floating\": 36.9\
                                              }");
    if(json == NULL) {
        printf("Parse Error!\n");
        return 1;
    }
    char* result = json_object_to_str(json);
    printf("Result: %s\n", result);
    free(result);
    json_object_free(json);

    json = json_object_from_str("{\"String\": \"String\",\"Number\": 35,\"Floating\": 36.9}");

    if(json == NULL) {
        printf("Parse Error!\n");
        return 1;
    }
    result = json_object_to_str(json);
    printf("Result: %s\n", result);
    free(result);
    json_object_free(json);


    json = json_object_create();
    json_object* json2 = json_object_create();

    json_object_add_number(json2, "Integer", 2);
    json_object_add(json2, "Dynamic Number", 2);
    json_object_add(json2, "Dynamic Number Float", 2.03);
    json_object_add_string(json2, "String", "Hello World1");
    
    json_object_add_object(json, "Object", json2);
   
    json_object_free(json2);
    json_object_get_object(json, "Object", &json2);

    json_object_add_number(json, "Integer", 1);
    json_object_add(json, "Dynamic Number", 1);
    json_object_add(json, "Dynamic Number Float", 1.04);
    json_object_add_string(json, "String", "Hello World2");
    json_object_add(json, "Dynamic String", "Dynamic Hello World2");

    result = json_object_to_str(json);
    printf("Result: %s\n", result);
    free(result);
    
    json_object_free(json);*/

    json_array* arr = json_array_create();
    printf("HEre\n");
    json_array_add(arr, 10);
    json_array_add(arr, "Hello World");
    json_array_add(arr, 10.56);
    json_array_add(arr, arr);
    printf("HEre2\n");
    json_array* arr2;

    json_array_get(arr, 3, &arr2);
    printf("HEre3\n");
    json_array_iterate(arr2, test3);
    printf("HEre4\n");
    json_array_iterate(arr, test3);

    json_array_free(arr);

    return 0;
}