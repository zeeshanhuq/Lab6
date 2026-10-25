/*
* Filename: vectorfile.h
* Author: Zeeshan Huq
* Date: 10/23/26
* Description: File IO for vector operations.
*/

#ifndef VECTORFILE_H
#define VECTORFILE_H

#define FILE_OK          0
#define FILE_ERR_OPEN   -1
#define FILE_ERR_MEMORY -2
#define FILE_ERR_IO     -3


int loadVectors(const char *fname, int *loaded, int *skipped, int *firstBadLine);
int saveVectors(const char *fname, int *saved);

#endif