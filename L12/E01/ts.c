#include "ts.h"

struct ts {
	char **v; // Uso come tabella di simboli un semplice vettore di stringhe sfruttando corrispondenza indice/dato
	int N;
};

TS TSinit(int N) {
	TS ts;

	ts = malloc(sizeof(*ts));
	ts->N = N;
	ts->v = calloc(N, sizeof(*ts->v)); // Puntatori a carattere inizializzati a NULL

	return ts;
}

int TScercaOagg(TS ts, char *str) {
	int cStr;
	
	// Cerca:
	for (cStr = 0; cStr < ts->N && ts->v[cStr] != NULL; cStr++) {
		if (strcmp(ts->v[cStr], str) == 0) {
			return cStr;
		}
	}

	// Aggiunge:
	ts->v[cStr] = malloc((strlen(str) + 1) * sizeof(*ts->v[cStr]));
	strcpy(ts->v[cStr], str);

	return cStr;
}

char *TSretrieve(TS ts, int id) {
	return ts->v[id];
}

void TSdestroy(TS ts) {
	int cStr;

	for (cStr = 0; cStr < ts->N; cStr++) {
		if (ts->v[cStr] != NULL) {
			free(ts->v[cStr]);
		}
	}
	free(ts->v);
	free(ts);

	return;
}