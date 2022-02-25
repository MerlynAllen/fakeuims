#ifndef LNKLIST
#define LNKLIST
#include "stdint.h"

typedef struct DNode
{
    void *data;
    DNode *next;
    DNode *prev;
} DNode;

typedef struct DLnklist
{
    DNode *head;
    DNode *tail;
    DNode *ptr;
    uint32_t count;
} DLnklist;

DLnklist *dnodeInit(void (*makeData)(void **));
int appendNode(DLnklist *ll, void (*makeData)(void **));
int insNode(DLnklist *ll, void (*makeData)(void **)); // insert after ptr
int insHeadNode(DLnklist *ll, void (*makeData)(void **));
int delNode(DLnklist *ll);
int seekNode(DLnklist *ll, bool (*criterion)(void *))
#endif