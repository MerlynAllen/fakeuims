#ifndef GLOBALS
#define GLOBALS
#include "login.h"
#include "lnklist.h"
#include "limits.h"
#include <stdint.h>
#include <stdio.h>

char USERNAME[MAX_LEN];
uint32_t USERID;
extern uint8_t _binary_HELP_end;
extern uint8_t _binary_HELP_start;

DLnklist *USERLIST;

DLnklist *TEACHERS;
DLnklist *STUDENTS;
DLnklist *ENROLLMENTS;
DLnklist *COURSES;
DLnklist *ROOMS;
DLnklist *REGISTRYS;

#endif