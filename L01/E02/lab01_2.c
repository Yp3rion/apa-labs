// LABORATORIO 1 ESERCIZIO 2
// DATA DI REALIZZAZIONE: 6/10/2016

// Il programma si basa sul presupposto che il file di input sia a priori
// formattato nel modo corretto.

#include <stdio.h>
#include <stdlib.h>

#define MAXDIM 20
#define MAXCHAR 10
#define DIREZIONI 4 
#define COORDINATE 2

FILE *apri_file(char modalita[MAXCHAR + 1]);
void acquisisci_mat(FILE *fpin, int mat[MAXDIM][MAXDIM], int dimensioni);
void linearizza_mat(FILE *fpout, int mat[MAXDIM][MAXDIM], int dimensioni);

int main(void) {
	int matrix[MAXDIM][MAXDIM], dim;
	FILE *input;

	if ((input = apri_file("r")) == NULL) {
		fprintf(stdout, "%s", "Impossibile aprire il file richiesto.\n");
		getchar();
		return EXIT_FAILURE;
	}

	fscanf(input, "%d", &dim);
	acquisisci_mat(input, matrix, dim);
	fclose(input); 
	linearizza_mat(stdout, matrix, dim);

	getchar();

	return EXIT_SUCCESS;
}

// Funzione basilare che si limita a chiedere il file da aprire/creare, aprirlo con la
// modalità specificata chiamando fopen e a restituire il puntatore a tale file se ha avuto 
// successo, NULL se non lo ha aperto/creato.
FILE *apri_file(char modalita[MAXCHAR+1]) {
	char nome_file[MAXCHAR + 1];

	fprintf(stdout, "%s", "Inserire il nome del file da aprire: ");
	gets(nome_file);
	
	return fopen(nome_file, modalita);
}

// Funzione che legge da file opportunamente formattato la matrice quadrata di interi mat
// di dimensioni date (max N).
void acquisisci_mat(FILE *fpin, int mat[MAXDIM][MAXDIM], int dimensioni) {
	int r, c;

	for (r = 0; r < dimensioni; r++) {
		for (c = 0; c < dimensioni; c++) {
			fscanf(fpin, "%d", &mat[r][c]);
		}
	}

	return;
}

// Funzione che compie il lavoro di stampare sul file dato la matrice quadrata di interi mat 
// di dimensioni date (max N) in base ai criteri dell'esercizio. Per elaborarla mi sono ispirato
// alla risoluzione dell'esercizio riguardante il gomoku.
void linearizza_mat(FILE *fpout, int mat[MAXDIM][MAXDIM], int dimensioni) {
	int l, r; // l riga superiore/colonna sinistra della cornice, r riga inferiore/colonna destra.
	int x, y, conta_passaggi; 
	int spostamento[DIREZIONI][COORDINATE] = { {0,1}, {1,0}, {0, -1}, {-1, 0} }, conta_dir;

	for (l=0, r=dimensioni-1; l < r; l++, r--) { // Considero le cornici da esterno verso interno.
		// Parto da casella della cornice più in alto a sinistra:
		x = l;
		y = l;
		// All'interno di una cornice devo considerare ciascuna delle 4 direzioni nella sequenza data.
		for (conta_dir = 0; conta_dir < DIREZIONI; conta_dir++) {
			// Tengo conto di quante caselle scrivo per ciascuna direzione mediante conta_passaggi.
			for (conta_passaggi = l; conta_passaggi < r; conta_passaggi++) {
				fprintf(fpout, "%d ", mat[x][y]);
				// Incremento/decremento x o y in base alla direzione che sto percorrendo:
				x += spostamento[conta_dir][0];
				y += spostamento[conta_dir][1];
			}
		}
	}
	// Considero l'eventuale casella centrale.
	if (l == r) {
		fprintf(fpout, "%d ", mat[l][r]);
	}

	return;
}