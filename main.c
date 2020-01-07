#include <stdio.h>
#include <stdlib.h>
#include <json.h>

void test3(json_value* val) {
    printf("Inside: %s\n", val->name);
}

int test2(void* non, void* val) {
    printf("Inside: %s\n", ((json_value*)val)->name);
    return MAP_OK;
}

int main(int argc, char** argv) {
    json_object* json = json_object_from_str("{\
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

    json_object_add_number(json2, "Integer", 134902);
    json_object_add(json2, "Dynamic Number", 23910);
    json_object_add(json2, "Dynamic Number Float", 2130.03);
    json_object_add_string(json2, "String", "Hello World");
    
    json_object_add_object(json, "Object", json2);  // TODO does weird stuff or
   
    json_object_iterate(json2, test3);
    json_object_free(json2);
    json_object_get_object(json, "Object", &json2); // TODO this does weird stuff
    json_object_iterate(json2, test3);

    json_object_add_number(json, "Integer", 134902);
    json_object_add(json, "Dynamic Number", 23910);
    json_object_add(json, "Dynamic Number Float", 2130.03);
    json_object_add_string(json, "String", "Hello World");
    json_object_add(json, "Dynamic String", "Dynamic Hello World");
    printf("After\n");
    result = json_object_to_str(json); // TODO crash
    printf("Result: %s\n", result);
    free(result);
    
    json_object_free(json);

    return 0;
}