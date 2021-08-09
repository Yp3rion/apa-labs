#ifndef _EDGE

#define _EDGE

typedef struct edge {
	int v;
	int w;
} t_edge;

t_edge EDGEcreate(int v, int w);

#endif