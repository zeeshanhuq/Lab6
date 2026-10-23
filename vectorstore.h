/*
* Filename: vectorstore.h
* Author: Zeeshan Huq
* Date: 10/23/26
* Description: Header file for vector storage functions.
*/

#ifndef VECTORSTORE_H
#define VECTORSTORE_H

#include "vectorops.h"

int addVect(vect v);
int findVect(const char *name);
int getVector(int index, vect *result);
int vectorCount(void);
int clear();
void cleanupStorage(void);
#endif // VECTORSTORE_H