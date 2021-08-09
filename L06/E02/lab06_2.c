// LABORATORIO 6 ESERCIZIO 2
// DATA DI REALIZZAZIONE: 11/11/2016

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSTR 200

typedef char * t_sottostringa;

typedef struct nodo {
	t_sottostringa val;
	struct nodo *next;
}t_nodo;

t_nodo *splitStringa(char *str, char sep);
t_nodo *newNode(t_sottostringa val, t_nodo *next);
t_sottostringa creaSottostr(char *str, int l);
void svuotaBufStdin(void);

int main(void) {
	char str[MAXSTR + 1], sep;
	t_nodo *head, *att;

	fprintf(stdout, "%s", "Inserire la stringa da scomporre: ");
	gets(str);
	fprintf(stdout, "%s", "Inserire il carattere separatore: ");
	fscanf(stdin, "%c", &sep);

	if ((head = splitStringa(str, sep)) == NULL) {
		fprintf(stdout, "%s", "Qualcosa e' andato storto nella creazione della lista di sottostringhe.\n");
		svuotaBufStdin();
		getchar();
		return EXIT_FAILURE;
	}
	
	fprintf(stdout, "%s", "Le sottostringhe individuate sono le seguenti: \n");
	for (att = head; att != NULL; att = att->next) {
		fprintf(stdout, "%s\n", att->val);
	}

	svuotaBufStdin();
	getchar();

	return EXIT_SUCCESS;
}

// Funzione che riceve il puntatore ad una stringa e un carattere separatore
// e si occupa di creare una lista delle sottostringhe che nella stringa
// originale sono distinte da un'occorrenza del separatore. Restituisce il puntatore
// alla testa della lista, NULL se qualche passaggio fallisce.
t_nodo *splitStringa(char *str, char sep) {
	t_nodo *h, *att;
	t_sottostringa tmp;
	char *posSep;

	// Creo un nodo fittizio in testa alla lista
	// per ridurre quantità di codice necessaria;
	// restituirò poi il puntatore h->next alla
	// vera testa della lista.
	if ((h = newNode("", NULL)) == NULL) {
		return NULL;
	}
	att = h; 

	while ((posSep = strchr(str, sep)) != NULL) {
		if ((tmp = creaSottostr(str, posSep - str)) == NULL) {
			return NULL;
		}
		if ((att->next = newNode(tmp, NULL)) == NULL) {
			return NULL;
		}
		att = att->next;
		str = posSep + 1;
	}
	if (str[0] != '\0') {
		if ((tmp = creaSottostr(str, strlen(str))) == NULL) {
			return NULL;
		}
		if ((att->next = newNode(tmp, NULL)) == NULL) {
			return NULL;
		}
	}

	return h->next;
}

// Funzione che si occupa di creare un nuovo nodo della lista;
// riceve dalla chiamante i contenuti del nuovo nodo (sottostringa
// e puntatore al nodo successore) e restituisce il puntatore al
// nodo appena creato (NULL se allocazione è fallita).
t_nodo *newNode(t_sottostringa val, t_nodo *next) {
	t_nodo *h;

	if ((h = malloc(sizeof(*h))) == NULL) {
		;
	}
	else {
		h->val = val;
		h->next = next;
	}

	return h;
}

// Funzione che riceve il puntatore alla stringa da cui estrarre
// la sottostringa e la lunghezza della sottostringa stessa. 
// Restituisce NULL se allocazione della sottostringa fallisce,
// altrimenti restituisce il puntatore alla sottostringa.
t_sottostringa creaSottostr(char *str, int l) {
	t_sottostringa tmp;
	int cCar;

	if ((tmp = malloc((l + 1) * sizeof(*tmp))) == NULL) {
		return NULL;
	}
	for (cCar = 0; cCar < l; cCar++) {
		tmp[cCar] = str[cCar];
	}
	tmp[cCar] = '\0';

	return tmp;
}

// Funzione che scarta eventuali caratteri residui nel buffer
// stdin per garantire il funzionamento della getchar finale.
void svuotaBufStdin(void) {
	char fineBuf;

	do {
		fscanf(stdin, "%c", &fineBuf);
	} while (fineBuf != '\n');

	return;
}