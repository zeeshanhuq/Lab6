/*
* Filename: main.c
* Author: Zeeshan Huq
* Date: 10/25/26
* Description: Vector calculator main program.
* Compile: gcc -o vect main.c vectorops.c vectorstore.c vectorui.c vectorutil.c vectorfile.c -lm
*/

#include "vectorui.h"
#include "vectorstore.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{   
    atexit(cleanupStorage);
    if (argc > 1){
        if (strcmp(argv[1], "-h") == 0) {
            
            printUI();
            return 0;
        }
        printf("Error: Unrecognized command line argument '%s'.\n", argv[1]);
        return 1;
    }
    runUI();
    return 0;
}