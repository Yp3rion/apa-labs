#include "graph.h"

typedef struct GRAPHnode *link;

struct GRAPHnode {
	int dest;
	link next;
};

// Struct che di un vertice conserva il valore Vs, il valore Ms e la lista dei vertici adiacenti
typedef struct vertex {
	int HPs;
	int Ms;
	link head;
} t_vertex;

struct graph {
	t_vertex *adj; // Vettore che contiene tutte le informazioni su qualsiasi vertice
	int V;
	int E;
	TS ts;
};

link NEW(int dest, link next);
void insertE(GRAPH g, t_edge e);
void GRAPHbestPathR(GRAPH g, int pos, int vert, int HP, int M, int *bM, int *bHP, int *bPos, t_edge *path, t_edge *bPath, int *mark);
void GRAPHshowPath(FILE *output, GRAPH g, t_edge *path, int lPath);

link NEW(int dest, link next) {
	link x;

	x = malloc(sizeof(*x));
	x->dest = dest;
	x->next = next;

	return x;
}

void insertE(GRAPH g, t_edge e) {
	g->adj[e.v].head = NEW(e.w, g->adj[e.v].head);
	g->adj[e.w].head = NEW(e.v, g->adj[e.w].head);

	return;
}

GRAPH GRAPHinit(int V) {
	GRAPH g;
	int cV;

	g = malloc(sizeof(*g));
	g->V = V;
	g->E = 0;
	g->adj = malloc(V * sizeof(*g->adj));
	for (cV = 0; cV < g->V; cV++) {
		g->adj[cV].head = NULL;
	}
	g->ts = TSinit(g->V);

	return g;
}

void GRAPHread(FILE *input, GRAPH g) {
	char src[MAXNOMSALE], dest[MAXNOMSALE];
	int HPs, Ms, id1, id2;
	int cV;

	for (cV = 0; cV < g->V; cV++) {
		fscanf(input, "%s %d %d", src, &Ms, &HPs);
		id1 = TScercaOagg(g->ts, src);
		g->adj[id1].HPs = HPs;
		g->adj[id1].Ms = Ms;
	}
	while (fscanf(input, "%s %s", src, dest) == 2) {
		id1 = TScercaOagg(g->ts, src);
		id2 = TScercaOagg(g->ts, dest);
		insertE(g, EDGEcreate(id1, id2));
		g->E++;
	}

	return;
}

void GRAPHbestPath(FILE *output, GRAPH g, int pvIniz) {
	t_edge *path, *bPath;
	int *mark;
	int bM, bHP, bPos;

	bM = -1;
	path = malloc(g->E * sizeof(*path));
	bPath = malloc(g->E * sizeof(*path));
	mark = calloc(g->V, sizeof(*mark));

	GRAPHbestPathR(g, 0, 0, pvIniz, 0, &bM, &bHP, &bPos, path, bPath, mark);

	if (bM == -1) {
		fprintf(output, "%s", "Nessuna soluzione valida trovata.\n");
	}
	else {
		fprintf(output, "L'avventuriero completa la missione con %d punti vita portando con se' %d monete.\n", bHP, bM);
		fprintf(output, "%s", "Il suo percorso e' stato il seguente:\n");
		GRAPHshowPath(output, g, bPath, bPos);
	}

	free(path);
	free(bPath);
	free(mark);

	return;
}

void GRAPHbestPathR(GRAPH g, int pos, int vert, int HP, int M, int *bM, int *bHP, int *bPos, t_edge *path, t_edge *bPath, int *mark) {
	int cE;
	link x;

	if (vert == g->V - 1) {
		if (M > *bM) {
			*bM = M;
			*bHP = HP;
			*bPos = pos;
			for (cE = 0; cE < pos; cE++) {
				bPath[cE] = path[cE];
			}
		}
	}

	mark[vert] = 1;
	for (x = g->adj[vert].head; x != NULL; x = x->next) {
		if (mark[x->dest] == 0 && HP - g->adj[x->dest].HPs > 0) {
			path[pos] = EDGEcreate(vert, x->dest);
			GRAPHbestPathR(g, pos + 1, x->dest, HP - g->adj[x->dest].HPs, M + g->adj[x->dest].Ms, bM, bHP, bPos, path, bPath, mark);
		}
	}
	mark[vert] = 0;

	return;
}

void GRAPHshowPath(FILE *output, GRAPH g, t_edge *path, int lPath) {
	int cE;

	for (cE = 0; cE < lPath; cE++) {
		fprintf(output, "%s -> %s\n", TSretrieve(g->ts, path[cE].v), TSretrieve(g->ts, path[cE].w));
	}

	return;
}

void GRAPHdestroy(GRAPH g) {
	TSdestroy(g->ts);
	free(g->adj);
	free(g);
	
	return;
}