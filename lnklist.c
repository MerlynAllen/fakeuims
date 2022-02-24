/*
Dual Linked List
*/
#include "lnklist.h"

void dnodeInit()
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
