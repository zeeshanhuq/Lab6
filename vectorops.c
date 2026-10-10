/*
* Filename: vectorops.c
* Author: Zeeshan Huq
* Date: 10/1/26
* Description: Defines vector operations.
*/

#include "vectorops.h"
#include <math.h>

vect add(vect a, vect b)
{
    vect temp;
    temp.x = a.x + b.x;
    temp.y = a.y + b.y;
    temp.z = a.z + b.z;
    return temp;
}
vect sub(vect a, vect b)
{
    vect temp;
    temp.x = a.x - b.x;
    temp.y = a.y - b.y;
    temp.z = a.z - b.z;
    return temp;
}
vect scalar(vect a, double b)
{
    vect temp;
    temp.x = a.x*b;
    temp.y = a.y*b;
    temp.z = a.z*b;
    return temp;
}