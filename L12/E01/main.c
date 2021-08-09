#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

#define NOMEINPUT "tempio.txt"
#define NOMEOUTPUT "risultato.txt"

void svuotaBufStdin(void);

int main(void) {
	GRAPH tempio;
	FILE *input, *output;
	int V; // Numero di sale.
	int HP; // Punti vita iniziali.

	input = fopen(NOMEINPUT, "r");
	fscanf(input, "%d", &V);
	tempio = GRAPHinit(V);
	GRAPHread(input, tempio);
	fclose(input);

	fprintf(stdout, "%s", "Inserire i punti vita iniziali dell'avventuriero: ");
	fscanf(stdin, "%d", &HP);

	output = fopen(NOMEOUTPUT, "w");
	GRAPHbestPath(output, tempio, HP);
	fclose(output);

	fprintf(stdout, "%s", "\nIl file risultato.txt contiene l'esito della ricerca. Il programma sara' terminato.\n");
	GRAPHdestroy(tempio);
	svuotaBufStdin();
	getchar();

	return EXIT_SUCCESS;
}

void svuotaBufStdin(void) {
	char fineBuf;

	do {
		fscanf(stdin, "%c", &fineBuf);
	} while (fineBuf != '\n');

	return;
}