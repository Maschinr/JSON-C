/*TODO HEADER INFO

    TODO
        These functions are for retrieving the data from json objects or arrays as const copys(Maybe not const copies for speed?)
            const <type> json_object_get_<type>(const char* name);
            json_array_get_<type>(const unsigned int position);
        Special are only the:
            const json_object* json_object_get_object(const char* name); which returns a pointer to another json object
            const json_array* json_object_get_array(const char* name); which returns a pointer to another json array
            const json_object* json_array_get_object(const unsigned int position); which returns a pointer to another json object
            const json_array* json_array_get_array(const unsigned int position); which returns a pointer to another json array
        All these accesor functions work the same way(basically):
            1. check if a value with this position/name even exist in the object/array
            y:  
                2. check if the type of the json_value_type of the json_value is compatible with the requested type
                y:
                    3. check if the requested type can fit the value (if the json contains a really large integer which could only be stored in 4 bytes that an int request could not be handled) <- most complex and critical step
                    y:
                        4. cast the value and return the copy
                    n: 
                        4. catch;
                n:
                    3. catch;
            n:
                2. catch;
            catch:
                *. return NULL or set error flag
        
        After these are complete write the fun parsing part of the lib
        After these are complete there should also be the modifier functions which allow to add, delete and change values with the same idiom
*/

#ifndef JSON_H
#define JSON_H
#include <stdlib.h>
#include <string.h>

//TODO change to all c var types and json object and json array and json null, json number should be split into float, int long double and so on
typedef enum json_value_type_e {
    JSON_STRING,
    JSON_CHAR,// JSON_UNSIGNED_CHAR,
    JSON_SHORT,// JSON_UNSIGNED_SHORT,
    JSON_INT,// JSON_UNSIGNED_INT,
    JSON_LONG,// JSON_UNSIGNED_LONG,
    JSON_LONG_LONG, // JSON_UNSIGNED_LONG_LONG,
    JSON_FLOAT,
    JSON_DOUBLE,// JSON_LONG_DOUBLE,
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_NULL
} json_value_type;

//The base json value which contains all informations needed for smart casting
typedef struct json_value_t {
    json_value_type type; //For remembering which value is stored in the void pointer
    unsigned short byte_size; //For checking which casts are valid
    void* value; //The actual value
} json_value;

//The wrapped json value for use in the json object linked list
typedef struct json_object_value_t {
    char* name;
    json_value* value;
    struct json_object_value_t* next;
} json_object_value;

/*Holds a linked list of json_value's*/ //TODO instead of linked list hash map
typedef struct json_object_t {
    json_object_value* first; // First value to linked list
} json_object;

/*Holds an array of json_value's*/
typedef struct json_array_t {
    unsigned int size;
    unsigned int memory_size;
    json_value* values; // Value array
} json_array;


//JSON OBJECT FUNCTIONS
extern json_object* json_object_create(void);//Empty json object
extern json_object* json_object_from_file(const char* path); //Try to load json object from file, fails if parse error or the main context is not an object(first char is not '{')
extern json_object* json_object_from_str(const char* str);
extern void json_object_to_file(const json_object* object, const char* path, int formatted);
extern char* json_object_to_str(const json_object* object, int formatted);
extern void json_object_free(json_object* object); // Free the json_object struct

//int
extern int json_object_get_int(const json_object* object, const char* name, int* result);
extern int json_object_add_int(json_object* object, const char* name, int value); // Adds only if non existent if existent return -1
extern void json_object_insert_int(const json_object* object, const char* name, int value); // Replaces value if it already exists, if not then create it
extern int json_object_change_int(const json_object* object, const char* name, int value); // Change a value that is already there, if not then return -1

extern void json_object_remove(const json_object* object, const char* name); // Remove a value from the object


//JSON ARRAY FUNCTIONS
/*These work for JSON string and files where the main context is an array, otherwise the return type is null*/
extern json_array* json_array_load_from_file(const char* path);
extern json_array* json_array_load_from_str(const char* str);

extern const int json_array_get_int(const json_array* array, const unsigned int position);

//GENERIC INTERFACES
#define json_object_get(object, name, result) _Generic((result),\
    int*: json_object_get_int\
) (object, name, result)

#define json_object_add(object, name, value) _Generic((value),\
    int: json_object_add_int\
) (object, name, value)

#endif