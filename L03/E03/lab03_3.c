// LABORATORIO 3 ESERCIZIO 3
// DATA DI REALIZZAZIONE: 27/10/2016

#include <stdio.h>
#include <stdlib.h>

#define MAXR 50
#define MAXC 50
#define NOMEINPUT "matrice.txt"

int leggiMatrice(int M[][MAXC], int maxr, int *nr, int *nc);
int verificaCammino(int M[][MAXC], int nr, int nc, int *p);
int verificaCasella(int M[][MAXC], int nr, int nc, int x, int y, int x_nuova, int y_nuova);
void tipoCammino(int cammino, int peso);
void svuota_buffer(void);

int main(void) {
	int matrix[MAXR][MAXC];
	int nr, nc;
	int peso, cammino;

	if (!leggiMatrice(matrix, MAXR, &nr, &nc)) {
		fprintf(stdout, "Il file %s non e' stato aperto con successo.\n", NOMEINPUT);
		getchar();
		return EXIT_FAILURE;
	}
	tipoCammino(cammino = verificaCammino(matrix, nr, nc, &peso), peso);
	
	svuota_buffer();
	getchar();

	return EXIT_SUCCESS;
}

// Funzione che si occupa solamente di riempire la matrice M di dimensioni massime
// maxr e MAXC, leggendo dal file "matrice.txt" prima le dimensioni effettive della
// matrice (che restituisce by reference al programma chiamante) e poi il contenuto
// della matrice stessa. Restituisce 0 se il file di input non è stato aperto con 
// successo, 1 altrimenti.
int  leggiMatrice(int M[][MAXC], int maxr, int *nr, int *nc) {
	FILE *input;
	int i, j;

	if ((input = fopen(NOMEINPUT, "r")) == NULL) {
		return 0;
	}

	fscanf(input, "%d %d", nr, nc);
	for (i = 0; i < *nr; i++) {
		for (j = 0; j < *nc; j++) {
			fscanf(input, "%d", &M[i][j]);
		}
	}
	fclose(input);

	return 1;
}

// Mi sono permesso di modificare in maniera limitata questa funzione
// rispetto alle richieste dell'esercizio per garantirmi la possibilità
// di distinguere i 3 possibili casi: cammino scorretto (la casella non
// è adiacente a quella attuale, è fuori dalla matrice o contiene 0), 
// cammino corretto ma non semplice, cammino corretto e semplice. La 
// funzione restituisce -1 nel primo caso, 0 nel secondo e 1 nel terzo.
int verificaCammino(int M[][MAXC], int nr, int nc, int *p) {
	int mat_passaggi[MAXR][MAXC];
	int conta_r, conta_c;
	int semplice; // Flag booleana.
	int x, y, x_nuova, y_nuova; // Coordinate dell'ultima casella letta e di quella appena ricevuta.

	// Inizializzo la mat_passaggi a 0, poi mano a mano che riceverò 
	// informazioni sul cammino indicherò con 1 le caselle già percorse.
	for (conta_r = 0; conta_r < nr; conta_r++) {
		for (conta_c = 0; conta_c < nc; conta_c++) {
			mat_passaggi[conta_r][conta_c] = 0;
		}
	}

	*p = 0;
	semplice = 1; // Parto dal presupposto che il cammino in input sia semplice.
	// Inizializzo x e y a -1 perché questo mi permette di fare un caso particolare
	// per il primo ciclo:
	x = -1;
	y = -1;
	fprintf(stdout, "%s", "Inserire coppia di coordinate, -1 -1 per terminare: ");
	while (fscanf(stdin, "%d %d", &x_nuova, &y_nuova) == 2 && !(x_nuova == -1 && y_nuova == -1)) {
		if (x == -1 && y == -1) {
			x = x_nuova;
			y = y_nuova;
		}
		if (!verificaCasella(M, nr, nc, x, y, x_nuova, y_nuova)) {
			return -1;
		}
		if (mat_passaggi[x_nuova][y_nuova] == 1) {
			semplice = 0;
		}
		else {
			mat_passaggi[x_nuova][y_nuova] = 1;
		}
		*p += M[x_nuova][y_nuova];
		x = x_nuova;
		y = y_nuova;
		fprintf(stdout, "%s", "Inserire coppia di coordinate, -1 -1 per terminare: ");
	}
	
	return semplice;
}

// Questa funzione verifica se la casella appena ricevuta in input dalla verificaCammino, di
// coordinate x_nuova e y_nuova, è accettabile, ovvero non contiene il valore 0, dista da quella
// precedente (di coordinate x e y) al massimo 1 ed è all'interno della matrice M di dimensioni 
// effettive nr ed nc. Restituisce 1 se la casella è valida, 0 se non è valida.
int verificaCasella(int M[][MAXC], int nr, int nc, int x, int y, int x_nuova, int y_nuova) {
	if (x_nuova >= 0 && x_nuova < nr && y_nuova >= 0 && y_nuova < nc) {
		if (abs(x - x_nuova) <= 1 && abs(y - y_nuova) <= 1 && M[x_nuova][y_nuova] != 0) {
			return 1;
		}
	}

	return 0;
}

// Funzione che riceve il risultato della verificaCammino nel parametro formale cammino
// e il peso dell'eventuale cammino semplice nel parametro formale peso, quindi stampa su
// stdout l'affermazione corretta a partire da questi due dati.
void tipoCammino(int cammino, int peso) {
	switch (cammino) {
	case -1:
		fprintf(stdout, "%s", "Il cammino proposto non e' corretto.\n");
		break;
	case 0:
		fprintf(stdout, "%s", "Il cammino proposto e' corretto ma non semplice.\n");
		break;
	case 1:
		fprintf(stdout, "Il cammino proposto e' corretto e semplice, il peso e' %d.\n", peso);
	}
}

// Svuota il buffer su stdin per garantire il funzionamento della getchar.
// N.B.: ipotizzo che l'ultimo carattere nel buffer sia sempre '\n'.
void svuota_buffer(void) {
	char fine_buf;

	do {
		fscanf(stdin, "%c", &fine_buf);
	} while (fine_buf != '\n');

	return;
}