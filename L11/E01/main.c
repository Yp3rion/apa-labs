#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "grafo.h"

#define NUMCOMANDI 4
#define MAXLCOMANDO 9 + 1

typedef enum {
	c_vertici,
	c_adiacenti,
	c_matrice,
	c_fine,
	c_err
} t_comando;

void stampaMenu();
t_comando leggiComando();
void eseguiComando(t_comando, G, int);
void svuotaBufStdin();

int main(int argc, char **argv) {
	FILE *input;
	G grafo;
	int V;
	t_comando comando;

	input = fopen(argv[1], "r");
	fscanf(input, "%d", &V);
	grafo = GRAPHinit(V);
	GRAPHread(input, grafo);
	fclose(input);

	stampaMenu();
	while ((comando = leggiComando()) != c_fine) {
		eseguiComando(comando, grafo, V);
		stampaMenu();
	}

	GRAPHdestroy(grafo);
	fprintf(stdout, "%s", "\nOperazione terminata...\n");
	svuotaBufStdin();
	getchar();

	return EXIT_SUCCESS;
}

void stampaMenu(void) {
	fprintf(stdout, "%s", "Scegliere una delle seguenti opzioni o scrivere \"fine\" per terminare il programma:\n");
	fprintf(stdout, "%s", "- per stampare il numero di nodi del grafo e il loro elenco per nome, scrivere \"vertici\";\n");
	fprintf(stdout, "%s", "- per stampare il numero di archi incidenti su un nodo dato ed elencare i nodi ad esso connessi, scrivere \"adiacenti\";\n");
	fprintf(stdout, "%s", "- per generare la matrice delle adiacenze relativa al grafo, scrivere \"matrice\";\n");
	fprintf(stdout, "%s", "Scelta: ");

	return;
}

t_comando leggiComando(void) {
	t_comando comando;
	char opzioni[NUMCOMANDI][MAXLCOMANDO] = {"vertici", "adiacenti", "matrice", "fine"};
	char comLetto[MAXLCOMANDO];

	fscanf(stdin, "%s", comLetto);
	for (comando = c_vertici; comando != c_err && strcmp(comLetto, opzioni[comando]) != 0; comando++);

	return comando;
}

void eseguiComando(t_comando comando, G grafo, int V) {
	char nomCercato[MAXLSTR];
	
	fprintf(stdout, "%s", "\n");
	switch (comando) {
		case c_vertici:
			fprintf(stdout, "Numero di nodi nel grafo: %d.\n", V);
			fprintf(stdout, "%s", "I nomi dei nodi sono: \n");
			GRAPHshownodes(grafo);
			break;
		case c_adiacenti:
			fprintf(stdout, "%s", "Inserire il nome del vertice a cui si e' interessati: ");
			fscanf(stdin, "%s", nomCercato);
			fprintf(stdout, "%s", "\n");
			fprintf(stdout, "Numero di archi incidenti sul nodo dato: %d.\n", GRAPHedgespernode(grafo, nomCercato));
			fprintf(stdout, "%s", "I nodi adiacenti al nodo dato sono:\n");
			GRAPHadjnodes(grafo, nomCercato);
			break;
		case c_matrice:
			fprintf(stdout, "%s", "La matrice delle adiacenze relativa al grafo e' la seguente:\n");
			GRAPHadjmat(grafo);
			break;
		case c_err:
			fprintf(stdout, "%s", "Il comando inserito non e' valido. Riprovare.\n");
			break;
	}
	fprintf(stdout, "%s", "\n");

	return;
}

void svuotaBufStdin(void) {
	char fineBuf;

	do {
		fscanf(stdin, "%c", &fineBuf);
	} while (fineBuf != '\n');

	return;
}