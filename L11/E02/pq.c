#include "pq.h"

struct pq {
	Item *heap;
	int heapDim;
};

int LEFT(int i);
int RIGHT(int i);
int PARENT(int i);
void swap(PQ pq, int id1, int id2);
void HEAPify(PQ pq, int i);

int LEFT(int i) {
	return 2 * i + 1;
}

int RIGHT(int i) {
	return 2 * i + 2;
}

int PARENT(int i) {
	return (i - 1) / 2;
}

void swap(PQ pq, int id1, int id2) {
	Item t;

	t = pq->heap[id1];
	pq->heap[id1] = pq->heap[id2];
	pq->heap[id2] = t;

	return;
}

void HEAPify(PQ pq, int i) {
	int l, r, largest;

	l = LEFT(i);
	r = RIGHT(i);
	if (l < pq->heapDim && ITEMcmp(pq->heap[i], pq->heap[l]) < 0) {
		largest = l;
	}
	else {
		largest = i;
	}
	if (r < pq->heapDim && ITEMcmp(pq->heap[largest], pq->heap[r]) < 0) {
		largest = r;
	}
	if (largest != i) {
		swap(pq, i, largest);
		HEAPify(pq, largest);
	}

	return;
}

PQ PQinit(int maxDim) {
	PQ pq;

	pq = malloc(sizeof(*pq));
	pq->heap = malloc(maxDim * sizeof(pq->heap));
	pq->heapDim = 0;

	return pq;
}

int PQempty(PQ pq) {
	return (pq->heapDim == 0);
}

Item PQget(PQ pq) {
	Item t;

	swap(pq, 0, pq->heapDim - 1);
	t = pq->heap[pq->heapDim - 1];
	pq->heapDim--;
	HEAPify(pq, 0);

	return t;
}

void PQinsert(PQ pq, Item val) {
	int i;

	i = pq->heapDim++;
	while (i >= 1 && ITEMcmp(pq->heap[PARENT(i)], val) < 0) {
		pq->heap[i] = pq->heap[PARENT(i)];
		i = PARENT(i);
	}
	pq->heap[i] = val;

	return;
}
