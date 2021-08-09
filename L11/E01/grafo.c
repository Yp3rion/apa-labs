#include "grafo.h"

typedef struct nodo *link;

struct nodo {
	int v;
	int wt;
	link next;
};

struct grafo {
	int V;
	int *E;
	link *adjList;
	int **adjMat;
	ST tab;
	link z;
};

link newNode(int, int, link);
void insertE(G, t_edge);
int **MATinit(int, int, int);

link newNode(int v, int wt, link next) {
	link t;

	t = malloc(sizeof(*t));
	t->v = v;
	t->wt = wt;
	t->next = next;

	return t;
}

G GRAPHinit(int V) {
	G graph;
	int i;

	graph = malloc(sizeof(*graph));
	graph->V = V;
	graph->E = malloc(V * sizeof(*graph->E));
	for (i = 0; i < V; i++) {
		graph->E[i] = 0;
	}
	graph->adjMat = NULL;
	graph->z = NULL;
	graph->adjList = malloc(V * sizeof(*graph->adjList));
	for (i = 0; i < V; i++) {
		graph->adjList[i] = graph->z;
	}
	graph->tab = STinit(V);

	return graph;
}

void GRAPHread(FILE *input, G graph) {
	int i;
	char src[MAXLSTR], dst[MAXLSTR];
	int wt;
	int idSrc, idDst;

	for (i = 0; i < graph->V; i++) {
		STinsert(graph->tab, ITEMscan(input));
	}
	while (fscanf(input, "%s %s %d", src, dst, &wt) == 3) {
		idSrc = STsearch(graph->tab, src);
		idDst = STsearch(graph->tab, dst);
		insertE(graph, EDGEcreate(idSrc, idDst, wt));
	}

	return;
}

void insertE(G graph, t_edge edge) {
	int v, w, wt;

	v = edge.v;
	w = edge.w;
	wt = edge.wt;
	graph->adjList[v] = newNode(w, wt, graph->adjList[v]);
	graph->adjList[w] = newNode(v, wt, graph->adjList[w]);
	graph->E[v]++;
	graph->E[w]++;

	return;
}

void GRAPHadjmat(G graph) {
	int i, j;
	link x;
	
	graph->adjMat = MATinit(graph->V, graph->V, 0);
	for (i = 0; i < graph->V; i++) {
		for (x = graph->adjList[i]; x != graph->z; x = x->next) {
			graph->adjMat[i][x->v] = x->wt;
		}
	}

	for (i = 0; i < graph->V; i++) {
		for (j = 0; j < graph->V; j++) {
			fprintf(stdout, "%d ", graph->adjMat[i][j]);
		}
		fprintf(stdout, "%s", "\n");
	}


	return;
}

int **MATinit(int r, int c, int val) {
	int **t;
	int i, j;

	t = malloc(r * sizeof(*t));
	for (i = 0; i < r; i++) {
		t[i] = malloc(c * sizeof(*t[i]));
	}
	for (i = 0; i < r; i++) {
		for (j = 0; j < c; j++) {
			t[i][j] = val;
		}
	}

	return t;
}

void GRAPHshownodes(G graph) {
	int i;
	t_item val;

	for (i = 0; i < graph->V; i++) {
		val = STretrieve(graph->tab, i);
		fprintf(stdout, "%s\n", KEYget(&val));
	}

	return;
}

int GRAPHedgespernode(G graph, Key k) {
	int i;

	i = STsearch(graph->tab, k);

	return graph->E[i];
}

void GRAPHadjnodes(G graph, Key k) {
	int i;
	t_item val;
	link x;

	i = STsearch(graph->tab, k);
	for (x = graph->adjList[i]; x != graph->z; x = x->next) {
		val = STretrieve(graph->tab, x->v);
		fprintf(stdout, "%s\n", KEYget(&val));
	}

	return;
}

void GRAPHdestroy(G graph) {
	int i;
	link x, t;

	free(graph->E);
	STdestroy(graph->tab);
	if (graph->adjMat != NULL) {
		for (i = 0; i < graph->V; i++) {
			free(graph->adjMat[i]);
		}
		free(graph->adjMat);
	}
	for (i = 0; i < graph->V; i++) {
		x = graph->adjList[i];
		while (x != graph->z) {
			t = x;
			x = x->next;
			free(t);
		}
	}
	free(graph->adjList);

	return;
}