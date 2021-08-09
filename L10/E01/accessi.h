#ifndef _ACCESSIH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skilift.h"

typedef struct lista *LIST;

#define _ACCESSIH

LIST initListAccessi(void);
int insOaggAccesso(LIST listAccessi, char skiliftId[], int ultimoAcc);
void distruggiListAccessi(LIST listAccessi);

#endif