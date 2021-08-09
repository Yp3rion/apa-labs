#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ingredienti.h"

// Funzione che crea un singolo nodo della lista di
// ingredienti a partire dal suo contenuto val e dal
// puntatore al suo successore next. Restituisce il
// puntatore h al nodo creato, NULL se l'allocazione
// dinamica fallisce.
link creaNodo(t_ingrediente val, link next) {
	link h;

	if ((h = malloc(sizeof(*h))) == NULL) {
		return NULL;
	}
	h->val = val;
	h->next = next;

	return h;
}

// Funzione che si occupa di aprire e leggere il file ingredienti.txt,
// costruendo di nodo in nodo la lista concatenata che rappresenta la
// tabella degli ingredienti per aggiunte in coda. Se qualche passo
// fallisce, restituisce NULL, altrimenti restituisce il puntatore
// alla struct involucro della lista, allocata dinamicamente.
LIST *creaTabIng(void) {
	LIST *tabIng;
	FILE *input;
	t_ingrediente val;

	if ((tabIng = malloc(sizeof(*tabIng))) == NULL) {
		return NULL;
	}
	tabIng->head = tabIng->tail = NULL;
	if ((input = fopen(NOMEINPUT, "r")) == NULL) {
		return NULL;
	}
	fscanf(input, "%*d"); // Scarto il numero di ingredienti, che non è strettamente necessario.
	while (fscanf(input, "%s %f %f", val.nome, &val.prezzo_k, &val.cal_g) == 3) {
		insNodoInCoda(tabIng, val);
		if (tabIng->head == NULL) {
			return NULL;
		}
	}
	fclose(input);

	return tabIng;
}

// Funzione che inserisce in coda alla lista rappresentata dalla
// struct puntata da tabIng un nuovo nodo, essendo dato il contenuto
// val di tale nuovo nodo.
void insNodoInCoda(LIST *tabIng, t_ingrediente val) {
	if (tabIng->head == NULL) {
		if ((tabIng->head = creaNodo(val, NULL)) == NULL) {
			return;
		}
		tabIng->tail = tabIng->head;
	}
	else {
		if ((tabIng->tail->next = (creaNodo(val, NULL))) == NULL) {
			tabIng->head = NULL;
			return;
		}
		tabIng->tail = tabIng->tail->next;
	}

	return;
}

// Funzione che riceve la chiave di ricerca (nome dell'ingrediente) e la lista
// (rappresentata dalla struct LIST tabIng) e restituisce il puntatore al nodo 
// contenente la chiave se la ricerca ha successo, NULL altrimenti.
t_ingrediente cercaIngNome(LIST tabIng, char nome[]) {
	link trovato;

	for (trovato = tabIng.head; trovato != NULL && strcmp(trovato->val.nome, nome) != 0; trovato = trovato->next);

	return trovato->val;
}

// Funzione che riceve il puntatore alla struct involucro (allocata dinamicamente)
// che rappresenta la lista di ingredienti tabIng e libera in sequenza le locazioni 
// di memoria occupate da ciascun nodo, dal primo fino all'ultimo. Libera infine la
// locazione occupata dalla struct involucro stessa.
void distruggiTabIng(LIST *tabIng) {
	link p, q;

	for (p = tabIng->head; p != NULL; p = q) {
		q = p->next;
		free(p);
	}
	free(tabIng);

	return;
}