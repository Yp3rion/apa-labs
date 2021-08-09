// LABORATORIO 2 ESERCIZIO 3
// DATA DI REALIZZAZIONE: 17/10/2016

#include <stdio.h>
#include <stdlib.h>

#define NOMEFILEIN "Textin.txt"
#define NOMEFILEOUT "Textout.txt"
#define MAXR 20
#define MAXC 20
#define DIREZIONI 4
#define COORDINATE 2

void stampaMatrice(FILE *output, int mat[][MAXC], int dim1, int dim2);
void sommaIntorno(int matin[][MAXC], int matout[][MAXC], int dim1, int dim2, int r);
void svuota_buffer(void);

int main(void) {
	FILE *input, *output;
	int matin[MAXR][MAXC], matout[MAXR][MAXC];
	int nr, nc, conta_r, conta_c;
	int raggio;

	if ((input = fopen(NOMEFILEIN, "r")) == NULL) { // Siccome il nome del file non va richiesto all'utente, lo specifico io.
		fprintf(stdout, "Il file %s di input non e' stato aperto con successo.\n", NOMEFILEIN);
		getchar();
		return EXIT_FAILURE;
	}

	if ((output = fopen(NOMEFILEOUT, "w")) == NULL) { // Anche qui, il nome del file non va richiesto all'utente, quindi lo specifico io. 
		fprintf(stdout, "Il file %s di output non e' stato creato con successo.\n", NOMEFILEOUT);
		fclose(input); // Chiudo correttamente il file di input che era già stato aperto.
		getchar();
		return EXIT_FAILURE;
	}

	// Leggo da input dimensioni effettive di matin e la riempio, quindi chiudo input:
	fscanf(input, "%d %d", &nr, &nc);
	for (conta_r = 0; conta_r < nr; conta_r++) {
		for (conta_c = 0; conta_c < nc; conta_c++) {
			fscanf(input, "%d", &matin[conta_r][conta_c]);
		}
	}
	fclose(input);

	fprintf(stdout, "La matrice contenuta nel file %s e':\n", NOMEFILEIN);
	stampaMatrice(stdout, matin, nr, nc);
	fprintf(stdout, "%s", "\n");

	fprintf(stdout, "%s", "Inserire raggio dell'intorno su cui effettuare le somme: ");
	fscanf(stdin, "%d", &raggio);
	sommaIntorno(matin, matout, nr, nc, raggio);

	// Stampo i dati richiesti nel file di output, quindi chiudo tale file e termino il programma:
	fprintf(output, "%d %d\n", nr, nc);
	stampaMatrice(output, matout, nr, nc);
	fclose(output);
	fprintf(stdout, "Operazione completata. Il file %s contiene la matrice risultante.\n", NOMEFILEOUT);

	svuota_buffer();
	getchar();

	return EXIT_SUCCESS;
}

// Funzione che stampa la matrice mat di dimensioni date dim1 e dim2 nel file 
// puntato da output.
void stampaMatrice(FILE *output, int mat[][MAXC], int dim1, int dim2) {
	int i, j;

	for (i = 0; i < dim1; i++) {
		for (j = 0; j < dim2; j++) {
			fprintf(output, "%d ", mat[i][j]);
		}
		fprintf(output, "%s", "\n");
	}

	return;
}

// Funzione che riceve la funzione matin (di dimensioni dim1 e dim2) e il raggio dell'intorno
// r e a partire da essi applica i criteri dell'esercizio per riempire la matrice matout. 
void sommaIntorno(int matin[][MAXC], int matout[][MAXC], int dim1, int dim2, int r) {
	int selez_r, selez_c;
	int spostamento[DIREZIONI][COORDINATE] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} }, x, y;
	int conta_passi, conta_dir;
	int somma;

	
	for (selez_r = 0; selez_r < dim1; selez_r++) {
		for (selez_c = 0; selez_c < dim2; selez_c++) {  // Ho selezionato un elemento di matin. Ora calcolerò la somma relativa ad esso.
			somma = 0;
			// Imposto coordinate iniziali:
			x = selez_r - r;
			y = selez_c - r;
			// Mi sposto lungo la "cornice" che rappresenta l'intorno di raggio r di matin[selez_r][selez_c]:
			for (conta_dir = 0; conta_dir < DIREZIONI; conta_dir++) {
				for (conta_passi = 0; conta_passi < 2 * r; conta_passi++) {
					if (x >= 0 && x < dim1 && y >= 0 && y < dim2) {
						somma += matin[x][y]; // Incremento la somma solo se sto considerando una casella della matrice.
					}
					// Incremento le coordinate opportunamente a seconda della direzione in cui mi muovo:
					x += spostamento[conta_dir][0];
					y += spostamento[conta_dir][1];
				}
			}
			matout[selez_r][selez_c] = somma; // Inserisco la somma calcolata nell'opportuna casella di matout.
		}
	}

	return;
}

// Funzione che in questo programma ha l'unico scopo di svuotare il buffer di stdin
// per assicurare il funzionamento della getchar. Ipotizzo che il carattere '\n' sia 
// sempre l'ultimo nel buffer.
void svuota_buffer(void) {
	char fine_buffer;

	do {
		fscanf(stdin, "%c", &fine_buffer);
	} while (fine_buffer != '\n');

	return;
}