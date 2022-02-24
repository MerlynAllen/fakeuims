#ifndef GLOBALS
#define GLOBALS
#include "login.h"

char USERNAME[MAX_LEN] = {0};
uint32_t USERID = 0;
extern uint8_t _binary_HELP_end;
extern uint8_t _binary_HELP_start;

uint32_t USERCOUNT = 0;
UserInfo* USER_HEAD = NULL;
UserInfo* USER_TAIL = NULL;
UserInfo* USER_PTR = NULL;

#endif