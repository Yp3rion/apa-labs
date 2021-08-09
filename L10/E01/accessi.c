#include "accessi.h"

typedef struct {
	char skiliftId[LSKILIFTID];
	int ultimoAcc;
	int timeInterval;
} t_accesso;

typedef struct nodo *link;

typedef struct nodo {
	t_accesso val;
	link next;
} t_nodo;

typedef struct lista {
	link head;
	link tail;
} t_lista;

link nuovoNodo(t_accesso val, link next);

link nuovoNodo(t_accesso val, link next) {
	link h;

	h = malloc(sizeof(*h));
	h->val = val;
	h->next = next;

	return h;
}

LIST initListAccessi(void) {
	LIST listAccessi;

	listAccessi = malloc(sizeof(*listAccessi));
	listAccessi->head = listAccessi->tail = NULL;

	return listAccessi;
}

int insOaggAccesso(LIST listAccessi, char skiliftId[], int ultimoAcc) {
	link x;
	t_accesso val;
	
	for (x = listAccessi->head; x != listAccessi->tail && strcmp(skiliftId, x->val.skiliftId) != 0; x = x->next);

	if (x == listAccessi->tail) {
		strcpy(val.skiliftId, skiliftId);
		val.ultimoAcc = ultimoAcc;
		val.timeInterval = timeIntervalGet(skiliftId);
		listAccessi->head = nuovoNodo(val, listAccessi->head);
	}
	else {
		if (ultimoAcc - x->val.ultimoAcc >= x->val.timeInterval) {
			x->val.ultimoAcc = ultimoAcc;
		}
		else {
			return 0;
		}
	}

	return 1;
}

void distruggiListAccessi(LIST listAccessi) {
	link h, t;

	h = listAccessi->head;
	while (h != listAccessi->tail) {
		t = h;
		h = h->next;
		free(t);
	}

	return;
}