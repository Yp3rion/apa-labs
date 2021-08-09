#include "PQ.h"
#include "item.h"

typedef struct nodo *link;

typedef struct nodo {
	ITEM val;
	link next;
} t_nodo;

static link head = NULL;
static link z = NULL;

link nuovoNodo(ITEM val, link next);
void freeNode(link t);

link nuovoNodo(ITEM val, link next) {
	link h;

	if ((h = malloc(sizeof(*h))) == NULL) {
		return NULL;
	}
	h->val = val;
	h->next = next;

	return h;
}

void freeNode(link t) {
	free(t);

	return;
}

int PQinit(void) {
	if ((head = z = nuovoNodo(ITEMsetVoid(), NULL)) == NULL) {
		return 0;
	}

	return 1;
}

int PQempty(void) {
	return head == z;
}

int PQinsert(ITEM val) {
	link t;

	if ((t = nuovoNodo(val, head)) == NULL) {
		return 0;
	}

	head = t;

	return 1;
}

ITEM PQextractMin(void) {
	link x, linkMinPred, t;
	ITEM minItem;

	if (PQempty()) {
		return ITEMsetVoid();
	}
	linkMinPred = NULL;
	x = head;
	minItem = head->val;
	while (x->next != z) {
		if (ITEMlessP(x->next->val, minItem)) {
			minItem = x->next->val;
			linkMinPred = x;
		}
		x = x->next;
	}
	if (linkMinPred == NULL) {
		t = head;
		head = head->next;
		freeNode(t);
	}
	else {
		t = linkMinPred->next;
		linkMinPred->next = linkMinPred->next->next;
		freeNode(t);
	}

	return minItem;
}

ITEM PQextractByN(KEYN nomCercato) {
	link x, p, t;
	ITEM cercato;

	for (p = NULL, x = head; KEYNcompare(KEYNget(x->val), nomCercato) != 0 && x != z; p = x, x = x->next);

	if (x == z) {
		return ITEMsetVoid();
	}

	if (p == NULL) {
		t = head;
		cercato = head->val;
		head = head->next;
		freeNode(t);
	}
	else {
		t = x;
		cercato = x->val;
		p->next = x->next;
		freeNode(t);
	}

	return cercato;
}

void PQprint(FILE *output) {
	link x;

	for (x = head; x != z; x = x->next) {
		ITEMprint(output, x->val);
	}

	return;
}

void PQdestroy(void) {
	link x, t;

	x = head;
	while (x != z) {
		t = x;
		x = x->next;
		ITEMfree(t->val);
		freeNode(t);
	}
	freeNode(z);

	return;
}