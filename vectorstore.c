/*
* Filename: vectorstore.c
* Author: Zeeshan Huq
* Date: 10/1/26
* Description: Defines storage functions for vectors.
*/

#include "vectorstore.h"
#include <string.h>

static vect vectorArray[MAX_VECTORS];
static int used[MAX_VECTORS];

int findVect(const char* name) {
    for (int i = 0; i < MAX_VECTORS; i++) {
        if (used[i] && strcmp(vectorArray[i].name, name) == 0) {
            return i; // Return index of found vector
        }
    }
    return -1; // Vector not found
}

int addVect(vect v) {
    int index = findVect(v.name);
    if (index >= 0) {
        // Replace existing vector
        vectorArray[index] = v;
        return index; // Return index of replaced vector
    }
    // Add new vector
    for (int i = 0; i < MAX_VECTORS; i++) {
        if (!used[i]) {
            vectorArray[i] = v;
            used[i] = 1;
            return i; // Return index where vector is stored
        }
    }
    return -1; // No space available
}
int getVector(int index, vect *result) {
    if (index < 0 || index >= MAX_VECTORS || !used[index]) {
        return -1; // Invalid index or unused slot
    }
    *result = vectorArray[index];
    return 0; // Success
}

int clear() {
    for (int i = 0; i < 10; i++) {
        used[i] = 0; // Mark all slots as unused
    }
    return 0; // Success
}