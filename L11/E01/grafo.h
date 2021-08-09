#ifndef _GRAFOH

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "item.h"
#include "edge.h"

#define _GRAFOH

typedef struct grafo *G;

G GRAPHinit(int);
void GRAPHread(FILE *, G);
void GRAPHadjmat(G);
void GRAPHshownodes(G);
int GRAPHedgespernode(G, Key);
void GRAPHadjnodes(G, Key);
void GRAPHdestroy(G);

#endif