#include <json_parse.h>
#include <string.h>
#include <stdlib.h>

char* parse_string(unsigned int begin, const char* str, unsigned int* end) {
    const unsigned int str_length = strlen(str);
    
    for(unsigned int i = begin; i < str_length; i++) {
        if(str[i] != ' ') {
            if(str[i] == '\"') {
                i = i + 1;
                char buffer[100];
                memset(&buffer, '\0', 100);
                for(unsigned int ix = 0; ix < 100; ix++) {
                    if(str[i + ix] == '\"') { // string ended
                        *end = i + ix + 1;
                        char* res = malloc(strlen(buffer) + 1);
                        if(res == NULL) {
                            return NULL;
                        }
                        strcpy(res, buffer);
                        return res;
                    } else if(str[i + ix] == '\\') { //if backslash then look if valid char is after
                        if(str[i + ix + 1] == '\"') {
                            buffer[ix] = '\"';
                        } else if(str[i + ix + 1] == '\\') {
                            buffer[ix] = '\\';
                        } else if(str[i + ix + 1] == '/') {
                            buffer[ix] = '/';
                        } else if(str[i + ix + 1] == 'b') {
                            buffer[ix] = '\b';
                        } else if(str[i + ix + 1] == 'f') {
                            buffer[ix] = '\f';
                        } else if(str[i + ix + 1] == 'n') {
                            buffer[ix] = '\n';
                        } else if(str[i + ix + 1] == 'r') {
                            buffer[ix] = '\r';
                        } else if(str[i + ix + 1] == 't') {
                            buffer[ix] = '\t';
                        } else if(str[i + ix + 1] == 'u') {
                            return NULL; // TODO
                            //buffer[ix] = '\u';//todo format to hexadecimal
                        }  else {
                            return NULL;
                        }
                            
                        i = i + 1; // skip one char
                        
                    } else { // any other char add to buffer
                        buffer[ix] = str[i + ix];
                    }
                }
            } else {
                return NULL;
            }
        }
    }
}