/*
* Filename: vectorui.c
* Author: Zeeshan Huq
* Date: 10/23/26
* Description: User interface for vector operations.
*/

#include "vectorops.h"
#include "vectorstore.h"
#include "vectorfile.h"
#include "vectorutil.h"
#include "vectorui.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINE_LENGTH 256
#define MAX_FILL 20000

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
    int n = vectorCount();
    for (int i = 0; i < n; i++) {
        if(getVector(i, &v) == 0) {
            printVector(&v);
        }
    }
    if (n == 0) {
        printf("No vectors stored.\n");
    }
}

static void handleLoad(const char *fname)
{
    int loaded, skipped, firstBad;
    int rc = loadVectors(fname, &loaded, &skipped, &firstBad);

    switch (rc) {
        case FILE_OK:
            if (loaded == 0 && skipped == 0) {
                printf("File '%s' is empty. Nothing loaded.\n", fname);
            } else if (loaded == 0) {
                printf("Error: '%s' has no valid vectors (%d malformed line(s), "
                       "first at line %d). Expected lines like: name,x,y,z\n",
                       fname, skipped, firstBad);
            } else {
                printf("Loaded %d vector(s) from '%s'.\n", loaded, fname);
                if (skipped > 0) {
                    printf("Warning: skipped %d malformed line(s), first at line %d.\n",
                           skipped, firstBad);
                }
            }
            break;
        case FILE_ERR_OPEN:
            printf("Error: Cannot open '%s'. Check that the file exists and the path is correct.\n", fname);
            break;
        case FILE_ERR_MEMORY:
            printf("Error: Out of memory after loading %d vector(s).\n", loaded);
            break;
        default:
            printf("Error: Problem reading '%s'. Is it a regular file?\n", fname);
            break;
    }
}

static void handleSave(const char *fname)
{
    int saved;
    int rc = saveVectors(fname, &saved);

    if (rc == FILE_OK) {
        printf("Saved %d vector(s) to '%s'.\n", saved, fname);
    } else if (rc == FILE_ERR_OPEN) {
        printf("Error: Cannot create '%s'. Check the path and permissions.\n", fname);
    } else {
        printf("Error: Problem writing '%s'.\n", fname);
    }
}

// fill <count>: creates v1..vN with pseudo-random values (exercises storage)
static void handleFill(const char *arg)
{
    char *end;
    long n = strtol(arg, &end, 10);

    if (*end != '\0' || n < 1 || n > MAX_FILL) {
        printf("Usage: fill <count>   (1 to %d)\n", MAX_FILL);
        return;
    }
    for (long i = 1; i <= n; i++) {
        vect v;
        snprintf(v.name, sizeof v.name, "v%ld", i);
        v.x = (rand() % 2001) / 10.0 - 100.0;
        v.y = (rand() % 2001) / 10.0 - 100.0;
        v.z = (rand() % 2001) / 10.0 - 100.0;
        if (addVect(v) < 0) {
            printf("Error: Out of memory after creating %ld vector(s).\n", i - 1);
            return;
        }
    }
    printf("Created %ld vector(s): v1 to v%ld.\n", n, n);
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
        printf("Error: Out of memory. Cannot add vector '%s'.\n", name);
        return 0;
    }
    printVector(&result);
    return 1;
}

static int handleLine(char *line)
{
    char *eq;
    vect v;
    char cmd[LINE_LENGTH], arg[LINE_LENGTH], extra;
    int words;

    if(*line == '\0') {
        return 0; // Ignore empty lines
    }

    // First word, optional second word, and a check for anything after that
    words = sscanf(line, "%255s %255s %c", cmd, arg, &extra);

    if(strcmp(line, "clear") == 0) {
        clear();
        printf("Vector memory cleared.\n");
    }
    else if(strcmp(line, "list") == 0) {
        handleList();
    }
    else if(strcmp(cmd, "load") == 0) {
        if (words == 2) handleLoad(arg); else printf("Usage: load <filename>\n");
    }
    else if(strcmp(cmd, "save") == 0) {
        if (words == 2) handleSave(arg); else printf("Usage: save <filename>\n");
    }
    else if(strcmp(cmd, "fill") == 0) {
        if (words == 2) handleFill(arg); else printf("Usage: fill <count>\n");
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
    printf("minimat - a tiny vector calculator (3D vectors, unlimited storage)\n\n"
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
           "  load <file>       load vectors from a CSV file (name,x,y,z per line);\n"
           "                    merges with stored vectors, same names are overwritten\n"
           "  save <file>       save all vectors to a CSV file (overwrites the file)\n"
           "  fill <n>          create n test vectors named v1..vn (max %d)\n"
           "  quit              exit\n"
           "Put spaces around =, +, - and *. File names cannot contain spaces.\n",
           MAX_FILL);
    return 0;
}

int runUI(void)
{
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
