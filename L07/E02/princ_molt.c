#include <stdio.h>
#include <stdlib.h>
#include "princ_molt.h"

// Funzione che alloca dinamicamente per via diretta la struct wrapper che
// rappresenta il vettore delle scelte e per via indiretta il vettore di struct
// t_amico ed i vettori di stringhe contenuti in ciascuna struct t_amico attraverso
// la chiamata a leggiVettScelte. Restituisce il puntatore alla struct wrapper 
// allocata o NULL se qualche allocazione fallisce o il file di input non viene aperto
// con successo.
t_amici *creaVettScelte(void) {
	t_amici *val;
	
	if ((val = malloc(sizeof(*val))) == NULL) {
		return NULL;
	}
	
	if (leggiVettScelte(val) == NULL) {
		return NULL;
	}

	return val;
}

// Funzione chiamata all'interno della creaVettScelte che si occupa in modo
// specifico della lettura di brani.txt e dell'allocazione dinamica dei costrutti
// all'interno della struct wrapper. Restituisce NULL se l'apertura del file o 
// qualche allocazione fallisce, il puntatore alla struct wrapper altrimenti.
t_amici *leggiVettScelte(t_amici *val) {
	FILE *input;
	int c_amici, c_canz;
	
	if ((input = fopen(NOMEINPUT, "r")) == NULL) {
		return NULL;
	}

	fscanf(input, "%d", &val->n_amici);
	if ((val->vet_amici = malloc(val->n_amici * sizeof(*val->vet_amici))) == NULL) {
		return NULL;
	}
	for (c_amici = 0; c_amici < val->n_amici; c_amici++) {
		fscanf(input, "%d", &val->vet_amici[c_amici].n_titoli);
		if ((val->vet_amici[c_amici].vet_titoli = malloc(val->vet_amici[c_amici].n_titoli * sizeof(*val->vet_amici[c_amici].vet_titoli))) == NULL) {
			return NULL;
		}
		for (c_canz = 0; c_canz < val->vet_amici[c_amici].n_titoli; c_canz++) {
			if ((val->vet_amici[c_amici].vet_titoli[c_canz] = malloc(MAXS * sizeof(*val->vet_amici[c_amici].vet_titoli[c_canz]))) == NULL) {
				return NULL;
			}
			fscanf(input, "%s", val->vet_amici[c_amici].vet_titoli[c_canz]);
		}
	}

	fclose(input);

	return val;
}

// Funzione che libera la memoria occupata dal vettore delle 
// scelte creato dalla creaVettScelte.
void distruggiVettScelte(t_amici *val) {
	int c_amici, c_canz;

	for (c_amici = 0; c_amici < val->n_amici; c_amici++) {
		for (c_canz = 0; c_canz < val->vet_amici[c_amici].n_titoli; c_canz++) {
			free(val->vet_amici[c_amici].vet_titoli[c_canz]);
		}
		free(val->vet_amici[c_amici].vet_titoli);
	}
	free(val->vet_amici);
	free(val);

	return;
}

// Funzione che alloca dinamicamente il vettore di interi usato
// per rappresentare le singole soluzioni. Restituisce NULL se 
// allocazione fallisce, altrimenti restituisce il puntatore
// al vettore creato.
int *creaVettSol(int dim_sol) {
	int *sol;
	
	if ((sol = malloc(dim_sol * sizeof(*sol))) == NULL) {
		return NULL;
	}

	return sol;
}

// Funzione che libera la memoria occupata dal vettore delle soluzioni.
void distruggiVettSol(int *sol) {
	free(sol);

	return;
}

// Funzione che si occupa di applicare il modello del principio di 
// moltiplicazione. Quando viene raggiunta la condizione di terminazione si
// stampa sul file di output la soluzione trovata.
void princ_molt(FILE *output, int pos, int *sol, t_amici val) {
	int c_sol, c_canz;

	if (pos == val.n_amici) {
		for (c_sol = 0; c_sol < val.n_amici; c_sol++) {
			fprintf(output, "%s ", val.vet_amici[c_sol].vet_titoli[sol[c_sol]]);
		}
		fprintf(output, "%s", "\n");
		return;
	}

	for (c_canz = 0; c_canz < val.vet_amici[pos].n_titoli; c_canz++) {
		sol[pos] = c_canz;
		princ_molt(output, pos + 1, sol, val);
	}

	return;
}