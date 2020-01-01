#include <stdio.h>
#include <stdlib.h>
#include <json.h>

int main(int argc, char** argv) {

    //json_object* test = json_object_from_str("{\"test\": {\"testval\": 20}}, \"halleluja\": \"master\"}");
    json_object* obj = json_object_create();
    json_object* obj2 = json_object_create();

    json_object_add_int(obj2, "integer", 30);

    json_object_add_object(obj, "test", obj2);
    json_object_free(obj2);
    
    json_object* res;
    if(json_object_get_object(obj, "test", &res) != 0) {
         printf("Error!\n");
    }
    char* string = json_object_to_str(res);
    printf("%s\n", string);
    free(string);
    string = json_object_to_str(obj);
    printf("%s\n", string);
    free(string);
    //res should be free with freeing of obj

    /*char* name;
    if(json_object_get_string(test, "halleluja", &name) != 0) {
        printf("Error retrieving string\n");
    }
    printf("%s\n", name);
    free(name);
  
    char* string = json_object_to_str(test); // here is the crash problem return a pointer to local address? or what is it
    printf("%s\n", string);
    printf("%p\n", string);
    free(string);

    json_object_free(test);*/
    json_object_free(obj);
    TMP_DELETE();
    return 0;
}