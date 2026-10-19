/*
* Filename: vectorui.c
* Author: Zeeshan Huq
* Date: 10/3/26
* Description: User interface for vector operations.
*/

#include "vectorops.h"
#include "vectorstore.h"
#include "vectorui.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define LINE_LENGTH 256

static char *trimWhitespace(char *str) 
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
    {
        str++;
    } 

    if (*str == 0)
    {  // All spaces?
        return str;
    }
    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
    { 
        end--;
    }
    // Write new null terminator
    *(end + 1) = '\0';

    return str;
}

static int validateVectorName(const char *name) {
    if (strlen(name) == 0 || strlen(name) >= NAME_LENGTH) {
        return 0; // Invalid name length
    }
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalnum((unsigned char)name[i]) && name[i] != '_') {
            return 0; // Invalid character found
        }
    }
    return 1; // Valid name
}

static int isNumber(const char *str, double *value) {
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

static void printVector(const vect *v) {
    printf("Vector Name: %s\n", v->name);
    printf("Coordinates: (%.2f, %.2f, %.2f)\n", v->x, v->y, v->z);
}

static int lookupVectorByName(const char *name, vect *result) {
    int index = findVect(name);
    if (index != -1 && getVector(index, result) == 0) {
        return 1; // Found
    }
    return 0; // Not found
}

// Parses only the values ("1 2 3", "1, 2, 3", or "1 2" with z = 0).
// The name is NOT in this string: handleAssignment already split it off.
static int parseLiteralVector(const char *input, vect *result) {
    char buf[LINE_LENGTH];
    double vals[3] = {0.0, 0.0, 0.0};
    int n = 0;

    strncpy(buf, input, sizeof buf - 1);
    buf[sizeof buf - 1] = '\0';

    for (char *tok = strtok(buf, " ,\t"); tok; tok = strtok(NULL, " ,\t")) {
        if (n == 3 || !isNumber(tok, &vals[n])) {
            return 0; // Too many values or not a number
        }
        n++;
    }
    if (n < 2) {
        return 0; // Need at least x and y
    }

    result->x = vals[0];
    result->y = vals[1];
    result->z = vals[2];
    return 1; // Successfully parsed
}

static int evaluateExpression(const char *expr, vect *result) {
    char name1[LINE_LENGTH], name2[LINE_LENGTH];
    char op, extra;
    vect v1, v2;
    double factor;

    int n = sscanf(expr, "%255s %c %255s %c", name1, &op, name2, &extra);
    if(n != 3){
        printf("Error: Invalid expression format.\n");
        return 0; // Invalid format
    }
    switch (op) {
        case '+':
            if(!lookupVectorByName(name1, &v1) || !lookupVectorByName(name2, &v2)) {
                printf("Error: One or both vectors not found for addition.\n");
                return 0;
            }
            *result = add(v1, v2);
            break;
        case '-':
            if(!lookupVectorByName(name1, &v1) || !lookupVectorByName(name2, &v2)) {
                printf("Error: One or both vectors not found for subtraction.\n");
                return 0;
            }
            *result = sub(v1, v2);
            break;
        case '*':
            // The vector always ends up in v1, whichever side it was on
            if(isNumber(name1, &factor)){
                if(!lookupVectorByName(name2, &v1)) {
                    printf("Error: Vector '%s' not found for scalar multiplication.\n", name2);
                    return 0;
                }
            }
            else if(isNumber(name2, &factor)){
                if(!lookupVectorByName(name1, &v1)) {
                    printf("Error: Vector '%s' not found for scalar multiplication.\n", name1);
                    return 0;
                }
            }
            else{
                printf("Error: One operand must be a vector and the other a scalar for multiplication.\n");
                return 0;
            }
            *result = scalar(v1, factor);
            break;
        default:
            printf("Error: Unsupported operator '%c'.\n", op);
            return 0; // Unsupported operator
    }

    strcpy(result->name, "ans"); // Unassigned results display as ans
    return 1; // Successfully evaluated
}

static void handleList(void){
    vect v;
    int count = 0;
    for (int i = 0; i < vectorCount; i++) {
        if(getVector(i, &v) == 0) {
            printVector(&v);
            count++;
        }
    }
    if (count == 0) {
        printf("No vectors stored.\n");
    }
}

static int handleAssignment(char *line, char *eq)
{
    *eq = '\0'; // Split the line into name and expression
    char *name = trimWhitespace(line);
    char *expr = trimWhitespace(eq + 1);
    vect result;
    if (!validateVectorName(name)) {
        printf("Error: Invalid vector name '%s'.\n", name);
        return 0;
    }
    if(*expr == '\0') {
        printf("Error: No expression provided for assignment.\n");
        return 0;
    }
    // Literal first; evaluateExpression prints its own error if this fails too
    if(!parseLiteralVector(expr, &result) && !evaluateExpression(expr, &result)) {
        return 0;
    }
    strcpy(result.name, name);
    if (addVect(result) < 0) {
        reallocateVectors();
        return 0;
    }
    printVector(&result);
    return 1;
}

static int handleLine(char *line)
{
    char *eq;
    vect v;
    if(*line == '\0') {
        return 0; // Ignore empty lines
    }
    if(strcmp(line, "clear") == 0) {
        clear();
        printf("Vector memory cleared.\n");
    }
    else if(strcmp(line, "list") == 0) {
        handleList();
    }
    else if((eq = strchr(line, '=')) != NULL) {
        return handleAssignment(line, eq);
    }
    else if(validateVectorName(line)){
        if(lookupVectorByName(line, &v)) {
            printVector(&v);
        }
        else {
            printf("Error: Vector '%s' not found.\n", line);
        }
    }
    else if(evaluateExpression(line, &v)) {
        printVector(&v);
    }
    return 1;
}

int printUI(void)
{
    printf("minimat - a tiny vector calculator (3D vectors, %d max)\n\n"
           "Usage: minimat [-h]\n\n"
           "Commands:\n"
           "  name = x y z      create/replace a vector (commas or spaces; z optional)\n"
           "  name              display a vector\n"
           "  a + b             add two vectors\n"
           "  a - b             subtract two vectors\n"
           "  a * 2, 2 * a      scalar multiplication\n"
           "  c = a + b         operate and assign (also works with - and *)\n"
           "  list              show all stored vectors\n"
           "  clear             delete all stored vectors\n"
           "  quit              exit\n"
           "Put spaces around =, +, - and *.\n", vectorCount);
    return 0;
}

int runUI(void)
{
    initialize();
    char line[LINE_LENGTH];

    for (;;) {
        printf("minimat> ");
        fflush(stdout);

        if (fgets(line, sizeof line, stdin) == NULL) {
            printf("\n");   // EOF (Ctrl-D)
            break;
        }

        char *cmd = trimWhitespace(line);
        if (strcmp(cmd, "quit") == 0) {
            break;
        }
        handleLine(cmd);
    }
    return 0;
}