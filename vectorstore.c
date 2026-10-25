/*
* Filename: vectorstore.c
* Author: Zeeshan Huq
* Date: 10/23/26
* Description: Defines storage functions for vectors.
*/

#include "vectorstore.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#define INITIAL_CAPACITY 10
static vect *vectorArray = NULL;
static int count;
static int capacity = 0;


int findVect(const char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(vectorArray[i].name, name) == 0) {
            return i; // Return index of found vector
        }
    }
    return -1; // Vector not found
}

static int grow(void){
    if(capacity > INT_MAX/2){
        return -1;
    }
    int newCapacity = (capacity==0) ? INITIAL_CAPACITY : capacity * 2;
    vect *bigger = realloc(vectorArray, (size_t)newCapacity * sizeof(vect));
    if(bigger == NULL){
        return -1;
    }
    vectorArray = bigger;
    capacity = newCapacity;
    return 0;
}

int addVect(vect v) {
    int index = findVect(v.name);
    if (index >= 0) {
        // Replace existing vector
        vectorArray[index] = v;
        return index; // Return index of replaced vector
    }
    if(count == capacity && grow() != 0){
        return -1;
    }
    vectorArray[count] = v;
    return count++;
}

int getVector(int index, vect *result) {
    if (index < 0 || index >= count) {
        return -1; // Invalid index or unused slot
    }
    *result = vectorArray[index];
    return 0; // Success
}

int vectorCount(void){
    return count;
}

int clear() {
    free(vectorArray);
    vectorArray = NULL;
    count = 0;
    capacity = 0;
    return 0;
}

void cleanupStorage(void){
    clear();
}