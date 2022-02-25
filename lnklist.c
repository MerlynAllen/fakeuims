/*
Dual Linked List
*/
#include "lnklist.h"

DLnklist *dnodeInit(void *data)
{
    DNode *head = calloc(1, sizeof(DNode));
    head->data = data;
    head->next = NULL;
    head->prev = NULL;
    DLnklist *dlist = calloc(1, sizeof(DLnklist));
    dlist->head = head;
    dlist->tail = head;
    dlist->ptr = head;
    dlist->count = 1;
    return dlist;
}

int appendNode(DLnklist *ll, void *data)
{
    DNode *tmp = ll->ptr;
    ll->ptr = ll->tail;
    insNode(ll, data);
    ll->ptr = tmp;
    return ll->count;
}

int insNode(DLnklist *ll, void *data) // insert after ptr
{
    DNode *node = calloc(1, sizeof(DNode));
    node->data = data; // for other functions to fill in
    node->next = ll->ptr->next;
    node->prev = ll->ptr;
    if (ll->ptr != ll->tail)
    {
        ll->ptr->next->prev = node;
    }
    else // nullptr -> tail
    {
        ll->tail = node;
    }
    ll->ptr->next = node;
    ll->ptr = node;
    ll->count++;
    return ll->count;
}
int insHeadNode(DLnklist *ll, void *data)
{

    DNode *node = calloc(1, sizeof(DNode));
    node->data = data;
    node->prev = NULL;
    node->next = ll->head;
    ll->head->prev = node;
    ll->head = node;
    ll->count++;
    return ll->count;
}

int delNode(DLnklist *ll)
{
    DNode *next = ll->ptr->next;
    ll->ptr->prev->next = ll->ptr->next;
    ll->ptr->next->prev = ll->ptr->prev;
    if (ll->ptr == ll->tail)
    {
        ll->tail = ll->ptr->prev;
    }
    if (ll->ptr == ll->head)
    {
        ll->head = ll->ptr->next;
    }
    free(ll->ptr->data);
    free(ll->ptr);
    ll->ptr = next;
}

int seekNode(DLnklist *ll, bool (*criterion)(void *, void *), void *data)
{
    ll->ptr = ll->head;
    for (; !criterion(ll->ptr->data, data); ll->ptr = ll->ptr->next)
    {
        if (ll->ptr == ll->tail) // end of list
        {
            return -1;
        }
    }
    return 0;
}

int clearList(DLnklist *ll)
{
    DNode *tmp = ll->head;
    while (tmp != NULL)
    {
        DNode *next = tmp->next;
        free(tmp->data);
        free(tmp);
        tmp = next;
    }
    ll->head = NULL;
    ll->tail = NULL;
    ll->ptr = NULL;
    ll->count = 0;
    free(ll);
    return 0;
}

// UserInfo *makeUser(char *username, uint32_t userid, char hash[MD5_LEN])
// {
//     UserInfo *user = calloc(1, sizeof(UserInfo));
//     strcpy(user->username, username);
//     user->userid = userid;
//     memcpy(user->hash, hash, 16);
//     user->next = NULL;
//     user->prev = NULL;
//     return user;
// }

// void appendUser(UserInfo *user)
// {
//     UserInfo *tmp = USER_PTR;
//     USER_PTR = USER_TAIL;
//     insUser(user, false);
// }

// void seekUser(UserInfo)
// {
// }

// void insUser(UserInfo *user, bool reverse)
// {
//     if (!reverse)
//     {
//         user->next = USER_PTR->next;
//         user->prev = USER_PTR;
//         if (USER_PTR->next != NULL)
//         {
//             USER_PTR->next->prev = user;
//         }
//         USER_PTR->next = user;
//     }
//     else
//     {
//         user->prev = USER_PTR->prev;
//         user->next = USER_PTR;
//         if (USER_PTR->prev != NULL)
//         {
//             USER_PTR->prev->next = user;
//         }
//         USER_PTR->prev = user;
//     }
// }
// }
