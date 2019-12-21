#include <stdio.h>
#include <stdlib.h>
#include <json.h>

int main(int argc, char** argv) {

    json_object* obj;
    obj = json_object_create();

    json_object_add(obj, "test", 10);
    json_object_add(obj, "test2", 30);
    char* string = json_object_to_str(obj);
    printf("%s\n", string );
    free(string);

    json_object* test = json_object_from_str("{\"test\": 10, \"halleluja\": \"master\"}");

    string = json_object_to_str(test);
    printf("%s\n", string);
    free(string);
    json_object_free(obj);
    json_object_free(test);
    //TODO crash on adding string value sometimes
    printf("Here\n");
    return 0;
}