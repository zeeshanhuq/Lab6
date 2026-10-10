/*
* Filename: vectorstore.h
* Author: Zeeshan Huq
* Date: 10/1/26
* Description: Header file for vector storage functions.
*/

#ifndef VECTORSTORE_H
#define VECTORSTORE_H

#include "vectorops.h"
#define MAX_VECTORS 10
int addVect(vect v);
int findVect(const char *name);
int getVector(int index, vect *result);
int clear();
#endif // VECTORSTORE_H