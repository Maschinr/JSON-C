# JSON-C(to be changed)
## Content
- [Introduction](https://github.com/Maschinr/JSON-C#introduction)
- [How to use](https://github.com/Maschinr/JSON-C#how-to-use)
    - [Reading a json_object from a string and printing it to the command line](https://github.com/Maschinr/JSON-C#reading-a-json_object-from-a-string-and-printing-it-to-the-command-line)
    - [Creating a new json_object, adding values and printing it to the command line](https://github.com/Maschinr/JSON-C#creating-a-new-json_object-adding-values-and-printing-it-to-the-command-line)
    - [Retrieving values of a JSON string](https://github.com/Maschinr/JSON-C#retrieving-values-of-a-json-string)
    - [Nesting json_objects in json_objects](https://github.com/Maschinr/JSON-C#nesting-json_objects-in-json_objects)
- [Conversion Table (TODO)](https://github.com/Maschinr/JSON-C#conversion-table-todo)
- [JSON Object Functions](https://github.com/Maschinr/JSON-C#json-object-functions)
    - [json_object_create](https://github.com/Maschinr/JSON-C#json_object-json_object_createvoid)
    - [json_object_free](https://github.com/Maschinr/JSON-C#void-json_object_freejson_object-object)
    - [json_object_from_file](https://github.com/Maschinr/JSON-C#json_object-json_object_from_fileconst-char-path)
    - [json_object_from_str](https://github.com/Maschinr/JSON-C#json_object-json_object_from_strconst-char-str)
    - [json_object_to_file](https://github.com/Maschinr/JSON-C#int-json_object_to_fileconst-json_object-object-const-char-path-int-formatted)
    - [json_object_to_str](https://github.com/Maschinr/JSON-C#char-json_object_to_strconst-json_object-object-int-formatted)
    - [json_object_get_*type*](https://github.com/Maschinr/JSON-C#int-json_object_get_typeconst-json_object-object-const-char-name-type-result)
    - [json_object_add_*type*](https://github.com/Maschinr/JSON-C#int-json_object_add_typeconst-json_object-object-const-char-name-const-type-value)
    - [json_object_insert_*type*](https://github.com/Maschinr/JSON-C#int-json_object_insert_typeconst-json_object-object-const-char-name-const-type-value)
    - [json_object_change_*type*](https://github.com/Maschinr/JSON-C#int-json_object_change_typeconst-json_object-object-const-char-name-const-type-value)
    - [json_object_remove](https://github.com/Maschinr/JSON-C#int-json_object_removejson_object-object-const-char-name)
- [JSON Array Functions (TODO)](https://github.com/Maschinr/JSON-C#json-array-functions-todo)
- [Generic Functions](https://github.com/Maschinr/JSON-C#generic-functions-if-c11-compiled)
    - [Using Generic Functions](https://github.com/Maschinr/JSON-C#using-generic-functions)
- [Test's](https://github.com/Maschinr/JSON-C#tests)

## Introduction

JSON-C(to be changed) is a C99 compatible (with optional C11 features)
library for JSON handling in an comfortable way. My Main Focus was not to 
have the fastest Library there is, but rather a pretty memory secure one with
an simple to use API. Not focusing on speed doesn't mean that this library
is particulary slow, just not the fastest. (Benchmarks may come in the future)

## How to use

Just add the json.h (include/json.h) to your include Path and add the json.c to your compilation.

The library consits of two main Structs the json_object and the json_array, 
the first describing an JSON Object and the latter, an JSON Array.
Both of these structs can be dynamically created by there associated create 
functions and freed by there free functions. The JSON object functions like a 
Map in other languages with the difference being that it's type agnostic, so 
it can contain all types not just one exclusively, the same holds true for 
the array, which functions like an dynamic array but without caring about 
types.

### Reading a json_object from a string and printing it to the command line:
```C
json_object* object;
char* str;
object = json_object_from_str("{\"integer\": 20, \"float\": 10.5, \"string\": \"Hello World!\"}");
if(object != NULL) {
    str = json_object_to_str(object, 0);
    if(str != NULL) {
        printf("%s\n", str);
        free(str);
    }
    json_object_free(object);
}
```

Output:
```
{"integer": 20,"float": 10.5,"string":"Hello World!"}
```

### Creating a new json_object, adding values and printing it to the command line:
```C
json_object* object;
char* str;
object = json_object_create();
if(object != NULL) {
    if(json_object_add_int(object, "integer", 325) != 0) {
        //Error adding value
    }

    if(json_object_add_string(object, "text", "Hello World!") != 0) {
        //Error adding value
    }

    str = json_object_to_str(object, 0);
    if(str != NULL) {
        printf("%s\n", str);
        free(str);
    }
    json_object_free(object);
}
```

Output:
```
{"integer": 325,"text":"Hello World!"}
```

### Retrieving values of a JSON string:
```C
json_object* object;
int test;
float test2;
object = json_object_from_str("{\"integer\": 20, \"float\": 10.5}");
if(object != NULL) {
    //Get "integer" as int
    if(json_object_get_int(object, "integer", &test) == 0) {
        //Successfully retrieved value
        printf("integer is: %i\n", test);
    }
    //Get "float" as float
    if(json_object_get_float(object, "float", &test2) == 0) {
        //Successfully retrieved value
        printf("float is: %f\n", test2);
    }
    //Get "float" as int Note: Automatic conversion of types works for many values, look at "https://github.com/Maschinr/JSON-C#conversion-table-todo" for further details
    if(json_object_get_int(object, "float", &test) == 0) {
        //Successfully retrieved value
        printf("float as integer is: %i\n", test);
    }
    json_object_free(object);
}
```

Output:
```
integer is: 20
float is: 10.5
float as integer is 10
```

### Nesting json_objects in json_objects
```C
json_object* object;
json_object* object2;
char* str;
object = json_object_from_str("{\"integer\": 20}");
if(object != NULL) {
    //Note: you could also load this second object from a string or file and then add it to the first
    object2 = json_object_create();
    if(object2 != NULL) {
        //Note: it's safe to delete object2 here because adding copies the object and it's content
        if(json_object_add_string(object2, "answer", "it work's") != 0) {
            //Error adding value to object
        }
        if(json_object_add_object(object, "nested", object2) != 0) {
            //Error adding value to object
        }
        json_object_free(object2);
    }
    str = json_object_to_str(object, 0);
    if(str != NULL) {
        printf("%s\n", str);
        free(str);
    }
    json_object_free(object);
}
```

Output:
```
{"integer":20,"nested":{"answer":"it work's"}}
```

## Conversion Table (TODO)

## JSON Object Functions

### json_object* json_object_create(void)

Creates a new empty json_object, returns NULL
if an error occured during initialization.

### void json_object_free(json_object* object)

Free's the given json_object, you only **need** to free the json_objects returned by json_object_create, json_object_from_file or json_object_from_str.

### json_object* json_object_from_file(const char* path)

Tries to parse the file found at **path** and creates a
new json_object out of it. Returns NULL if it is an invalid JSON file
according to [RFC 8259](https://tools.ietf.org/html/rfc8259), the 
file was not found or if the JSON doesn't hold an object at the
top most level.

### json_object* json_object_from_str(const char* str)

Tries to parse the given string **str** to a new 
json_object, returns NULL if the string doesn't comply with 
[RFC 8259](https://tools.ietf.org/html/rfc8259) or if the JSON 
doesn't hold an object at the top most level.

### int json_object_to_file(const json_object* object, const char* path, int formatted)

Tries to output the given **object** to the file at given **path**, if 
**formatted** != 0 then the output will be formatted as seen in **How to use**. Returns 0 if successfull, -1 if anything failed.

### char* json_object_to_str(const json_object* object, int formatted)

Tries to create a string representation of the given **object**, if
**formatted** != 0 then the output will be formatted as seen in **How to use**.
Returns NULL if something failed or the null terminated string representation
of the **object** if everything was successfull, the returned string 
**needs** to be freed by you.

### int json_object_get_*type*(const json_object* object, const char* name, *type** result)

Where *type* is one of the Standard C Types, json_object or json_array. Tries
to retrieve the value where the name is the same as 
**name** from the given json_object, **object**.
If the value is found inside the json_object and the 
value can be converted to the requested *type* it will
be stored inside the given *type** **result**. The 
return value is zero if the value is found and 
successfully converted and -1 if anything failed.

### int json_object_add_*type*(const json_object* object, const char* name, const *type* value)

Where *type* is one of the Standard C Types, json_object or json_array.
Tries to add an value to the json_object **object** with the name **name** 
where the stored value is 
**value**. This functions returns 0 if the value was added to the object, 
otherwise it 
will return -1. This function will not  overwrite values that are already 
inside the 
object but will return -1 if the name is already used for another value, if you want to 
overwrite the value if it's there use json_object_insert_*type*.

### int json_object_insert_*type*(const json_object* object, const char* name, const *type* value)

Where *type* is one of the Standard C Types, json_object or json_array. Behaves like 
json_object_add_*type* with the difference being that this function will overwrite
a value that is stored with **name** with the new value, if it's already there. This 
functions returns 0 if the value was successfully added, - 1 if it failed.

### int json_object_change_*type*(const json_object* object, const char* name, const *type* value)

Where *type* is one of the Standard C Types, json_object or json_array. Tries 
to change the value 
with **name** already inside **object** to **value**. This function 
returns 0 if it 
successfully changed the value to **value** or -1 if the value didn't 
exist in the array or other errors.

### int json_object_remove(json_object* object, const char* name)

Removes the value with **name** from **object**, returns 0 if it was 
successfully removed or -1 if an error occured or no value with this name was
found.

## JSON Array Functions (TODO)

## Generic Functions (If C11 compiled)
Most of the above functions have a generic selection
interface which will automatically choose the right 
function for the given types, the functions that are 
chosen will behave exactly the same as the corresponding 
ones explained in **JSON Object Functions** above.

| Non generic Function  | Generic Interface |
|:---|:---|
|json_object_get_*type*|json_object_get|
|json_object_add_*type*|json_object_add|
|json_object_insert_*type*|json_object_insert|
|json_object_change_*type*|json_object_change|

### Using Generic Functions
Just use them as you would the non generic functions but leave out the 
_*type* in the name, the type that is than used is determined by the type
of the value you pass.

```C
json_object* object;
int test;
float test2;
object = json_object_from_str("{\"integer\": 20, \"float\": 10.5}");
if(object != NULL) {
    //Get "integer" as int because test is an int
    if(json_object_get(object, "integer", &test) == 0) {
        //Successfully retrieved value
        printf("integer is: %i\n", test);
    }
    //Get "float" as float because test2 is a float
    if(json_object_get(object, "float", &test2) == 0) {
        //Successfully retrieved value
        printf("float is: %f\n", test2);
    }
    //Get "integer" as float because test2 is a float
    if(json_object_get(object, "integer", &test2) == 0) {
        //Successfully retrieved value
        printf("float is: %f\n", test2);
    }
    //Added a new value with the name "string" with the content "Hello World"
    if(json_object_add(object, "string", "Hello World") == 0) {
        //Successfully added value
    }
    
    json_object_free(object);
}
```

Output:
```
integer is: 20
float is: 10.5
integer as float is 10.0
```

## Test's

The entire functionality is tested with the [Unity Test API](https://github.com/ThrowTheSwitch/Unity) as best as i could. All Test's are implemented in [this file](test/TestJson.c).