#include "item.h"

struct item {
	char *str;
	int prio;
};

void ITEMshow(Item val) {
	fprintf(stdout, "%s", val->str);

	return;
}

Item ITEMnew(char *str, int prio) {
	Item t;

	t = malloc(sizeof(*t));
	t->str = malloc((strlen(str) + 1) * sizeof(*t->str));
	strcpy(t->str, str);
	t->prio = prio;

	return t;
}

int ITEMcmp(Item val1, Item val2) {
	return val1->prio - val2->prio;
}

void ITEMfree(Item val) {
	free(val->str);
	free(val);

	return;
}