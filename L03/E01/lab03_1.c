// LABORATORIO 3 ESERCIZIO 1
// DATA DI REALIZZAZIONE: 20/10/2016

#include <stdio.h>
#include <stdlib.h>

#define MAXR 50
#define MAXC 50
#define NOMEINPUT "matrice.txt" // Siccome il nome del file non va richiesto all'utente, lo stabilisco personalmente.

int leggiMatrice(int matrix[][MAXC], int r_max, int* r_effettive, int* c_effettive);
int riconosciRegione(int matrix[][MAXC], int nr, int nc, int r_selez, int c_selez, int* b, int* h);

int main(void) {
	int M[MAXR][MAXC], nr, nc;
	int conta_r, conta_c, conta_reg;
	int base, altezza, area;

	if (!leggiMatrice(M, MAXR, &nr, &nc)) {
		fprintf(stdout, "Il file %s non e' stato aperto con successo.\n", NOMEINPUT);
		getchar();
		return EXIT_FAILURE;
	}

	conta_reg = 1;
	for (conta_r = 0; conta_r < nr; conta_r++) {
		for (conta_c = 0; conta_c < nc; conta_c++) {
			if (M[conta_r][conta_c] == 1) {
				if (riconosciRegione(M, nr, nc, conta_r, conta_c, &base, &altezza)) {
					fprintf(stdout, "Regione %d: estr. sup. sx = <%d, %d> b = %d, h = %d, Area = %d.\n", conta_reg, conta_r, conta_c, base, altezza, base*altezza);
					conta_reg++;
				}
			}
		}
	}

	getchar();

	return EXIT_SUCCESS;
}

// Questa funzione si occupa di aprire il file di input e leggere la matrice matrix di dimensioni
// massime r_max e MAXC; oltre a ciò, legge dalla prima riga del file di input anche le dimensioni
// effettive della matrice, che le sono passate per riferimento attraverso i puntatori r_effettive
// e c_effettive. Se la funzione non riesce ad aprire il file di input restituisce 0, altrimenti
// restituisce 1.
// N.B.: Se posso assumere che il file sia formattato in modo tale da non richiedere controlli,
// in realtà r_max è superfluo poiché r_effettive sarà sicuramente minore; tuttavia lo uso come
// parametro in quanto richiesto dall'esercizio stesso.
int leggiMatrice(int matrix[][MAXC], int r_max, int* r_effettive, int* c_effettive) {
	FILE *input;
	int conta_r, conta_c;

	if ((input = fopen(NOMEINPUT, "r")) == NULL) {
		return 0;
	}

	fscanf(input, "%d %d", r_effettive, c_effettive);
	for (conta_r = 0; conta_r < *r_effettive; conta_r++) {
		for (conta_c = 0; conta_c < *c_effettive; conta_c++) {
			fscanf(input, "%d", &matrix[conta_r][conta_c]);
		}
	}

	fclose(input);

	return 1;
}

// Questa funzione considera l'elemento della matrice matrix di dimensioni nr e nc individuato da
// r_selez e c_selez e controlla che sia l'estremo in alto a sinistra di una regione nera; se così
// è, calcola base e altezza della regione e restituisce 1, altrimenti restituisce 0.
int riconosciRegione(int matrix[][MAXC], int nr, int nc, int r_selez, int c_selez, int* b, int* h) {
	if ((r_selez - 1 < 0 || matrix[r_selez - 1][c_selez] == 0) && (c_selez - 1 < 0 || matrix[r_selez][c_selez - 1] == 0)) {
		for (*h = 1; r_selez + *h < nr && matrix[r_selez + *h][c_selez] == 1; (*h)++);
		for (*b = 1; c_selez + *b < nc && matrix[r_selez][c_selez + *b] == 1; (*b)++);
	}
	else { 
		return 0;
	}

	return 1;
}