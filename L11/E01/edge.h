#ifndef _EDGEH

#define _EDGEH

typedef struct {
	int v;
	int w;
	int wt;
} t_edge;

t_edge EDGEcreate(int, int, int);

#endif