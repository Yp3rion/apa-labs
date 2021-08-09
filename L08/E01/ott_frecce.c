#include <stdio.h>
#include <stdlib.h>
#include "ott_frecce.h"

int powersetComSemR(int *input, int *sol, int l_input, int l_sol, int pos, int start);
int controlla(int *input, int *sol, int l_input, int l_sol);
void giraFrecce(int *input, int *sol, int l_sol);

void powerset(int *input, int *sol, int l_input) {
	int l_sol;
	int c_sol;

	if (controlla(input, sol, l_input, 0)) {
		fprintf(stdout, "%s", "La configurazione di input e' gia' una soluzione valida.\n");
		return;
	}
	
	for (l_sol = 1; l_sol <= l_input && !powersetComSemR(input, sol, l_input, l_sol, 0, 0); l_sol++);

	fprintf(stdout, "%s", "Una soluzione in cui si gira il minimo numero di frecce e': \n");
	for (c_sol = 0; c_sol < l_sol; c_sol++) {
		fprintf(stdout, "Girare la freccia in posizione %d\n", sol[c_sol]);
	}

	return;
}

int powersetComSemR(int *input, int *sol, int l_input, int l_sol, int pos, int start) {
	int c_sol;
	
	if (pos >= l_sol) {
		if (controlla(input, sol, l_input, l_sol)) {
			return 1;
		}
		else {
			return 0;
		}
	}
	for (c_sol = start; c_sol < l_input; c_sol++) {
		sol[pos] = c_sol;
		if (powersetComSemR(input, sol, l_input, l_sol, pos + 1, c_sol + 1)) {
			return 1;
		}
	}

	return 0;
}

// Funzione che controlla la validità
// della soluzione contenuta nel vettore
// sol chiamando la giraFrecce per generare
// la combinazione proposta e valutandola
// opportunamente, dopodiché chiamando di 
// nuovo la giraFrecce per tornare nella
// configurazione di input. Restituisce 1
// se la soluzione e' valida, altrimenti
// restituisce 0.
int controlla(int *input, int *sol, int l_input, int l_sol) {
	int c_input;
	int c_sx, c_dx;
	int valida, scontro;

	giraFrecce(input, sol, l_sol);
	// Se la prima freccia e' girata verso destra o l'ultima
	// verso sinistra di sicuro la soluzione non e' valida.
	if (input[0] == 1 || input[l_input - 1] == 0) {
		giraFrecce(input, sol, l_sol);
		return 0;
	}
	for (c_input = 0, c_dx = 0, c_sx = 0, scontro = 0, valida = 1; c_input < l_input && valida; c_input++) {
		if (input[c_input] == 1 && scontro == 0) {
			scontro = 1;
		}
		if (input[c_input] == 0 && scontro == 1) {
			scontro = 0;
			if (c_dx != c_sx) {
				valida = 0;
			}
			c_dx = 0;
			c_sx = 0;
		}
		if (input[c_input] == 0) {
			c_dx++;
		}
		else {
			c_sx++;
		}
	}
	if (c_dx != c_sx) {
		valida = 0;
	}
	giraFrecce(input, sol, l_sol);

	return valida;
}

// Funzione che gira le frecce nel vettore di input
// a seconda del contenuto del vettore sol.
void giraFrecce(int *input, int *sol, int l_sol) {
	int c_sol;

	for (c_sol = 0; c_sol < l_sol; c_sol++) {
		if (input[sol[c_sol]] == 1) {
			input[sol[c_sol]] = 0;
		}
		else {
			input[sol[c_sol]] = 1;
		}
	}

	return;
}