#ifndef LOGIN
#define LOGIN
#include "globals.h"
#include "limits.h"
#include <stdint.h>
#include "parser.h"
#include "md5.h"
#include "globals.h"
#include <stdio.h>
#include <string.h>
#include "lnklist.h"
#include "curriculum.h"
typedef struct UserInfo
{
    char username[MAX_LEN];
    uint32_t userid;
    uint8_t hash[MD5_LEN];
} UserInfo;

typedef enum UserType
{
    ANONYMOUS, // 0x00000000
    ROOT,      // 0x00000001
    STUDENT,   // 0x00000002-0x80000000
    TEACHER    // 0x80000001-0xFFFFFFFF
} UserType;

uint32_t uid2subid(uint32_t uid);
bool isAdmin(uint32_t userid);
bool isAnonymous(uint32_t userid);
bool isRoot(uint32_t userid);
bool isStudent(uint32_t userid);
bool isTeacher(uint32_t userid);

int initUserProfile();
int getUserInfo();
int login();
int loginHash(char *username, char *password, char hash[MD5_LEN]);
int createAdmin();
int createUser();
void fillUserInfo(UserInfo *user, char *name, uint32_t uid, uint8_t hash[MD5_LEN]);
int saveLoginInfo();
#endif