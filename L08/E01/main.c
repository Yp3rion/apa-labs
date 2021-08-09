#include <stdio.h>
#include <stdlib.h>
#include "ott_frecce.h"

#define NOMEINPUT "frecce.txt"

int main(void) {
	int *vFrecce, *vSol, nFrecce, cFrecce;
	FILE *input;

	if ((input = fopen(NOMEINPUT, "r")) == NULL) {
		fprintf(stdout, "Impossibile aprire il file %s.\n", NOMEINPUT);
		getchar();
		return EXIT_FAILURE;
	}

	fscanf(input, "%d", &nFrecce);
	if ((vFrecce = malloc(nFrecce * sizeof(*vFrecce))) == NULL) {
		fprintf(stdout, "%s", "Errore nell'allocazione dinamica del vettore delle frecce.\n");
		getchar();
		return EXIT_FAILURE;
	}
	if ((vSol = malloc(nFrecce * sizeof(*vSol))) == NULL) {
		fprintf(stdout, "%s", "Errore nell'allocazione dinamica del vettore delle soluzioni.\n");
		getchar();
		return EXIT_FAILURE;
	}

	for (cFrecce = 0; cFrecce < nFrecce; cFrecce++) {
		fscanf(input, "%d", &vFrecce[cFrecce]);
	}

	fclose(input);

	powerset(vFrecce, vSol, nFrecce);

	free(vFrecce);
	free(vSol);

	getchar();

	return EXIT_SUCCESS;
}