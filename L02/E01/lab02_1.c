// LABORATORIO 2 ESERCIZIO 1
// DATA DI REALIZZAZIONE: 16/10/2016

// Il programma presuppone che i dati in input siano forniti nella forma corretta.

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXN 100 // Costante simbolica usata all'interno della funzione ruotaVettore per dare una dimensione al vettore Vc.
#define MAXSTR 50
#define MAXR 3 // Dimensioni massime della matrice. Non ricevendole in input, faccio coincidere con esse quelle effettive.
#define MAXC 3

void ruotaVettore(int V[], int dir, int N, int P); // Funzione non usata nel main ma facente parte dell'esercizio.
void ruotaMatrice(int M[][MAXC], char r_o_c, int target, int dim1, int dim2, int P, int dir);
void stampaMatrice(FILE *output, int M[][MAXC], int dim1, int dim2);

int main(void) {
	int matrix[MAXR][MAXC] = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9} };
	char str[MAXSTR+1], selettore; 
	int target, pos, dir;

	fprintf(stdout, "%s", "Stato iniziale della matrice:\n");
	stampaMatrice(stdout, matrix, MAXR, MAXC);
	fprintf(stdout, "%s", "\n");

	// Analizzo il selettore prima degli altri dati eventualmente presenti nella stringa:
	fprintf(stdout, "%s", "Fornire <selettore> <target> <posizioni> <direzione>: ");
	fgets(str, MAXSTR + 1, stdin);
	sscanf(str, " %c", &selettore); // Considero il primo carattere dopo gli eventuali spazi (iniziale del selettore).
	selettore = (char)tolower((int)selettore); // Nel caso in cui l'iniziale del selettore sia maiuscola.
	while (selettore != 'f') { // Se l'input è formattato correttamente, selettore contiene 'r', 'c', o 'f'.
		sscanf(str, "%*s%d%d%d", &target, &pos, &dir); // Il selettore è già stato considerato quindi lo scarto.
		ruotaMatrice(matrix, selettore, target, MAXR, MAXC, pos, dir);
		fprintf(stdout, "%s", "Nuovo stato della matrice:\n");
		stampaMatrice(stdout, matrix, MAXR, MAXC);
		fprintf(stdout, "%s", "\n");
		fprintf(stdout, "%s", "Fornire <selettore> <target> <posizioni> <direzione>: ");
		fgets(str, MAXSTR + 1, stdin);
		sscanf(str, " %c", &selettore);
		selettore = (char)tolower((int)selettore);
	}

	fprintf(stdout, "%s", "Termino programma...\n");
	getchar();

	return EXIT_SUCCESS;
}

// Questa funzione fa ruotare il vettore V di lunghezza N di P posizioni nella direzione
// dir data (0 verso sinistra, 1 verso destra) trattando il vettore come "circolare".
void ruotaVettore(int V[], int dir, int N, int P) {
	int Vc[MAXN];
	int i;

	// Copio in Vc il contenuto di V:
	for (i = 0; i < N; i++) {
		Vc[i] = V[i];
	}
	
	// Lo spostamento a sinistra di P equivale allo spostamento a destra di N - P.
	// Opero su P opportunamente, quindi considero solo lo spostamento a destra.
	P = dir ? P : N - P;
	for (i = 0; i < N; i++) {
		V[(i + P) % N] = Vc[i];
	}

	return;
}

// Questa funzione fa ruotare la riga o colonna (in base al carattere r_o_c, 'r' per riga,
// 'c' per colonna) target della matrice M di dimensioni dim1 e dim2 di P posizioni nella 
// direzione dir data (per le colonne 0 verso il basso, 1 verso l'altro, per le righe 0
// verso sinistra, 1 verso destra). Righe e colonne sono trattate come "circolari".
void ruotaMatrice(int M[][MAXC], char r_o_c, int target, int dim1, int dim2, int P, int dir) {
	int Vc[MAXR]; // N.B. se MAXC > MAXR sarebbe necessariamente Vc[MAXR].
	int i;

	target--; // La riga 1 è nel programma la riga di indice 0!

	// Espando ruotaVettore, esaminando i due casi (riga o colonna) separatamente.
	if (r_o_c == 'r') {
		for (i = 0; i < dim2; i++) {
			Vc[i] = M[target][i];
		}

		P = P % dim2; // P potrebbe essermi data maggiore delle dimensioni della matrice.
		P = dir ? P : dim2 - P;
		for (i = 0; i < dim2; i++) {
			M[target][(i + P) % dim2] = Vc[i];
		}
	}
	else {
		for (i = 0; i < dim1; i++) {
			Vc[i] = M[i][target];
		}

		P = P % dim1;
		P = dir ? dim1 - P : P;
		for (i = 0; i < dim1; i++) {
			M[(i + P) % dim1][target] = Vc[i];
		}
	}

	return;
}

// Questa funzione stampa la matrice M di dimensioni dim1 e dim2 date nel file puntato
// da output.
void stampaMatrice(FILE *output, int M[][MAXC], int dim1, int dim2) {
	int i, j;

	for (i = 0; i < dim1; i++) {
		for (j = 0; j < dim2; j++) {
			fprintf(output, "%d ", M[i][j]);
		}
		fprintf(output, "%s", "\n");
	}
	
	return;
}