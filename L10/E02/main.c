#include <stdio.h>
#include <stdlib.h>
#include "ott_combsem.h"

void svuotaBufStdin(void);

int main(void) {
	int nrInv, ncInv;

	fprintf(stdout, "%s", "Inserire le dimensioni verticali e orizzontali dell'inventario <dimVer> <dimOr>: ");
	fscanf(stdin, "%d %d", &nrInv, &ncInv);
	
	initOtt(nrInv, ncInv);
	eseguiOtt();
	terminaOtt();

	fprintf(stdout, "%s", "Operazione terminata...\n");
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