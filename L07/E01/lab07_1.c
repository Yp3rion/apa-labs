// LABORATORIO 7 ESERCIZIO 1
// DATA DI REALIZZAZIONE: 20/11/2016

#include <stdio.h>
#include <stdlib.h>

#define MAXC 10

void cover(int S[][MAXC], int n, int k);
int coverR(int S[][MAXC], int n, int k, int *sol, int pos, int *cov, int start);

// Main di prova basato sull'esempio dell'esercizio:
int main(void) {
	int S[5][MAXC] = { {1, 2, 3, 0}, {2, 3, 7, 0}, {7, 8, 9, 0}, {3, 4, 0}, {4, 5, 6, 0} };

	cover(S, 5, 3);

	getchar();

	return EXIT_SUCCESS;
}

// Funzione wrapper che riceve la matrice S, il numero di righe di S e le 
// dimensioni richieste k del set cover. Crea dinamicamente e distrugge i
// vettori cov (che contiene le occorrenze degli elementi di U nel set cover)
// e sol (che contiene gli indici di riga nella matrice dei sottoinsiemi che
// formano il set cover), inoltre chiama la funzione ricorsiva coverR e ne
// stampa i risultati.
void cover(int S[][MAXC], int n, int k) {
	int *cov, *sol;
	int c_sol, c_el;

	if ((cov = calloc(MAXC, sizeof(*cov))) == NULL) {
		return;
	}
	if ((sol = malloc(k * sizeof(*sol))) == NULL) {
		return;
	}
	if (coverR(S, n, k, sol, 0, cov, 0) == 1) {
		fprintf(stdout, "%s", "Il set cover trovato e' formato dai seguenti insiemi:\n");
		for (c_sol = 0; c_sol < k; c_sol++) {
			fprintf(stdout, "%s", "{ ");
			for (c_el = 0; S[sol[c_sol]][c_el] != 0; c_el++) {
				fprintf(stdout, "%d ", S[sol[c_sol]][c_el]);
			}
			fprintf(stdout, "%s", "}\n");
		}
	}
	else {
		fprintf(stdout, "%s", "Non esiste un set cover valido della dimensione richiesta.\n");
	}

	free(cov);
	free(sol);

	return;
}

// Funzione ricorsiva che utilizza il modello delle combinazioni semplici. Riceve
// la matrice, il numero di righe, le dimensioni del set cover richieste, i vettori
// dinamici sol e cov, il valore intero pos (che indica le dimensioni della soluzione
// attuale) e il valore intero start (che indica a partire da quale sottoinsieme/riga
// di S si sta costruendo la soluzione attuale). Restituisce 1 nel momento in cui 
// trova un set cover valido, 0 altrimenti.
int coverR(int S[][MAXC], int n, int k, int *sol, int pos, int *cov, int start) {
	int c_scelte, c_el;

	if (pos >= k) {
		for (c_el = 1; cov[c_el] != 0 && c_el < MAXC; c_el++);
		if (c_el == MAXC) {
			return 1;
		}
		else {
			return 0;
		}
	}
	for (c_scelte = start; c_scelte < n; c_scelte++) {
		sol[pos] = c_scelte;
		for (c_el = 0; S[c_scelte][c_el] != 0; c_el++) {
			cov[S[c_scelte][c_el]]++;
		}
		if (coverR(S, n, k, sol, pos + 1, cov, c_scelte + 1) == 1) {
			return 1;
		}
		for (c_el = 0; S[c_scelte][c_el] != 0; c_el++) {
			cov[S[c_scelte][c_el]]--;
		}
	}

	return 0;
}