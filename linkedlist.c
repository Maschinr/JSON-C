#include <linkedlist.h>

#define TYPE(type, name)\
name##_list* name##_list_create() {\
    name##_list* t;\
    t = NULL;\
    t = malloc(sizeof(name##_list));\
    if(t != NULL) {\
        t->size = 0;\
        t->first = NULL;\
    }\
    return t;\
}\
\
void name##_list_free(name##_list* list) {\
    name##_list_element* elem;\
    elem = list->first;\
    while(elem != NULL) {\
        name##_list_element* next_elem = elem->next;\
        free(elem);\
        elem = next_elem;\
    }\
    free(list);\
}\
\
name##_list_element* name##_list_element_create(type value) {\
    name##_list_element* elem;\
    elem = NULL;\
    elem = malloc(sizeof(name##_list_element));\
    if(elem != NULL) {\
        elem->next = NULL;\
        elem->data = value;\
    }\
    \
    return elem;\
}\
\
void name##_list_add_end(name##_list* list, type value) {\
    /*Create new Element*/\
    name##_list_element* elem;\
    elem = name##_list_element_create(value);\
    \
    if(list->first == NULL) {\
        list->first = elem;\
    } else {\
        /*Skip to last Element*/\
        name##_list_element* last;\
        last = list->first;\
        while(last->next != NULL) {\
            last = last->next;\
        }\
        last->next = elem;\
        \
    }\
    \
    list->size = list->size + 1;\
}\
\
void name##_list_add_first(name##_list* list, type value) {\
    name##_list_element* elem;\
    elem = name##_list_element_create(value);\
    if(list->first == NULL) {\
        list->first = elem;\
    } else {\
        elem->next = list->first;\
        list->first = elem;\
    }\
    list->size = list->size + 1;\
}\
\
bool name##_list_add(name##_list* list, type value, unsigned int position) {\
    \
}\
\
type* name##_list_get(name##_list* list, unsigned int position) {\
    type* result;\
    result = NULL;\
    \
    if(position < list->size) {\
        name##_list_element* elem;\
        elem = list->first;\
        for(unsigned int i = 0; i < position ; i++) {\
            elem = elem->next;\
        }\
        result = &elem->data;\
    };\
    \
    return result;\
}


LINKED_LIST_TYPES;

#undef TYPE