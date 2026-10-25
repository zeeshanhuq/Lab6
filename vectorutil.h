/*
* Filename: vectorutil.h
* Author: Zeeshan Huq
* Date: 10/23/26
* Description: Helpers for UI and file layers
*/

#ifndef VECTORUTIL_H
#define VECTORUTIL_H

char *trimWhitespace(char *str);
int validateVectorName(const char *name);
int isNumber(const char *str, double *value);
#endif