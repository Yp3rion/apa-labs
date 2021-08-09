#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sciatori.h"
#include "skilift.h"

#define LMAXSTR 255 + 1

int main(int argc, char *argv[]) {
	char stringa[LMAXSTR];
	char skiliftId[LSKILIFTID];
	int cardId, tempo;

	initTabSkilift(argv[1]);
	initBSTsciatori();

	fprintf(stdout, "%s", "Inserire <skiliftId> <cardId> <tempo> o \"fine\" per terminare: ");
	gets(stringa);
	while (strcmp(stringa, "fine") != 0) {
		if (sscanf(stringa, "%s %d %d", skiliftId, &cardId, &tempo) != 3) {
			fprintf(stdout, "Input errato. Riprovare.\n");
		}
		else {
			if (authorize(cardId, skiliftId, tempo)) {
				fprintf(stdout, "ID carta %d: accesso allo skilift ID %s autorizzato.\n", cardId, skiliftId);
			}
			else {
				fprintf(stdout, "ID carta %d: accesso allo skilift ID %s non autorizzato.\n", cardId, skiliftId);
			}
		}
		fprintf(stdout, "%s", "\n");
		fprintf(stdout, "%s", "Inserire <skiliftId> <cardId> <tempo> o \"fine\" per terminare: ");
		gets(stringa);
	}

	fprintf(stdout, "%s", "\nTerminazione del programma...\n");
	distruggiBSTsciatori();
	destroyTabSkilift();
	getchar();

	return EXIT_SUCCESS;
}
