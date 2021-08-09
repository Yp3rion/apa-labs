#include <stdio.h>
#include <stdlib.h>
#include "ott_greed.h"  // O #include "ott_crip.h" per la versione completa

void svuotaBufStdin(void);

int main(void) {
	float maxDiff;
	
	if (!preparaOttim()) {
		fprintf("Errore nell'apertura di %s o nell'allocazione dei vettori necessari.\n", NOMEINPUT);
		getchar();
		return EXIT_FAILURE;
	}

	fprintf(stdout, "%s", "Inserire la massima difficolta' complessiva consentita: ");
	fscanf(stdin, "%f", &maxDiff);
	fprintf(stdout, "%s", "\n");
	eseguiOttim(stdout, maxDiff);

	terminaOttim();
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