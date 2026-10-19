/*
* Filename: vectorstore.c
* Author: Zeeshan Huq
* Date: 10/1/26
* Description: Defines storage functions for vectors.
*/

#include "vectorstore.h"
#include <string.h>
#include <stdlib.h>

static vect *vectorArray;
static int *used;
int vectorCount = STARTING_VECTORS;

int initialize(){
    vectorArray = malloc(STARTING_VECTORS * sizeof(vect));
    used = malloc(STARTING_VECTORS * sizeof(int));
    return 0;
}

int findVect(const char* name) {
    for (int i = 0; i < vectorCount; i++) {
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
    for (int i = 0; i < vectorCount; i++) {
        if (!used[i]) {
            vectorArray[i] = v;
            used[i] = 1;
            return i; // Return index where vector is stored
        }
    }
    return -1; // No space available
}
int getVector(int index, vect *result) {
    if (index < 0 || index >= vectorCount || !used[index]) {
        return -1; // Invalid index or unused slot
    }
    *result = vectorArray[index];
    return 0; // Success
}

int clear() {
    for (int i = 0; i < vectorCount; i++) {
        used[i] = 0; // Mark all slots as unused
    }
    return 0; // Success
}

int reallocateVectors() {
    int newSize = vectorCount * 2;
    vect *newArray = (vect *)malloc(newSize * sizeof(vect));
    int *newUsed = (int *)malloc(newSize * sizeof(int));

    if (!newArray || !newUsed) {
        // Memory allocation failed
        return -1;
    }

    // Copy existing vectors and usage flags to new arrays
    for (int i = 0; i < vectorCount; i++) {
        newArray[i] = vectorArray[i];
        newUsed[i] = used[i];
    }

    // Initialize the rest of the new arrays
    for (int i = vectorCount; i < newSize; i++) {
        newUsed[i] = 0; // Mark as unused
    }

    // Free old arrays and update pointers
    free(vectorArray);
    free(used);
    vectorArray = newArray;
    used = newUsed;
    vectorCount = newSize;

    return 0; // Success
}