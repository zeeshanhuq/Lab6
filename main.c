/*
* Filename: main.c
* Author: Zeeshan Huq
* Date: 10/3/26
* Description: Vector calculator main program.
* Compile: gcc -o vector main.c vectorops.c vectorstore.c vectorui.c -lm
*/

#include "vectorui.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{   
    printf("Lab 6 Git  and Github Version of Vector Calculator\n");
    if (argc > 1){
        if (strcmp(argv[1], "-") == 0) {
            
            printUI();
            return 0;
        }
        printf("Error: Unrecognized command line argument '%s'.\n", argv[1]);
        return 1;
    }
    runUI();
    return 0;
}