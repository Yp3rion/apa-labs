#include "edge.h"

t_edge EDGEcreate(int v, int w) {
	t_edge e;

	e.v = v;
	e.w = w;

	return e;
}