#include <unity.h>
#include <json.h>

json_object* object;

void test_CreateJsonObject(void) {
    TEST_ASSERT_NOT_EQUAL(NULL, object);
    TEST_ASSERT_EQUAL(NULL, object->first);
}

void test_JsonObjectToStrEmpty(void) {
    char* string;
    string = json_object_to_str(object, 0);
    TEST_ASSERT_EQUAL_STRING("{}", string);
    free(string);
}

void test_JsonObjectToStrValues(void) {
    char* string;
    json_object_add(object, "test", 10);
    json_object_add(object, "test2", 30);
    string = json_object_to_str(object, 0);
    TEST_ASSERT_EQUAL_STRING("{\"test\":10,\"test2\":30}", string);
    free(string);
}

void test_JsonObjectToStrNullObject(void) {
    char* string;
    string = json_object_to_str(NULL, 0);
    TEST_ASSERT_EQUAL(NULL, string);
    free(string);
}

//TODO further testing
void test_JsonObjectAddInt(void) {

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