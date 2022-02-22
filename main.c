#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<time.h>
#include<stdint.h>

#include "parser.h"

int printMOTD()
{
    printf("Welcome to use EDUAdmin.\n");
    return 0;
}

int interactiveShell()
{

}

int main(int argc, char *argv[MAX_ARGC])
{
    printMOTD();
    if (argc == 1) // Nor args redir to -i
    {
        return interactiveShell();
    } else { // with args

    }
    return 0;
}