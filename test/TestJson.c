#include <unity.h>
#include <json.h>

json_object* object;

/*  Json Object Tests
*   Create Json Object with json_object_create
*   Free Json Object with json_object_free
*   Read in an Json Object from a File
*   Read in an Json Object from a String
*   Convert a Json Object to a String
*   Output an Json Object to a File
*   Create a Json Object and assign an int value to it
*   Create a Json Object with an int value and get the value as int
*   Create a Json Object with an int value and insert a new value with the same name
*   
*/

void test_CreateJsonObject(void) {
    TEST_ASSERT_NOT_EQUAL(NULL, object);
}

void test_JsonObjectToStrEmpty(void) {
    char* string;
    string = json_object_to_str(object);
    TEST_ASSERT_EQUAL_STRING("{}", string);
    free(string);
}

void test_JsonObjectToStrValues(void) {
    char* string;
    json_object_add_string(object, "string", "string");
    json_object_add_int(object, "int", 30);
    json_object_add_float(object, "float", 30.3);
    json_object* obj = json_object_create();
    json_object_add_object(object, "object", *obj); // obj is copied so you can free it now
    json_object_free(obj);
    string = json_object_to_str(object);
    TEST_ASSERT_EQUAL_STRING("{\"string\":\"string\",\"int\":30,\"float\":30.3,\"object\": {}}", string);
    free(string);
}

void test_JsonObjectToStrNullObject(void) {
    char* string;
    string = json_object_to_str(NULL);
    TEST_ASSERT_EQUAL(NULL, string);
    free(string);
}

void setUp (void) {
    object = json_object_create();
}

void tearDown (void) {
    json_object_free(object);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_CreateJsonObject);
    RUN_TEST(test_JsonObjectToStrEmpty);
    RUN_TEST(test_JsonObjectToStrValues);
    RUN_TEST(test_JsonObjectToStrNullObject);
    return UNITY_END();
}