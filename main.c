#include <stdio.h>
#include <stdlib.h>
#include <json.h>

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
    return 0;
}