#ifndef _TS

#define _TS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ts *TS;

TS TSinit(int N);
int TScercaOagg(TS ts, char *str);
char *TSretrieve(TS ts, int id);
void TSdestroy(TS ts);

#endif