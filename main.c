#include <stdio.h>
#include <stdlib.h>
#include <json.h>

int main(int argc, char** argv) {

    json_object* obj;
    obj = json_object_create();

    if(json_object_add(obj, "test", 10) == 0) {
        printf("add success\n");
    }

    int a;
    json_object_get(obj, "test", &a);
    printf("Value: %i\n", a);

    json_object_free(obj);

    return 0;
}