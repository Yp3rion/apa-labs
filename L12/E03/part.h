#ifndef _PART

#define _PART

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

typedef struct prob *PROB;

PROB PROBinit(int N, int K, int M);
void PROBread(FILE *input, PROB prob);
void PROBsolve(PROB prob);
void PROBterminate(PROB prob);

#endif