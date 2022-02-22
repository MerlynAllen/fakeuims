#include "parser.h"

bool isChars(char c, char *chars)
{
    for (int i = 0; i < strlen(chars); i++)
    {
        if (c == chars[i])
        {
            return true;
        }
    }
    return false;
}

int parseArgs(char *argv[MAX_ARGC], char *str)
{
    int argc = 0;
    int len = strlen(str);
    for (int i = 0; i < len && argc < MAX_ARGC;)
    {
        // First check if alnumoper
        if (!isChars(str[i], WHITESPACE))
        {
            argv[argc] = &str[i]; // Yes, a new argument, mark start position and skip all alnumoper
            for (; i < len && !isChars(str[i], WHITESPACE); i++)
                ;
            argc++; // Commit new argument
        }
        else
        { // Not alnumoper, no arguments, break
            break;
        }
        // Skipped all alnumoper, now check if space/tab
        if (isChars(str[i], SPACE))
        {
            str[i] = '\0'; // Replace space/tab with null terminator
            for (; i < len && isChars(str[++i], SPACE);)
                ; // Skip all spaces/tabs
        }
        else // not space/tab, break
        {
            break;
        }
    }
    return argc;
}

int readline(char *str, int maxlen)
{
    int len = 0;
    char c;
    while (len < maxlen - 1 && (c = getchar()) != '\n' && c != EOF)
    {
        str[len++] = c;
    }
    str[len] = '\0';
    return len;
}