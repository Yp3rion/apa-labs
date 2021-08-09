#include "item.h"

typedef struct item {
	char *nomePart;
	float punti;
} t_item;

char buf[MAXNOMPART];

ITEM ITEMnew(void);

KEYN KEYNget(ITEM val) {
	if (!ITEMcheckVoid(val)) {
		return val->nomePart;
	}

	return NULL;
}

int KEYNcompare(KEYN nome1, KEYN nome2) {
	return (strcmp(nome1, nome2));
}

KEYP KEYPget(ITEM val) {
	if (!ITEMcheckVoid(val)) {
		return &val->punti;
	}

	return NULL;
}

int KEYPless(KEYP punti1, KEYP punti2) {
	return *punti1 < *punti2;
}

int ITEMscan(FILE *input, ITEM *pVal) {
	ITEM tmp;
	int letturaCorretta;
	float punti;

	if (input == stdin) {
		fprintf(stdout, "Inserire il nome del nuovo partecipante: ");
		letturaCorretta = fscanf(input, "%s", buf);
		punti = (float) 10;
	}
	else {
		if (fscanf(input, "%s %f", buf, &punti) == 2) {
			letturaCorretta = 1;
		}
		else {
			*pVal = ITEMsetVoid();
			return 0;
		}
	}

	if (ITEMcheckVoid(tmp = ITEMnew())) {
		*pVal = tmp;
		return letturaCorretta;
	}

	if ((tmp->nomePart = malloc((strlen(buf) + 1) * sizeof(*tmp->nomePart))) == NULL) {
		ITEMfree(tmp);
		*pVal = ITEMsetVoid();
		return letturaCorretta;
	}
	strcpy(tmp->nomePart, buf);
	tmp->punti = punti;

	*pVal = tmp;

	return letturaCorretta;
}

ITEM ITEMnew(void) {
	ITEM newItem;

	if ((newItem = malloc(sizeof(*newItem))) == NULL) {
		return ITEMsetVoid();
	}
	newItem->nomePart = NULL;
	newItem->punti = 0;

	return newItem;
}

// Funzione che si occupa di deallocare la stringa che
// rappresenta il nome dell'ITEM e l'ITEM stesso.
void ITEMfree(ITEM val) {
	if (!ITEMcheckVoid(val)) {
		if (val->nomePart != NULL) {
			free(val->nomePart);
		}
		free(val);
	}

	return;
}

// Funzione che restituisce al chiamante un ITEM
// sotto forma di "oggetto vuoto", ovvero come
// puntatore a NULL.
ITEM ITEMsetVoid(void) {
	return NULL;
}

// Funzione che restituisce al chiamante
// 1 se l'ITEM fornito è un oggetto vuoto,
// 0 altrimenti.
int ITEMcheckVoid(ITEM val) {
	return val == NULL;
}

int ITEMlessP(ITEM val1, ITEM val2) {
	return *KEYPget(val1) < *KEYPget(val2);
}

void ITEMprint(FILE *output, ITEM val) {
	fprintf(output, "%s %.2f\n", val->nomePart, val->punti);

	return;
}