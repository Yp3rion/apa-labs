#include "item.h"

Key KEYget(t_item *pval) {
	return pval->idElab;
}

int KEYcompare(Key key1, Key key2) {
	return strcmp(key1, key2);
}

t_item ITEMscan(FILE *input) {
	t_item t;

	fscanf(input, "%s %s", t.idElab, t.idRete);

	return t;
}

t_item ITEMsetvoid(void) {
	t_item val;

	strcpy(val.idElab, "");
	strcpy(val.idRete, "");

	return val;
}

int ITEMcheckvoid(t_item val) {
	if (KEYcompare(KEYget(&val), "") == 0) {
		return 1;
	}

	return 0;
}