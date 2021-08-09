#include "skilift.h"

typedef struct {
	char skiliftId[LSKILIFTID];
	int timeInterval;
} t_skilift;

static t_skilift *vSkilift;
static int nSkilift;

void initTabSkilift(char fileName[]) {
	FILE *input;
	int cSkilift;

	input = fopen(fileName, "r");

	fscanf(input, "%d", &nSkilift);
	vSkilift = malloc(nSkilift * sizeof(*vSkilift));
	for (cSkilift = 0; cSkilift < nSkilift; cSkilift++) {
		fscanf(input, "%s %d", vSkilift[cSkilift].skiliftId, &vSkilift[cSkilift].timeInterval);
	}

	fclose(input);

	return;
}

int timeIntervalGet(char skiliftId[]) {
	int cSkilift;
	int timeInterval;

	for (cSkilift = 0; cSkilift < nSkilift && strcmp(skiliftId, vSkilift[cSkilift].skiliftId) != 0; cSkilift++);
	timeInterval = vSkilift[cSkilift].timeInterval;

	return timeInterval;
}

void destroyTabSkilift(void) {
	free(vSkilift);

	return;
}