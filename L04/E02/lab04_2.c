// LABORATORIO 4 ESERCIZIO 2
// DATA DI REALIZZAZIONE: 29/10/2016

#include <stdio.h>
#include <stdlib.h>

#define NOMEINPUT "matrice.txt"

int acquisisciMatDin(int*** p_p_mat, int *nr, int*nc);
int allocaMatDin(int*** p_p_mat, int nr, int nc);
void leggiMatDin(FILE *input, int** p_mat, int nr, int nc);
void scriviMatDin(FILE *output, int** p_mat, int nr, int nc);
int creaVettDiVett(int*** p_p_vetvet, int nr, int nc);
int contaDiag(int nr, int nc);
int minorValore(int a, int b);
void riempiVettDiVett(int** p_mat, int nr, int nc, int** p_vetvet, int modo);
void scriviVettDiVett(FILE *output, int** p_vetvet, int nr, int nc);


int main(void) {
	int **p_mat, **p_diag, **p_antidiag;
	int nr, nc;

	// Alloco, riempio e stampo la matrice:
	switch (acquisisciMatDin(&p_mat, &nr, &nc)) {
		case -1:
			fprintf(stdout, "Il file %s non e' stato aperto con successo.\n", NOMEINPUT);
			getchar();
			return EXIT_FAILURE;
		case 0:
			fprintf(stdout, "%s", "La matrice non e' stata allocata con successo.\n");
			getchar();
			return EXIT_FAILURE;
	}
	fprintf(stdout, "%s", "La matrice letta e' la seguente: \n");
	scriviMatDin(stdout, p_mat, nr, nc);

	// Alloco, riempio e stampo il vettore delle diagonali:
	if (!creaVettDiVett(&p_diag, nr, nc)) {
		fprintf(stdout, "%s", "Il vettore delle diagonali non e' stato allocato con successo.\n");
		getchar();
		return EXIT_FAILURE;
	}
	riempiVettDiVett(p_mat, nr, nc, p_diag, 0);
	fprintf(stdout, "%s", "Il vettore delle diagonali e' il seguente: \n");
	scriviVettDiVett(stdout, p_diag, nr, nc);

	// Alloco, riempio e stampo il vettore delle antidiagonali:
	if (!creaVettDiVett(&p_antidiag, nr, nc)) {
		fprintf(stdout, "%s", "Il vettore delle antidiagonali non e' stato allocato con successo.\n");
		getchar();
		return EXIT_FAILURE;
	}
	riempiVettDiVett(p_mat, nr, nc, p_antidiag, 1);
	fprintf(stdout, "%s", "Il vettore delle antidiagonali e' il seguente: \n");
	scriviVettDiVett(stdout, p_antidiag, nr, nc);

	fprintf(stdout, "%s", "Operazione terminata.\n");
	getchar();

	return EXIT_SUCCESS;
}

// Funzione che legge dal file NOMEINPUT la matrice, puntata da p_mat,
// di dimensioni nr ed nc (che restituisce al main mediante puntatori), 
// dopo averla allocata dinamicamente. Restituisce -1 se fallisce
// apertura file, 0 se fallisce allocazione dinamica, altrimenti 1.
int acquisisciMatDin(int*** p_p_mat, int* nr, int* nc) {
	FILE *input;

	if ((input = fopen(NOMEINPUT, "r")) == NULL) {
		return -1;
	}

	// Leggo da file le dimensioni della matrice e alloco dinamicamente
	// la matrice stessa:
	fscanf(input, "%d %d", nr, nc);
	if (!allocaMatDin(p_p_mat, *nr, *nc)) {
		return 0;
	}

	// Leggo da file la matrice e chiudo il file:
	leggiMatDin(input, *p_p_mat, *nr, *nc);
	fclose(input);

	return 1;
}

// Funzione che alloca dinamicamente la matrice p_mat di
// dimensioni nr ed nc date. Restituisce 0 se l'allocazione
// fallisce, altrimenti resistuisce 1.
int allocaMatDin(int*** p_p_mat, int nr, int nc) {
	int conta_r;
	
	if ((*p_p_mat = (int**)malloc(nr * sizeof(int*))) == NULL) {
		return 0;
	}
	for (conta_r = 0; conta_r < nr; conta_r++) {
		if (((*p_p_mat)[conta_r] = (int *)malloc(nc * sizeof(int))) == NULL) {
			return 0;
		}
	}

	return 1;
}

// Funzione che legge la matrice allocata dinamicamente p_mat, di dimensioni
// nr ed nc, dal file di input aperto nella acquisisciMatDin. N.B.: il puntatore
// a file passato non punta all'inizio del file ma a subito dopo i primi due 
// interi, letti dalla acquisisciMatDin, che sarebbero proprio nr ed nc.
void leggiMatDin(FILE *input, int** p_mat, int nr, int nc) {
	int conta_r, conta_c;

	for (conta_r = 0; conta_r < nr; conta_r++) {
		for (conta_c = 0; conta_c < nc; conta_c++) {
			fscanf(input, "%d", &p_mat[conta_r][conta_c]);
		}
	}

	return;
}

// Funzione che scrive sul file puntato da output la matrice
// dinamica p_mat, di dimensioni nr ed nc.
void scriviMatDin(FILE *output, int** p_mat, int nr, int nc) {
	int conta_r, conta_c;

	for (conta_r = 0; conta_r < nr; conta_r++) {
		for (conta_c = 0; conta_c < nc; conta_c++) {
			fprintf(output, "%d ", p_mat[conta_r][conta_c]);
		}
		fprintf(output, "%s", "\n");
	}

	return;
}

// Funzione che alloca dinamicamente il vettore di vettori puntato da p_vetvet,
// quindi alloca dinamicamente i singoli vettori di p_vetvet. Restituisce 0 se
// l'allocazione fallisce, 1 altrimenti.
int creaVettDiVett(int*** p_p_vetvet, int nr, int nc) {
	int conta_r;
	int n_vett, l_vett; // Numero di vettori nel vettore di vettori e lunghezza di ciascun vettore.
	int dim_min; // La minore tra nr ed nc.

	n_vett = contaDiag(nr, nc);
	dim_min = minorValore(nr, nc);

	if ((*p_p_vetvet = (int**)malloc(n_vett * sizeof(int *))) == NULL) {
		return 0;
	}
	l_vett = 1;
	for (conta_r = 0; conta_r < n_vett; conta_r++) {
		if (((*p_p_vetvet)[conta_r] = (int*)malloc(l_vett * sizeof(int))) == NULL) {
			return 0;
		}
		if (conta_r < dim_min - 1) {
			l_vett++;
		}
		if (conta_r > n_vett - dim_min - 1) {
			l_vett--;
		}
	}

	return 1;
}

// Elementare funzione che conta il numero di diagonali (e antidiagonali)
// presenti nella matrice di dimensioni nr ed nc date. Creata solo per
// ragioni di modularità del programma.
int contaDiag(int nr, int nc) {
	return (nr + nc - 1);
}

// Funzione che restituisce il minore tra gli interi a e b.
int minorValore(int a, int b) {
	if (a < b) {
		return a;
	}
	else {
		return b;
	}
}

// Funzione che riempie il vettore di vettori p_vetvet con i valori della 
// matrice p_mat di dimensioni nr ed nc, in base al valore di modo: se
// modo è uguale a 0 il vettore di vettori viene riempito con le diagonali
// della matrice, da in alto a destra fino a in basso a sinistra, se invece 
// modo vale 1 il vettore di vettore di vettori viene riempito con le 
// antidiagonali, da in alto a sinistra fino a in basso a destra.
void riempiVettDiVett(int** p_mat, int nr, int nc, int** p_vetvet, int modo) {
	int conta_r, conta_c, conta_v;
	int ds; // Conta di quante caselle mi sposto lungo la diagonale/antidiagonale.

	conta_v = 0;
	// Considero le diagonali, se modo vale 0:
	if (modo == 0) {
		conta_r = 0;
		for (conta_c = nc - 1; conta_c > 0; conta_c--) {
			for (ds = 0; conta_c + ds < nc && conta_r + ds < nr; ds++) {
				p_vetvet[conta_v][ds] = p_mat[conta_r + ds][conta_c + ds];
			}
			conta_v++;
		}
		while (conta_r < nr) {
			for (ds = 0; conta_c + ds < nc && conta_r + ds < nr; ds++) {
				p_vetvet[conta_v][ds] = p_mat[conta_r + ds][conta_c + ds];
			}
			conta_v++;
			conta_r++;
		}
	}
	// Considero le antidiagonali, se modo vale 1:
	if (modo == 1) {
		conta_c = 0;
		for (conta_r = 0; conta_r < nr - 1; conta_r++) {
			for (ds = 0; conta_c + ds < nc && conta_r - ds >= 0; ds++) {
				p_vetvet[conta_v][ds] = p_mat[conta_r - ds][conta_c + ds];
			}
			conta_v++;
		}
		while (conta_c < nc) {
			for (ds = 0; conta_c + ds < nc && conta_r - ds >= 0; ds++) {
				p_vetvet[conta_v][ds] = p_mat[conta_r - ds][conta_c + ds];
			}
			conta_v++;
			conta_c++;
		}
	}

	return;
}

// Funzione che scrive sul file puntato da output il vettore di vettori
// p_vetvet, che contiene le diagonali (o antidiagonali) della matrice
// di dimensioni nr ed nc.
void scriviVettDiVett(FILE *output, int** p_vetvet, int nr, int nc) {
	int n_vett;
	int conta_r, conta_c, lung_v;
	int min_dim;

	n_vett = contaDiag(nr, nc);
	min_dim = minorValore(nr, nc);
	lung_v = 1;
	for (conta_r = 0; conta_r < n_vett; conta_r++) {
		for (conta_c = 0; conta_c < lung_v; conta_c++) {
			fprintf(output, "%d ", p_vetvet[conta_r][conta_c]);
		}
		if (conta_r < min_dim - 1) {
			lung_v++;
		}
		if (conta_r > n_vett - min_dim - 1) {
			lung_v--;
		}
		fprintf(output, "%s", "\n");
	}

	return;
}