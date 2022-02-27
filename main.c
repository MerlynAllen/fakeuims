#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include <stdarg.h>

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

bool checkArgs(char *arg, int argc, ...)
{
    va_list ap;
    va_start(ap, argc);
    // printf("checking arg for input %s\n", arg);
    for (int i = 0; i < argc; ++i)
    {
        if (strcmp(arg, va_arg(ap, char *)) == 0)
        {
            va_end(ap);
            return true;
        }
    }
    va_end(ap);
    return false;
}

int interactiveShell()
{

    strcpy(USERNAME, "anonymous");
    // init userlist
    initData();
    loadData();
    if (getUserInfo() == -1)
    {
        initUserProfile();
    }
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
            if (checkArgs(argv[0], 1, "login"))
            {
                // login
                login();
            }
            else if (checkArgs(argv[0], 2, "help", "?"))
            {
                printHelp();
            }
            else if (checkArgs(argv[0], 2, "quit", "exit"))
            {
                printf("Bye!\n");
                break;
            }
            else if (checkArgs(argv[0], 1, "add"))
            {
                if (checkArgs(argv[1], 1, "user"))
                {
                    createUser();
                }
                else if (checkArgs(argv[1], 1, "admin"))
                {
                    createAdmin();
                }
                else if (checkArgs(argv[1], 1, "course"))
                {
                    userAddCourse();
                }
                else if (checkArgs(argv[1], 1, "room"))
                {
                    userAddRoom();
                }
                else
                {
                    printf("Unknown subcommand %s\n", argv[1]);
                }
            }
            else if (checkArgs(argv[0], 1, "list"))
            {
                if (checkArgs(argv[1], 1, "user"))
                {
                    listUsers();
                }
                else if (checkArgs(argv[1], 1, "course"))
                {
                    listCourses();
                }
                else if (checkArgs(argv[1], 1, "teacher"))
                {
                    listTeachers();
                }
                else if (checkArgs(argv[1], 1, "student"))
                {
                    listStudents();
                }
                else if (checkArgs(argv[1], 1, "room"))
                {
                    listRooms();
                }
                else
                {
                    printf("Unknown subcommand %s\n", argv[1]);
                }
            }
            else if (checkArgs(argv[0], 1, "show"))
            {
                if (checkArgs(argv[1], 1, "student"))
                {
                    if (argc == 3)
                    {
                        showStudentInfo(atoi(argv[2]));
                    }
                    else
                    {
                        printf("Expected argc 3, but got %d\n", argc);
                    }
                }
                else if (checkArgs(argv[1], 1, "course"))
                {
                    if (argc == 3)
                    {
                        showCourseInfo(atoi(argv[2]));
                    }
                    else
                    {
                        printf("Expected argc 3, but got %d\n", argc);
                    }
                }
                else if (checkArgs(argv[1], 1, "teacher"))
                {
                    if (argc == 3)
                    {
                        showTeacherInfo(atoi(argv[2]));
                    }
                    else
                    {
                        printf("Expected argc 3, but got %d\n", argc);
                    }
                }
                else
                {
                    printf("Unknown subcommand %s\n", argv[1]);
                }
            }
            else if (checkArgs(argv[0], 1, "enroll"))
            {
                userEnroll();
            }
            else if (checkArgs(argv[0], 1, "register"))
            {
                userCourseRegister();
            }
            else
            {
                printf("Unknown command: %s\n", argv[0]);
                printHelp();
            }
        }
    }
    terminate(0);
}

int main(int argc, char *argv[MAX_ARGC])
{
    // init global vars

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