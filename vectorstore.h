/*
* Filename: vectorstore.h
* Author: Zeeshan Huq
* Date: 10/1/26
* Description: Header file for vector storage functions.
*/

#ifndef VECTORSTORE_H
#define VECTORSTORE_H

#include "vectorops.h"
#define STARTING_VECTORS 10
extern int vectorCount;
int initialize();
int addVect(vect v);
int findVect(const char *name);
int getVector(int index, vect *result);
int clear();
int reallocateVectors();
#endif // VECTORSTORE_H