/*
Dual Linked List
*/
#include "lnklist.h"

DLnklist *makeDLinkedList()
{
    DLnklist *dlist = calloc(1, sizeof(DLnklist));
    dlist->head = NULL;
    dlist->tail = NULL;
    dlist->ptr = NULL;
    dlist->count = 0;
    return dlist;
}

int initDLinkedList(DLnklist *dlist)
{
    dlist->head = NULL;
    dlist->tail = NULL;
    dlist->ptr = NULL;
    dlist->count = 0;
    return 0;
}

int appendNode(DLnklist *ll, void *data)
{
    if (ll->count != 0)
    {
        DNode *tmp = ll->ptr;
        ll->ptr = ll->tail;
        insNode(ll, data);
        ll->ptr = tmp;
        return ll->count;
    }
    else
    {
        insNode(ll, data);
        return ll->count;
    }
}

int insNode(DLnklist *ll, void *data) // insert after ptr
{
    DNode *node = calloc(1, sizeof(DNode));
    node->data = data; // for other functions to fill in
    if (ll->count == 0)
    {
        node->next = NULL;
        node->prev = NULL;
        ll->head = node;
        ll->tail = node;
        ll->ptr = node;
        ll->count++;
        return ll->count;
    }
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

int moveNext(DLnklist *ll)
{
    if (ll->ptr->next != NULL)
    {
        ll->ptr = ll->ptr->next;
        return 0;
    }
    return -1;
}

int seekNode(DLnklist *ll, bool (*criterion)(void *, void *), void *data)
{
    if (ll->ptr == NULL)
    {
        return 1; // error of empty list
    }
    ll->ptr = ll->head;
    for (; !criterion(ll->ptr->data, data); moveNext(ll))
    {
        if (ll->ptr == ll->tail) // end of list
        {
            return -1; // not found
        }
    }
    return 0; // found
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
    // free(ll);
    return 0;
}

int nodeqsort(DLnklist *ll, bool (*criterion)(void *, void *), bool reverse)
{
    if (ll == NULL || ll->count == 0)
    {
        return -1;
    }
    qsortcore(ll->head, ll->tail, criterion, reverse);
    return 0;
}
int swapNode(DNode *a, DNode *b)
{
    if (a != b)
    {

        void *tmp = a->data;
        a->data = b->data;
        b->data = tmp;
    }
    return 0;
}
int qsortcore(DNode *head, DNode *tail, bool (*criterion)(void *, void *), bool reverse)
{
    if (head == tail)
    {
        return 0;
    }
    DNode *left = head;
    DNode *right = tail;
    while (left != right)
    {
        if (reverse ^ criterion(left->data, left->next->data))
        {
            swapNode(left, left->next);
            left = left->next;
        }
        else
        {
            swapNode(left->next, right);
            right = right->prev;
        }
    }
    qsortcore(head, left, criterion, reverse);
    if (left != tail)
    {
        qsortcore(left->next, tail, criterion, reverse);
    }
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
