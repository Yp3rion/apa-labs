#include "hash.h"

struct symboltable {
	t_item *vett;
	int M;
};

int hash(Key, int);

ST STinit(int dimMax) {
	ST st;
	int i;

	st = malloc(sizeof(*st));
	st->M = dimMax;
	st->vett = malloc(st->M * sizeof(*st->vett));
	for (i = 0; i < st->M; i++) {
		st->vett[i] = ITEMsetvoid();
	}

	return st;
}

int hash(Key k, int M) {
	int h, base;

	for (h = 0, base = 127; *k != '\0'; k++) {
		h = (base * h + *k) % M;
	}

	return h;
}

void STinsert(ST st, t_item val) {
	int i;

	i = hash(KEYget(&val), st->M);
	while (!ITEMcheckvoid(st->vett[i])) {
		i = (i + 1) % st->M;
	}
	st->vett[i] = val;

	return;
}

int STsearch(ST st, Key k) {
	int i;

	i = hash(k, st->M);
	while (!ITEMcheckvoid(st->vett[i])) {
		if (KEYcompare(KEYget(&st->vett[i]), k) == 0) {
			return i;
		}
		i = (i + 1) % st->M;
	}

	return -1;
}

t_item STretrieve(ST st, int i) {
	return st->vett[i];
}

void STdestroy(ST st) {
	free(st->vett);
	free(st);

	return;
}