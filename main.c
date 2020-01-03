#include <stdio.h>
#include <stdlib.h>
#include <json.h>

// I think hashmap are not copied correctly when you add an object to another object because the hashmap content is not copied, only the indexes, so
// i need to build a way to copy the hashmap, maybe just iterate through the old one and fill a new one with it?

int main(int argc, char** argv) {
    json_object* json = json_object_create();
    json_object_free(json);
    return 0;
}