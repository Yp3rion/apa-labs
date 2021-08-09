#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ingredienti.h"

#define GRAMMIPERCHILO 1000.0

typedef struct ingrediente{
	char *nome;
	float prezzo_k;
	float cal_g;
} t_ingrediente;

struct tabella {
	t_ingrediente *v_ing;
	int n_ing;
};

TAB leggiTabIng(TAB tabIng);
void quickSortTab(TAB tabIng);
void quickSortTabR(TAB tabIng, int l, int r);
int partitionHoare(TAB tabIng, int l, int r);

// Funzione che alloca dinamicamente l'involucro della collezione di
// ingredienti, quindi chiama la leggiTabIng che completa la creazione
// di tale collezione. A questo punto chiama la quickSortTab che ordina il
// vettore alfabeticamente per nomi. Se l'allocazione dell'involucro fallisce o 
// fallisce qualche passaggio nella leggiTabIng si ritorna NULL, altrimenti
// si ritorna il puntatore all'involucro.
TAB creaTabIng(void) {
	TAB tabIng;

	if ((tabIng = malloc(sizeof(*tabIng))) == NULL) {
		return NULL;
	}
	if (leggiTabIng(tabIng) == NULL) {
		return NULL;
	}
	quickSortTab(tabIng);

	return tabIng;
}

// Funzione che apre e legge il file ingredienti.txt e si occupa di allocare
// dinamicamente il vettore di ingredienti. Se l'apertura del file o l'allocazione
// fallisce, si ritorna NULL, altrimenti si ritorna il puntatore all'involucro
// della collezione che era stato ricevuto come parametro, invariato.
TAB leggiTabIng(TAB tabIng) {
	FILE *input;
	int c_ing;

	if ((input = fopen(NOMEFILEING, "r")) == NULL) {
		return NULL;
	}

	fscanf(input, "%d", &tabIng->n_ing);
	if ((tabIng->v_ing = malloc(tabIng->n_ing * sizeof(*tabIng->v_ing))) == NULL) {
		return NULL;
	}
	for (c_ing = 0; c_ing < tabIng->n_ing; c_ing++) {
		if ((tabIng->v_ing[c_ing].nome = malloc(MAXING * sizeof(*tabIng->v_ing[c_ing].nome))) == NULL) {
			return NULL;
		}
	}
	for (c_ing = 0; c_ing < tabIng->n_ing; c_ing++) {
		fscanf(input, "%s %f %f", tabIng->v_ing[c_ing].nome, &tabIng->v_ing[c_ing].prezzo_k, &tabIng->v_ing[c_ing].cal_g);
	}

	fclose(input);

	return tabIng;
}

// Funzione involucro che chiama la funzione di ordinamento
// ricorsiva quickSortTabR. Il suo scopo è ordinare il vettore
// degli ingredienti alfabeticamente per nome in maniera tale
// da velocizzare le operazioni di ricerca.
void quickSortTab(TAB tabIng) {
	int l, r;

	l = 0;
	r = tabIng->n_ing - 1;
	quickSortTabR(tabIng, l, r);

	return;
}

// Funzione ricorsiva che ordina il vettore contenuto in tabIng
// alfabeticamente per nomi usando l'algoritmo quick sort.
void quickSortTabR(TAB tabIng, int l, int r) {
	int piv;

	if (l >= r) {
		return;
	}
	piv = partitionHoare(tabIng, l, r);
	quickSortTabR(tabIng, l, piv - 1);
	quickSortTabR(tabIng, piv + 1, r);

	return;
}

// Funzione di partizione usata all'interno della quickSortTabR.
int partitionHoare(TAB tabIng, int l, int r) {
	int c_min, c_magg;
	t_ingrediente tmp;

	c_min = l - 1;
	c_magg = r;
	while (c_min < c_magg) {
		while (strcmp(tabIng->v_ing[++c_min].nome, tabIng->v_ing[r].nome) < 0);
		while (strcmp(tabIng->v_ing[--c_magg].nome, tabIng->v_ing[r].nome) > 0);
		if (c_min < c_magg) {
			tmp = tabIng->v_ing[c_min];
			tabIng->v_ing[c_min] = tabIng->v_ing[c_magg];
			tabIng->v_ing[c_magg] = tmp;
		}
	}
	tmp = tabIng->v_ing[c_min];
	tabIng->v_ing[c_min] = tabIng->v_ing[r];
	tabIng->v_ing[r] = tmp;

	return c_min;
}

// Funzione che cerca nella collezione di ingredienti l'ingrediente
// col nome richiesto mediante algoritmo di ricerca binaria.
// Se l'ingrediente viene trovato si ritorna il puntatore
// all'ingrediente nel vettore, altrimenti si ritorna NULL.
ING cercaIngPerNomeBin(TAB tabIng, char nome[]) {
	int m, l, r;

	l = 0;
	r = tabIng->n_ing - 1;

	while (l <= r) {
		m = l + (r - l) / 2;
		if (strcmp(nome, tabIng->v_ing[m].nome) == 0) {
			return &tabIng->v_ing[m];
		}
		if (strcmp(nome, tabIng->v_ing[m].nome) < 0) {
			r = m - 1;
		}
		else {
			l = m + 1;
		}
	}

	return NULL;
}

// Funzione che stampa sul file dato i dettagli di un ingrediente
// cercato per nome, informando il chiamante se l'ingrediente non esiste.
void stampaDetIng(FILE *output, TAB tabIng, char nome[]) {
	ING trovato;

	if ((trovato = cercaIngPerNomeBin(tabIng, nome)) == NULL) {
		fprintf(output, "%s", "L'ingrediente cercato non e' presente nella tabella.\n");
	}
	else {
		fprintf(output, "Nome ingrediente: %s\n"
						"Prezzo al chilo: %.2f\n" 
						"Calorie al grammo: %.2f\n", trovato->nome, trovato->prezzo_k, trovato->cal_g);
	}

	return;
}

// Funzione che stampa sul file dato il nome di un ingrediente 
// a cui si conosce il puntatore.
void stampaNomeIng(FILE *output, ING p_ing) {
	fprintf(output, "%s\n", p_ing->nome);

	return;
}

// Funzione che dato il puntatore all'ingrediente nella tabella e
// la quantità in grammi di tale ingrediente restituisce il prezzo 
// di quella quantità di ingrediente.
float costoIng(ING p_ing, int grammi) {
	return (p_ing->prezzo_k) * grammi / GRAMMIPERCHILO;
}

// Funzione che dato il puntatore all'ingrediente nella tabella e
// la quantità in grammi di tale ingrediente restituisce l'apporto 
// calorico di quella quantità di ingrediente.
float calorieIng(ING p_ing, int grammi) {
	return p_ing->cal_g * grammi;
}

// Funzione che stampa sul file dato l'elenco degli ingredienti
// della tabella, notificando se la tabella e' vuota o meno.
void stampaTabIng(FILE *output, TAB tabIng) {
	int c_ing;

	if (tabIng->n_ing == 0) {
		fprintf(output, "%s", "La tabella degli ingredienti e' vuota.\n");
		return;
	}
	fprintf(output, "%s", "Gli ingredienti presenti nella tabella sono:\n");
	for (c_ing = 0; c_ing < tabIng->n_ing; c_ing++) {
		fprintf(output, "%s\n", tabIng->v_ing[c_ing].nome);
	}
	
	return;
}

// Funzione che libera prima la memoria occupata dal vettore di ingredienti, 
// quindi la memoria occupata dall'involucro.
void distruggiTabIng(TAB tabIng) {
	int c_ing;

	for (c_ing = 0; c_ing < tabIng->n_ing; c_ing++) {
		free(tabIng->v_ing[c_ing].nome);
	}
	free(tabIng->v_ing);
	free(tabIng);

	return;
}