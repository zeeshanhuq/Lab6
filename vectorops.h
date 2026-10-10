/*
* Filename: vectorops.h
* Author: Zeeshan Huq
* Date: 10/1/26
* Description: Header file for vector operations.
*/
#ifndef VECTOROPS_H
#define VECTOROPS_H

#define NAME_LENGTH 50
typedef struct 
{
    char name[NAME_LENGTH];
    double x;
    double y;
    double z;
} vect;

vect add(vect a, vect b);
vect sub(vect a, vect b);
vect scalar(vect a, double b);

#endif // VECTOROPS_H