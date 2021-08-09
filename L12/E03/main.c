#include <stdio.h>
#include <stdlib.h>
#include "part.h"

#define NOMEINPUT "amici.txt"

int main(void) {
	FILE *input;
	PROB data;
	int N, K, M;

	input = fopen(NOMEINPUT, "r");
	fscanf(input, "%d%d%d", &N, &K, &M);
	data = PROBinit(N, K, M);
	PROBread(input, data);
	fclose(input);

	PROBsolve(data);

	PROBterminate(data);
	getchar();

	return EXIT_SUCCESS;
}