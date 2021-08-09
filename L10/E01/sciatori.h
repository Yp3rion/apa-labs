#ifndef _SCIATORIH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "accessi.h"

#define _SCIATORIH

void initBSTsciatori(void);
int authorize(int cardId, char *skiliftId, int time);
void distruggiBSTsciatori(void);

#endif