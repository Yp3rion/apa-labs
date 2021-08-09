#ifndef _GRAPH

#define _GRAPH

#include <stdio.h>
#include <stdlib.h>
#include "ts.h"
#include "edge.h"

#define MAXNOMSALE 255 + 1 // Ipotizzo che il nome delle sale sia al massimo 255 caratteri privi di spazi

typedef struct graph *GRAPH;

GRAPH GRAPHinit(int V);
void GRAPHread(FILE *input, GRAPH g);
void GRAPHbestPath(FILE *output, GRAPH g, int pvIniz);
void GRAPHdestroy(GRAPH g);

#endif