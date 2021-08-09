#include  "edge.h"

t_edge EDGEcreate(int v, int w, int wt) {
	t_edge edge;

	edge.v = v;
	edge.w = w;
	edge.wt = wt;

	return edge;
}