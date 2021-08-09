#include "part.h"

struct prob {
	int **rel;
	int N, K, M;
	int *sol;
	int *bSol;
	int m;
	float bUm;
	int solTrov;
};

void partR(int pos, int m, PROB prob);
void check(int m, PROB prob);

PROB PROBinit(int N, int K, int M) {
	PROB prob;
	int i;

	prob = malloc(sizeof(*prob));
	prob->N = N;
	prob->K = K;
	prob->M = M;
	prob->sol = malloc(N * sizeof(*prob->sol));
	prob->bSol = malloc(N * sizeof(*prob->sol));
	prob->rel = malloc(N * sizeof(*prob->rel));
	for (i = 0; i < prob->N; i++) {
		prob->rel[i] = malloc(N * sizeof(*prob->rel[i]));
	}
	prob->solTrov = 0;

	return prob;
}

void PROBread(FILE *input, PROB prob) {
	int i, j;

	for (i = 0; i < prob->N; i++) {
		for (j = 0; j < prob->N; j++) {
			fscanf(input, "%d", &prob->rel[i][j]);
		}
	}

	return;
}

void PROBsolve(PROB prob) {
	int i, j;

	partR(0, 0, prob);
	if (!prob->solTrov) {
		fprintf(stdout, "%s", "Nessuna soluzione trovata.\n");
	}
	else {
		fprintf(stdout, "La soluzione ottima trovata ha umore globale %.2f ed e' la seguente:\n", prob->bUm);
		for (i = 0; i < prob->m; i++) {
			fprintf(stdout, "Macchina %d: amico/i di indice/i", i + 1);
			for (j = 0; j < prob->N; j++) {
				if (prob->bSol[j] == i) {
					fprintf(stdout, " %d", j);
				}
			}
			fprintf(stdout, "%s", "\n");
		}
	}

	return;
}

// Funzione che calcola le possibili partizioni dell'insieme amici
// con l'algoritmo di Er e una volta trovata una potenziale soluzione
// ne controlla la validità chiamando la funzione check.
void partR(int pos, int m, PROB prob) {
	int i;
	
	if (pos >= prob->N) {
		if (m >= prob->N/(float)prob->M && m <= prob->K) {
			check(m, prob);
		}
		return;
	}

	for (i = 0; i < m; i++) {
		prob->sol[pos] = i;
		partR(pos + 1, m, prob);
	}
	prob->sol[pos] = m;
	partR(pos + 1, m + 1, prob);

	return;
}

void check(int m, PROB prob) {
	float um;
	int sommaTot, sommaPar;
	int i, j, k;
	int count; // Variabile che controlla il numero di amici in ogni partizione

	sommaPar = 0;
	sommaTot = 0;
	count = 0;
	for (i = 0; i < m; i++) {
		for (j = 0; j < prob->N; j++) {
			if (prob->sol[j] == i) {
				count++;
				for (k = j+1; k < prob->N; k++) {
					if (prob->sol[k] == i) {
						sommaPar += prob->rel[j][k];
					}
				}
			}
		}
		if (count > prob->M) {
			return;
		}
		count = 0;
		sommaTot += sommaPar;
		sommaPar = 0;
	}

	um = sommaTot / (float)m;
	if (!prob->solTrov || um > prob->bUm) {
		prob->bUm = um;
		prob->m = m;
		prob->solTrov = 1;
		for (i = 0; i < prob->N; i++) {
			prob->bSol[i] = prob->sol[i];
		}
	}

	return;
}

void PROBterminate(PROB prob) {
	int i;

	for (i = 0; i < prob->N; i++) {
		free(prob->rel[i]);
	}
	free(prob->rel);
	free(prob->sol);
	free(prob);

	return;
}