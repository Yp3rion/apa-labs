#ifndef _HASHH

#include <stdio.h>
#include <stdlib.h>
#include "item.h"

#define _HASHH

typedef struct symboltable *ST;

ST STinit(int);
void STinsert(ST, t_item);
int STsearch(ST, Key);
t_item STretrieve(ST st, int i);
void STdestroy(ST);

#endif