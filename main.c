#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#include "parser.h"
#include "login.h"
#include "globals.h"

int printMOTD()
{
    printf("Welcome to use EDUAdmin.\n");
    return 0;
}

int printHelp()
{
    for (uint8_t *p = &_binary_HELP_start; p != &_binary_HELP_end; p++)
    {
        putchar(*p);
    }
    putchar('\n');
    return 0;
}
int interactiveShell()
{
    strcpy(USERNAME, "anonymous");
    while (true)
    {
        printf("[%s]> ", USERNAME);
        char cmd[MAX_LEN] = {0};
        int len = readline(cmd, MAX_LEN);
        if (len == 0)
        {
            continue;
        }
        else
        {
            char *argv[MAX_ARGC] = {0};
            int argc = parseArgs(argv, cmd);
            if (strcmp(argv[0], "login") == 0)
            {
                // login
                login();
            }
            else if (strcmp(argv[0], "help") == 0)
            {
                printHelp();
            }
            else if (strcmp(argv[0], "quit") == 0)
            {
                printf("Bye!\n");
                return 0;
            }
            else
            {
                printf("Unknown command: %s\n", argv[0]);
                printHelp();
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[MAX_ARGC])
{
    printMOTD();
    if (argc == 1) // No args, redir to -i
    {
        printf("No arguments, interactive mode.\n");
        return interactiveShell();
    }
    else
    {                                   // with args
        if (strcmp(argv[1], "-i") == 0) // -i interactive
        {
            printf("Interactive mode.\n");
            return interactiveShell();
        }
        else if (strcmp(argv[1], "-c") == 0) // -c
        {
            // Not implemented
        }
        else if (strcmp(argv[1], "-h"))
        { // -h help
            printf("Help mode.\n");
            return printHelp();
            // Not implemented
        }
    }
    return 0;
}