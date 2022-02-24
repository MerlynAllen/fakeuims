#ifndef PARSER
#define PARSER

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "limits.h"

#define ALNUM "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"
#define ALNUMOPER \
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-="
#define EOL "\n\r\0"
#define WHITESPACE " \t\n\r\0"
#define SPACE " \t"

bool isChars(char c, char *chars);
int readline(char *str, int maxlen);
int parseArgs(char *argv[MAX_ARGC], char *str);

#endif