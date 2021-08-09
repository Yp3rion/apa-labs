#ifndef _ITEMH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _ITEMH
#define MAXLSTR 30 + 1

typedef char *Key;

typedef struct item {
	char idElab[MAXLSTR];
	char idRete[MAXLSTR];
} t_item;

Key KEYget(t_item *);
int KEYcompare(Key, Key);
t_item ITEMscan(FILE *);
t_item ITEMsetvoid();
int ITEMcheckvoid(t_item);

#endif