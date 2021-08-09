#ifndef _PQH

#include <stdio.h>
#include <stdlib.h>
#include "item.h"

#define _PQH

int PQinit(void);
int PQempty(void);
int PQinsert(ITEM val);
ITEM PQextractMin(void);
ITEM PQextractByN(KEYN nomCercato);
void PQprint(FILE *output);
void PQdestroy(void);

#endif