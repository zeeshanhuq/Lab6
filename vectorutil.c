/*
* Filename: vectorutil.c
* Author: Zeeshan Huq
* Date: 10/23/26
* Description: Helpers for UI and file layers
*/

#include "vectorutil.h"
#include "vectorops.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *trimWhitespace(char *str){
    char *end;

    while (isspace((unsigned char)*str)){
      str++;
    }

    if(*str == 0){
        return *str;
    }
    end = str + strlen(str) - 1;
    while( end > str && isspace((unsigned char) *end)){
        end--;
    }
    *(end + 1) = '\0';
    return str;
}

int validateVectorName(const char *name) {
    static const char *reserved[] = {"quit", "list", "clear", "load", "save", "fill"};

    if (!isalpha((unsigned char)name[0]) || strlen(name) >= NAME_LENGTH) {
        return 0; 
    }
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalnum((unsigned char)name[i]) && name[i] != '_') {
            return 0; 
        }
    }
    for (size_t i = 0; i < sizeof reserved / sizeof reserved[0]; i++) {
        if (strcmp(name, reserved[i]) == 0) {
            return 0;
        }
    }
    return 1; 
}

// The first-character check stops strtod from accepting "", "inf", "nan".
int isNumber(const char *str, double *value) {
    char *endptr;
    double d;

    if (!(isdigit((unsigned char)*str) || *str == '-' || *str == '+' || *str == '.')) {
        return 0;
    }
    d = strtod(str, &endptr);
    if (endptr == str || *endptr != '\0') {
        return 0;
    }
    *value = d;
    return 1;
}
