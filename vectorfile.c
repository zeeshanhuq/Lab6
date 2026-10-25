/*
* Filename: vectorfile.c
* Author: Zeeshan Huq
* Date: 10/23/26
* Description: File IO for vector operations.
*/

#include "vectorfile.h"
#include "vectorops.h"
#include "vectorstore.h"
#include "vectorutil.h"
#include <stdio.h>
#include <string.h>

#define CSV_LINE_MAX 512

static int parseCsvLine(char *line, vect *out){
    char *fields[4];
    double vals[3];
    char *p = line;

    for(int i = 0; i < 3; i++){
        char *comma = strchr(p, ',');
        if (comma == NULL){
            return 0;
        }
        *comma = '\0';
        fields[i] = p;
        p = comma + 1;
    }
    if(strchr(p, ',') != NULL){
        return 0;
    }
    fields[3] = p;
    char *name = trimWhitespace(fields[0]);
    if(!validateVectorName(name)){
        return 0;
    }
    for(int i = 0; i < 3; i++){
        if(!isNumber(trimWhitespace(fields[i+1]), &vals[i])){
            return 0;
        }
    }
    strcpy(out -> name, name);
    out -> x = vals[0];
    out -> y = vals[1];
    out -> z = vals[2];
    return 1;
}

int loadVectors(const char *fname, int *loaded, int *skipped, int *firstBadLine) {
    char line[CSV_LINE_MAX];
    int lineNum = 0;
    int result = FILE_OK;

    *loaded = 0;
    *skipped = 0;
    *firstBadLine = 0;

    FILE *fp = fopen(fname, "r");
    if (fp == NULL) {
        return FILE_ERR_OPEN;
    }

    while (fgets(line, sizeof line, fp) != NULL) {
        int tooLong = 0;
        lineNum++;

        if (strchr(line, '\n') == NULL && !feof(fp)) {
            int ch;
            tooLong = 1;
            while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
                ;
            }
        }

        char *text = line;

        if (lineNum == 1 && (unsigned char)text[0] == 0xEF &&
            (unsigned char)text[1] == 0xBB && (unsigned char)text[2] == 0xBF) {
            text += 3;
        }
        text = trimWhitespace(text); 

        if (*text == '\0') {
            continue; 
        }

        vect v;
        if (tooLong || !parseCsvLine(text, &v)) {
            (*skipped)++;
            if (*firstBadLine == 0) {
                *firstBadLine = lineNum;
            }
            continue;
        }
        if (addVect(v) < 0) {
            result = FILE_ERR_MEMORY;
            break;
        }
        (*loaded)++;
    }

    if (result == FILE_OK && ferror(fp)) {
        result = FILE_ERR_IO;
    }
    fclose(fp);
    return result;
}

int saveVectors(const char *fname, int *saved) {
    *saved = 0;

    FILE *fp = fopen(fname, "w");
    if (fp == NULL) {
        return FILE_ERR_OPEN;
    }

    int n = vectorCount();
    for (int i = 0; i < n; i++) {
        vect v;
        if (getVector(i, &v) != 0) {
            continue;
        }
        if (fprintf(fp, "%s,%.15g,%.15g,%.15g\n", v.name, v.x, v.y, v.z) < 0) {
            fclose(fp);
            return FILE_ERR_IO;
        }
        (*saved)++;
    }

    if (fclose(fp) != 0) {
        return FILE_ERR_IO;
    }
    return FILE_OK;
}
