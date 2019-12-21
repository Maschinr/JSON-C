#include <stdio.h>
#include <stdlib.h>
#include <json.h>

int main(int argc, char** argv) {

    json_object* test = json_object_from_str("{\"test\": 10, \"halleluja\": \"master\"}");
    
    char* name;
    if(json_object_get_string(test, "halleluja", &name) != 0) {
        printf("Error retrieving string\n");
    }
    printf("%s\n", name);
    free(name);
  
    char* string = json_object_to_str(test); // here is the crash problem return a pointer to local address? or what is it
    printf("%s\n", string);
    //free(string);

    json_object_free(test);
    TMP_DELETE();
    return 0;
}