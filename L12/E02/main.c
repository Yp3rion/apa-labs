#include <stdio.h>
#include <stdlib.h>
#include "part.h"

#define NOMEINPUT "amici.txt"

int main(void) {
	FILE *input;
	PROB data;
	int N, K, M, U;

	input = fopen(NOMEINPUT, "r");
	fscanf(input, "%d%d%d%d", &N, &K, &M, &U);
	data = PROBinit(N, K, M, U);
	PROBread(input, data);
	fclose(input);

	PROBsolve(data);

	PROBterminate(data);
	getchar();

	return EXIT_SUCCESS;
}