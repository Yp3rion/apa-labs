#ifndef _ITEMH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _ITEMH
// Ipotizzo che i nomi dei partecipanti siano stringhe
// prive di spazi con lunghezza massima 255 caratteri.
#define MAXNOMPART 255 + 1

typedef char *KEYN;
typedef float *KEYP;

typedef struct item *ITEM;

KEYN KEYNget(ITEM val);
int KEYNcompare(KEYN nome1, KEYN nome2);
KEYP KEYPget(ITEM val);
int KEYPless(KEYP punti1, KEYP punti2);

int ITEMscan(FILE *input, ITEM *pVal);
void ITEMfree(ITEM val);
ITEM ITEMsetVoid(void);
int ITEMcheckVoid(ITEM val);
int ITEMlessP(ITEM val1, ITEM val2);
void ITEMprint(FILE *output, ITEM val);

#endif