#ifndef GLOBALS
#define GLOBALS
#include "login.h"
#include "lnklist.h"
#include "limits.h"
#include <stdint.h>

#define NULL 0;
char USERNAME[MAX_LEN] = {0};
uint32_t USERID = 0;
extern uint8_t _binary_HELP_end;
extern uint8_t _binary_HELP_start;

DLnklist *USERLIST = NULL;

#endif