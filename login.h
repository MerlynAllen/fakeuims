#ifndef LOGIN
#define LOGIN
#include "globals.h"
typedef struct UserInfo
{
    char username[MAX_LEN];
    uint32_t userid;
    uint8_t hash[MD5_LEN];
    UserInfo *next;
    UserInfo *prev;
} UserInfo;

typedef enum UserType
{
    ANONYMOUS, // 0x00000000
    ROOT,      // 0x00000001
    STUDENT,   // 0x00000002-0x80000000
    TEACHER    // 0x80000001-0xFFFFFFFF
} UserType;

int login();
int checkLogin(char *username, char *password);

int createLogin();
int createUser(char *username, char *password);
#endif