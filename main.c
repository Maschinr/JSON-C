#include <stdio.h>
#include <json.h>

int main(int argc, char** argv) {
    int test, test2;
    json_object* json;
    //json = json_object_load_from_str("{\"test\":10}");
    json = json_object_create();

    if(json_object_add(json, "test2", 20) != 0) {
        printf("Error adding int\n");
    }

    if(json_object_add(json, "test", 10) != 0) {
        printf("Error adding int\n");
    }

    json_object_get(json, "test", &test);
    json_object_get(json, "test2", &test2);

    printf("Test: %i Test2: %i\n", test, test2);
  
    char* json_text = json_object_to_str(json, 0);
    printf("%s %i\n", json_text, strlen(json_text));
    free(json_text);

    json_object_free(json);

    return 0;
}