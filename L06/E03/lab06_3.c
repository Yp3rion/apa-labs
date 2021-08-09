// LABORATORIO 6 ESERCIZIO 3
// DATA DI REALIZZAZIONE: 12/11/2016

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	double x;
	double y;
	double dist;
} t_punto;

typedef struct nodo * link;

typedef struct nodo {
	t_punto val;
	link next;
} t_nodo;

link addPointToList(double x, double y, link h);
t_punto creaVal(double x, double y);
double calcolaDist(double x, double y);
link newNode(t_punto val, link next);
void svuotaBufStdin(void);

int main(void) {
	double x, y;
	link head, att;

	head = NULL;
	fprintf(stdout, "%s", "N.B.: Inserire un carattere non numerico ne' whitespace termina la fase di input.\n\n");
	fprintf(stdout, "%s", "Inserire coordinate <x> <y>: ");
	while (fscanf(stdin, "%lf%lf", &x, &y) == 2) {
		head = addPointToList(x, y, head);
		fprintf(stdout, "%s", "Inserire coordinate <x> <y>: ");
	}

	if (head == NULL) {
		fprintf(stdout, "%s", "\nNon e' stato inserito alcun punto!\n");
	}
	else {
		fprintf(stdout, "%s", "\nEcco la lista dei punti inseriti, ordinati per distanza dall'origine del piano cartesiano:\n");
		for (att = head; att != NULL; att = att->next) {
			fprintf(stdout, "Coordinate: <%.2lf> <%.2lf>; distanza: %.2lf\n", att->val.x, att->val.y, att->val.dist);
		}
	}
	svuotaBufStdin();
	getchar();

	return EXIT_SUCCESS;
}

// Funzione che riceve le coordinate del punto da aggiungere
// e il puntatore all'attuale testa della lista e si occupa
// di aggiungere tale punto alla lista in modo da mantenerla
// ordinata. Restituisce il puntatore alla testa della lista
// modificata.
link addPointToList(double x, double y, link h) {
	t_punto val;
	link t, p;

	val = creaVal(x, y);
	if (h == NULL || val.dist < h->val.dist) {
		return newNode(val, h);
	}
	else {
		for (p = h, t = h->next; t != NULL && val.dist >= t->val.dist; p = t, t = t->next);
		if ((p->next = newNode(val, t)) == NULL) {
			return NULL;
		}
	}

	return h;
}

// Funzione che riceve le coordinate di un punto del piano
// cartesiano, chiama la calcolaDist per calcolare la sua 
// distanza dall'origine e restituisce la struct t_punto val, 
// che contiene questi dati.
t_punto creaVal(double x, double y) {
	t_punto val;

	val.x = x;
	val.y = y;
	val.dist = calcolaDist(x, y);

	return val;
}

// Funzione elementare, creata solo per maggiore modularità, che
// riceve dalla chiamante le coordinate x e y di un punto
// del piano cartesiano e restituisce la distanza di tale punto
// dall'origine del piano cartesiano stesso.
double calcolaDist(double x, double y) {
	return sqrt(pow(x, 2) + pow(y, 2));
}

// Funzione che riceve la struct di tipo t_punto val e il 
// link al successore next ed alloca dinamicamente un nodo
// della lista contenente i valori dati. Restituisce alla
// chiamante il puntatore a tale nodo o NULL se allocazione
// fallisce.
link newNode(t_punto val, link next) {
	link h;

	if ((h = malloc(sizeof(*h))) == NULL) {
		;
	}
	else {
		h->val = val;
		h->next = next;
	}

	return h;
}

// Funzione che percorre il buffer di stdin per scartare
// eventuali whitespace trascurati dalle fscanf precedenti.
void svuotaBufStdin(void) {
	char fineBuf;

	do {
		fscanf(stdin, "%c", &fineBuf);
	} while (fineBuf != '\n');

	return;
}