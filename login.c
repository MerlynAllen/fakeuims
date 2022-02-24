#include "login.h"
#include "parser.h"
#include "md5.h"
#include "globals.h"
#include <stdint.h>

UserInfo *userInit()
{
    head = calloc(1, sizeof(UserInfo));
    head->next = NULL;
    head->prev = NULL;
    USER_HEAD = head;
    USER_TAIL = head;
    USER_PTR = head;
    return head;
}

UserInfo *makeUser(char *username, uint32_t userid, char hash[MD5_LEN])
{
    UserInfo *user = calloc(1, sizeof(UserInfo));
    strcpy(user->username, username);
    user->userid = userid;
    memcpy(user->hash, hash, 16);
    user->next = NULL;
    user->prev = NULL;
    return user;
}

void appendUser(UserInfo *user)
{
    UserInfo *tmp = USER_PTR;
    USER_PTR = USER_TAIL;
    insUser(user, false);
}

void seekUser(UserInfo)
{
}

void insUser(UserInfo *user, bool reverse)
{
    if (!reverse)
    {
        user->next = USER_PTR->next;
        user->prev = USER_PTR;
        if (USER_PTR->next != NULL)
        {
            USER_PTR->next->prev = user;
        }
        USER_PTR->next = user;
    }
    else
    {
        user->prev = USER_PTR->prev;
        user->next = USER_PTR;
        if (USER_PTR->prev != NULL)
        {
            USER_PTR->prev->next = user;
        }
        USER_PTR->prev = user;
    }
}

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

getUserInfo()
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
int getPasswdInfo()
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
}

int login()
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

int loginHash(char *username, char *password, char *hash)
{
    int un_len, pw_len = 0;
    un_len = strlen(username);
    pw_len = strlen(password);
    strcpy(buf, username);
    strcpy(buf + un_len, ":");
    strcpy(buf + un_len + 1, password);
    uint8_t checksum[MD5_LEN] = {0};
    md5(buf, strlen(buf), checksum);
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
        for (i = 0; i < strlen(username), ALNUM)
            ; i++)
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
        for (i = 0; i < strlen(password), ALNUM)
            ; i++)
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
        else if strcmp (type, "T")
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

int createLogin(char *username, char *password, uint32_t uid)
{
    FILE *fp_usr = fopen("users", "a");
    FILE *fp_pwd = fopen("passwd", "ab");
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
        fwrite();
    }
    fprintf(fp_usr, "%s:%d\n", username, uid);
    fclose(fp_usr);

    char buf[MAX_LEN] = {0};
    strcpy(buf, username);
    strcpy(buf + strlen(username), ":");
    strcpy(buf + strlen(username) + 1, password);
    uint8_t checksum[MD5_LEN] = {0};
    md5(buf, strlen(buf), checksum);
    fwrite(&uid, sizeof(uint32_t), 1, fp_pwd);
    fwrite(checksum, sizeof(uint8_t), MD5_LEN, fp_pwd);
    freopen("passwd", "rb+", fp_pwd);
    USERCOUNT++;
    fwrite(&USERCOUNT, sizeof(uint32_t), 1, fp_pwd);
    fclose(fp_pwd);
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
        fwrite();
    }
    fclose(fp_usr);
    fclose(fp_pwd);
}