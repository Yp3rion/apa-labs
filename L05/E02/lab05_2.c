// LABORATORIO 5 ESERCIZIO 2
// DATA DI REALIZZAZIONE 6/11/2016

#include <stdio.h>
#include <stdlib.h>

void stampaCambioBase(int n, int b);
void svuotaBufStdin(void);

int main(void) {
	int n, b;

	fprintf(stdout, "%s", "Inserire il numero da cambiare di base: ");
	fscanf(stdin, "%d", &n);
	fprintf(stdout, "%s", "Inserire la nuova base: ");
	fscanf(stdin, "%d", &b);
	fprintf(stdout, "%s", "Il numero cambiato di base e' il seguente: ");
	stampaCambioBase(n, b);

	svuotaBufStdin();
	getchar();

	return EXIT_SUCCESS;
}

// Funzione ricorsiva che calcola le cifre del numero
// nella nuova base e le scrive all'inverso per dare
// loro l'ordine corretto.
void stampaCambioBase(int n, int b) {
	if (n < b) {
		fprintf(stdout, "%d", n);
	}
	else {
		stampaCambioBase(n / b, b);
		fprintf(stdout, "%d", n % b);
	}

	return;
}

// Funzione che garantisce il funzionamento della getchar finale, che altrimenti
// assorbirebbe whitespace ignorati da fscanf precedenti.
void svuotaBufStdin(void) {
	char fineBuf;

	do {
		fscanf(stdin, "%c", &fineBuf);
	} while (fineBuf != '\n');

	return;
}