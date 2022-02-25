#include "login.h"
#include "parser.h"
#include "md5.h"
#include "globals.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "lnklist.h"

bool isTeacher(userid)
{
    return (userid & 0x80000000) >> 31;
}
bool isStudent(userid)
{
    return !isTeacher(userid);
}
bool isAdmin(userid)
{
    return (userid & 0x40000000) >> 30;
}
bool isRoot(userid)
{
    return userid == 1;
}
bool isAnonymous(userid)
{
    return userid == 0;
}

int touchUserPasswd()
{
    FILE *fp = fopen("user", "w");
    if (fp == NULL)
    {
        printf("Cannot create user profile.\n");
        return -1;
    }
    fclose(fp);
    FILE *fp = fopen("passwd", "w");
    if (fp == NULL)
    {
        printf("Cannot create passwd profile.\n");
        return -1;
    }
    fwrite(&(uint32_t){0}, sizeof(uint32_t), 1, fp);
    fclose(fp);
}

int getUserInfo()
{
    FILE *fp_usr = fopen("users", "r");
    if (fp_usr == NULL)
    {
        printf("Failed to open users file.\n");
        return -1;
    }
    char username[MAX_LEN] = {0};
    char userID[MAX_LEN] = {0};
    fscanf(fp_usr, "%s:%s", username, userID);
}
int getPasswdInfo() // get password info form file
{
    FILE *fp_pwd = fopen("passwd", "r");
    if (fp_pwd == NULL)
    {
        printf("Failed to open passwd file.\n");
        return -1;
    }
    char username[MAX_LEN] = {0};
    char passwd[MAX_LEN] = {0};
    fscanf(fp_pwd, "%s:%s", username, passwd);
    //
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

int checkLogin(char *username, char *password)
{
    FILE *passwd;
    FILE *users;
    fopen("passwd", "r");
    fopen("users", "r");
    char buf[MAX_LEN] = {0};
    loginHash(username, password, buf);
    //
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
        if (strcmp(type, "S"))
        {
            is_student = true;
            printf("Student ID: ");
        }
        else if (strcmp(type, "T"))
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

int createLogin(char *username, char *password, uint32_t uid) // avability passed, add user to linked list (RAM bot not file)
{
    char md5sum[MD5_LEN] = {0};
    loginHash(username, password, md5sum);

    // US
    // fwrite(&uid, sizeof(uint32_t), 1, fp_pwd);
    // fwrite(checksum, sizeof(uint8_t), MD5_LEN, fp_pwd);
    // freopen("passwd", "rb+", fp_pwd);
    // USERCOUNT++;
    // fwrite(&USERCOUNT, sizeof(uint32_t), 1, fp_pwd);
    // fclose(fp_pwd);
}

int saveLoginInfo() // save to a new file
{
    FILE *fp_usr = fopen("users", "wb");
    FILE *fp_pwd = fopen("passwd", "wb");
    if (fp_usr == NULL)
    {
        printf("Failed to open users  file.\n");
        return -1;
    }
    if (fp_pwd == NULL)
    {
        printf("Password file missing, try creating new\n");
        fp_pwd = fopen("passwd", "wb");
        if (fp_pwd == NULL)
        {
            printf("Failed to create password file.\n");
            return -1;
        }
        //
        // saving current user info;
        // fwrite();
    }
    fclose(fp_usr);
    fclose(fp_pwd);
}