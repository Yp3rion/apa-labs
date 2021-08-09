#include <stdio.h>
#include <stdlib.h>
#include "princ_molt.h"

int main(void) {
	t_amici *val;
	int *sol;

	if ((val = creaVettScelte()) == NULL) {
		fprintf(stdout, "Il file %s non e' stato aperto con successo o l'allocazione del vettore delle scelte e' fallita.\n", NOMEINPUT);
		getchar();
		return EXIT_FAILURE;
	}
	if ((sol = creaVettSol(val->n_amici)) == NULL) {
		fprintf(stdout, "%s", "l'allocazione del vettore delle soluzioni e' fallita.\n");
		getchar();
		return EXIT_FAILURE;
	}

	fprintf(stdout, "%s", "Le possibili playlist sono:\n");
	princ_molt(stdout, 0, sol, *val);

	distruggiVettScelte(val);
	distruggiVettSol(sol);

	getchar();

	return EXIT_SUCCESS;
}