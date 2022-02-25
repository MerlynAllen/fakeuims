#include "login.h"
#include "parser.h"
#include "md5.h"
#include "globals.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "lnklist.h"

bool isTeacher(uint32_t userid)
{
    return (userid & 0x80000000) >> 31;
}
bool isStudent(uint32_t userid)
{
    return !isTeacher(userid);
}
bool isRoot(uint32_t userid)
{
    return userid == 1;
}
bool isAdmin(uint32_t userid)
{
    return ((userid & 0x40000000) >> 30) || isRoot(userid);
}
bool isAnonymous(uint32_t userid)
{
    return userid == 0;
}

int initUserProfile() // no login info, init
{
    while (1)
    {
        printf("Creating user profile...\n");
        UserInfo *root = calloc(1, sizeof(UserInfo));
        root->userid = 1;
        strcpy(root->username, "root");
        char root_passwd[MAX_LEN] = {0};
        char root_passwd_confirm[MAX_LEN] = {0};
        printf("Please input root password: ");
        readline(root_passwd, MAX_LEN);
        printf("Please confirm root password again: ");
        readline(root_passwd_confirm, MAX_LEN);
        if (strcmp(root_passwd, root_passwd_confirm) != 0)
        {
            printf("Password not match.\n");
            free(root);
            continue;
        }
        uint8_t md5sum[MD5_LEN] = {0};
        loginHash(root->username, root_passwd, md5sum);
        memcpy(root->hash, md5sum, MD5_LEN);

        UserInfo *anonymous = calloc(1, sizeof(UserInfo));
        strcpy(anonymous->username, "anonymous");
        USERLIST = dnodeInit(anonymous);

        appendNode(USERLIST, root);
        break;

        // FILE *fp = fopen("passwd", "w");
        // if (fp == NULL)
        // {
        //     printf("Cannot create passwd profile. Filesystem error.\n");
        //     exit(1);
        // }
        // fwrite(&(uint32_t){2}, sizeof(uint32_t), 1, fp);
        // fclose(fp);
    }
}

int getUserInfo()
{
    FILE *fp_pwd = fopen("passwd", "r");
    if (fp_pwd == NULL)
    {
        printf("Failed to open passwd file. File might be missing.\n");
        return -1;
    }
    // initialize a new linked list

    uint32_t user_count = 0;
    fread(&user_count, sizeof(uint32_t), 1, fp_pwd);

    for (int i = 0; i < user_count; ++i)
    {
        UserInfo *data = calloc(1, sizeof(UserInfo));
        if (fread(data, sizeof(UserInfo), 1, fp_pwd) != 1)
        {
            printf("Failed to read user info. File might be corrupted.\n");
            return -1;
        }
        if (USERLIST == NULL)
        {
            USERLIST = dnodeInit(data);
        }
        else
        {
            appendNode(USERLIST, data);
        }
    }
    return 0;
}

int login() // main login function
{
    int fail_count = 0;
    printf("Username: ");
    char username[MAX_LEN] = {0};
    char password[MAX_LEN] = {0};
    int i;
    for (i = 0; i < MAX_LEN && isChars(username[i] = getchar(), ALNUM); i++)
        ;
    username[i] = '\0';
    printf("Password: ");
    for (i = 0; i < MAX_LEN && isChars(password[i] = getchar(), ALNUM); i++)
        ;
    password[i] = '\0';
    checkLogin(username, password);
}

int loginHash(char *username, char *password, char hash[MD5_LEN]) // generates md5 from username and password
{
    int un_len, pw_len = 0;
    un_len = strlen(username);
    pw_len = strlen(password);
    uint8_t buf[MD5_LEN] = {0};
    strcpy(buf, username);
    strcpy(buf + un_len, ":");
    strcpy(buf + un_len + 1, password);
    md5(buf, strlen(buf), hash);
    // for (int i = 0; i < 16; i++)
    // {
    //     printf("%02x ", checksum[i]);
    // }
    // printf("\n");
}

bool usernameMatch(void *data, void *username)
{
    if (strcmp((char *)username, ((UserInfo *)data)->username) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int checkLogin(char *username, char *password)
{
    FILE *passwd;
    fopen("passwd", "r");
    char buf[MAX_LEN] = {0};
    loginHash(username, password, buf);
    seekNode(USERLIST, usernameMatch, username);
    UserInfo *sel_user = (UserInfo *)USERLIST->ptr->data;
    if (memcmp(buf, sel_user->hash, MD5_LEN) == 0)
    {
        printf("Login success!\n");
        USERID = sel_user->userid;
        strcpy(USERNAME, sel_user->username);
        return 0;
    }
    else
    {
        printf("Login failed!\n");
        return -1;
    }
    //
}
void fillUserInfo(UserInfo *user, char *name, uint32_t uid, uint8_t hash[MD5_LEN])
{
    strcpy(user->username, name);
    user->userid = uid;
    memcpy(user->hash, hash, MD5_LEN);
}
int createLogin(char *username, char *password, uint32_t uid) // avability check passed, add user to linked list (RAM bot not file)
{
    char md5sum[MD5_LEN] = {0};
    loginHash(username, password, md5sum);
    UserInfo *data = calloc(1, sizeof(UserInfo)); // empty
    fillUserInfo(data, username, uid, md5sum);
    appendNode(USERLIST, data);
    return 0;
}

bool checkDuplicate(char *username)
{
    if (seekNode(USERLIST, usernameMatch, username) == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int createAccount(bool admin)
{
    while (true)
    {
        bool is_student;
        uint32_t sub_id = 0;
        uint32_t uid = 0;
        char username[MAX_LEN] = {0};
        char password[MAX_LEN] = {0};
        char password_confirm[MAX_LEN] = {0};
        printf("Username: ");
        int i;
        readline(username, MAX_LEN);
        if (strlen(username) < 5 && strlen(username) > 20)
        {
            printf("Username must be between 5 and 20 characters.\n");
            goto retry;
        }
        for (i = 0; i < strlen(username); i++)
        {
            if (!isChars(username[i], ALNUM))
            {
                printf("Username must be alphanumeric.\n");
                printf("Valid username characters regex should be [A-Za-z0-9_]{5,20}\n");
                goto retry;
            }
        }
        if (checkDuplicate(username))
        {
            printf("Username already exists.\n");
            return 0;
        }
        // Username passed
        printf("Password: ");
        readline(password, MAX_LEN);
        printf("Password Again: ");
        readline(password_confirm, MAX_LEN);
        if (strcmp(password, password_confirm) != 0)
        {
            printf("Passwords do not match.\n");
            goto retry;
        }
        if (strlen(password) < 5 && strlen(password) > 20)
        {
            printf("Password must be between 5 and 20 characters.\n");
            goto retry;
        }
        for (i = 0; i < strlen(password); i++)
        {
            if (!isChars(password[i], ALNUM))
            {
                printf("Password must be alphanumeric.\n");
                printf("Valid password characters regex should be [A-Za-z0-9_]{5,20}\n");
                goto retry;
            }
        }
        // Password passed
        printf("Account type Teacher/Student [T/S]: ");
        char type[MAX_LEN] = {0};
        readline(type, MAX_LEN);
        if (strcmp(type, "S") == 0)
        {
            is_student = true;
            printf("Student ID: ");
        }
        else if (strcmp(type, "T") == 0)
        {
            is_student = false;
            printf("Teacher ID: ");
        }
        else
        {
            printf("Invalid account type.\n");
            goto retry;
        }
        scanf("%d", &sub_id);
        while (getchar() != '\n') // Clear stdin
            ;
        printf("Creating user...\n");
        uid = (sub_id & 0x0FFFFFFF) | (is_student << 31) | (admin << 30);
        createLogin(username, password, uid);
        printf("User created!\n");
        return 0;
    // retry
    retry:
        printf("Retry? [Y/N]\n");
        char retry[MAX_LEN] = {0};
        readline(retry, MAX_LEN);
        if (strcmp(retry, "Y") == 0)
        {
            continue;
        }
        else
        {
            break;
        }
    }
}
int createAdmin()
{
    if (isAdmin(USERID))
    {

        createAccount(true);
    }
    else
    {
        printf("You are not an admin.\n");
    }
}

int createUser()
{
    createAccount(false);
}

int saveLoginInfo() // save to a new file
{
    FILE *fp_pwd = fopen("passwd", "wb");
    if (fp_pwd == NULL)
    {
        printf("Failed to create password file.\n");
        return -1;
    }

    // saving current user info;
    DNode *curr = USERLIST->ptr;
    fwrite(&USERLIST->count, sizeof(uint32_t), 1, fp_pwd);
    USERLIST->ptr = USERLIST->head;
    for (; USERLIST->ptr != NULL; USERLIST->ptr = USERLIST->ptr->next)
    {
        UserInfo *data = (UserInfo *)USERLIST->ptr->data;
        fwrite(USERLIST->ptr->data, sizeof(UserInfo), 1, fp_pwd);
    }
    // fwrite();

    USERLIST->ptr = curr;
    fclose(fp_pwd);
}