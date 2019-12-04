#include <stdio.h>
#include <json.h>

int main(int argc, char** argv) {
    double test;
    int test2;
    json_object* json;
    //json = json_object_load_from_str("{\"test\":10}");
    json = json_object_create();

    if(json_object_add(json, "test2", 20) != 0) {
        printf("Error adding int1\n");
    }

    if(json_object_add(json, "test", 10) != 0) {
        printf("Error adding int2\n");
    } 

    if(json_object_get(json, "test", &test) != 0) {
        printf("Error getting int1\n");
    }

    if(json_object_get(json, "test2", &test2) != 0) {
        printf("Error getting int2\n");
    }

    printf("%g %i\n", test, test2);

    char* json_text = json_object_to_str(json, 0);
    printf("%s %lu\n", json_text, strlen(json_text));
    free(json_text);

    if(json_object_change(json, "test", -102.90341f) != 0) {
        printf("Error changing test\n");
    }

    if(json_object_get(json, "test", &test) != 0) {
        printf("Error getting int1\n");
    }

    if(json_object_get(json, "test", &test2) != 0) {
        printf("Error getting int2\n");
    }

    printf("%g %i\n", test, test2);

    json_text = json_object_to_str(json, 0);
    printf("%s %lu\n", json_text, strlen(json_text));
    free(json_text);

    if(json_object_remove(json, "test") != 0) {
        printf("Error removing test\n");
    }

    /*json_text = json_object_to_str(json, 0);
    printf("%s %lu\n", json_text, strlen(json_text));
    free(json_text);*/

    json_object_free(json);

    //TODO fix + 3 free because of string, leaking some
    TMP_DELETE();

    return 0;
}