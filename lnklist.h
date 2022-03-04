#ifndef LNKLIST
#define LNKLIST
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct DNode // linked list node
{
    void *data;
    struct DNode *next;
    struct DNode *prev;
} DNode;

typedef struct DLnklist // save information of a linked list
{
    DNode *head;
    DNode *tail;
    DNode *ptr;
    uint32_t count;
} DLnklist;

DLnklist *makeDLinkedList();
int initDLinkedList(DLnklist *dlist);

int appendNode(DLnklist *ll, void *data);
int insNode(DLnklist *ll, void *data); // insert after ptr
int insHeadNode(DLnklist *ll, void *data);
int moveNext(DLnklist *ll);
int delNode(DLnklist *ll);
int seekNode(DLnklist *ll, bool (*criterion)(void *, void *), void *data);
int nodeqsort(DLnklist *ll, bool (*criterion)(void *, void *), bool reverse);
int qsortcore(DNode *head, DNode *tail, bool (*criterion)(void *, void *), bool reverse);
int clearList(DLnklist *ll);
#endif