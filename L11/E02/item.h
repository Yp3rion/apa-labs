#ifndef ITEM_H_DEFINED
#define ITEM_H_DEFINED

#include<string.h>
#include<stdlib.h>
#include<stdio.h>

typedef struct item *Item;

void ITEMshow(Item);
Item ITEMnew(char *, int);
int ITEMcmp(Item val1, Item val2);
void ITEMfree(Item);

#endif
