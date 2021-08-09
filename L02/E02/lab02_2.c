// LABORATORIO 2 ESERCIZIO 1
// DATA DI REALIZZAZIONE 19/10/2016

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Avrò bisogno della funzione strlen.

#define NOMESORG "sorgente.txt"
#define NOMEDIZ "dizionario.txt"
#define NOMECOMP "compresso.txt"
#define MAXRG 200
#define MAXPAR 30  // Ipotizzo questa sia la massima lunghezza delle parole nel dizionario.
#define MAXDIZ 30  // Numero massimo di coppie presenti nel file dizionario.

void comprimi_file(FILE *input, FILE *output, char da_sostituire[][MAXPAR], char sostituti[][MAXPAR], int num_diz);

int main(void) {
	FILE *sorg, *diz, *comp;
	char da_sostituire[MAXDIZ][MAXPAR], sostituti[MAXDIZ][MAXPAR];
	int conta_diz, num_diz;

	if ((sorg = fopen(NOMESORG, "r")) == NULL) {
		fprintf(stdout, "Il file %s non e' stato aperto con successo.\n", NOMESORG);
		getchar();
		return EXIT_FAILURE;
	}

	if ((diz = fopen(NOMEDIZ, "r")) == NULL) {
		fprintf(stdout, "Il file %s non e' stato aperto con successo.\n", NOMEDIZ);
		fclose(sorg);
		getchar();
		return EXIT_FAILURE;
	}

	if ((comp = fopen(NOMECOMP, "w")) == NULL) {
		fprintf(stdout, "Il file %s non e' stato aperto con successo.\n", NOMECOMP);
		fclose(sorg);
		fclose(diz);
		getchar();
		return EXIT_FAILURE;
	}

	// Leggo dizionario.txt e riempio i vettori di stringhe da_sostituire e sostituti, quindi lo chiudo.
	fscanf(diz, "%d", &num_diz);
	for (conta_diz = 0; conta_diz < num_diz; conta_diz++) {
		fscanf(diz, "%s %s", sostituti[conta_diz], da_sostituire[conta_diz]);
	}
	fclose(diz);

	comprimi_file(sorg, comp, da_sostituire, sostituti, num_diz);
	fclose(sorg);
	fclose(comp);

	fprintf(stdout, "Operazione completata. %s contiene il risultato della compressione.\n", NOMECOMP);

	getchar();

	return EXIT_SUCCESS;
}

// Funzione che si occupa della compressione del file di input producendo il file di output; le vengono
// forniti due vettori di stringhe: da_sostituire contiene le stringhe da cercare nel file di input, 
// sostituti contiene le stringhe corrispondenti da scrivere nel file di output per rimpiazzare le prime.
void comprimi_file(FILE *input, FILE *output, char da_sostituire[][MAXPAR], char sostituti[][MAXPAR], int num_diz) {
	char riga[MAXRG + 1];
	int selez_car;
	int conta_diz;
	int car_ug, sostituito;

	while (fgets(riga, MAXRG + 1, input) != NULL) {
		// Scandisco la riga:
		selez_car = 0;
		while (riga[selez_car] != '\0') {
			sostituito = 0;
			for (conta_diz = 0; conta_diz < num_diz && !sostituito; conta_diz++) {
				for (car_ug = 0; riga[selez_car + car_ug] == da_sostituire[conta_diz][car_ug]; car_ug++);
				if (car_ug == strlen(da_sostituire[conta_diz])) { // La stringa in da_sostituire è contenuta per intero nell'input.
					fprintf(output, "%s", sostituti[conta_diz]); // Scrivo in output la stringa in sostituti corrispondente a quella trovata.
				}
			}
			if (!sostituito) { // Non ho trovato in da_sostituire una stringa valida, stampo in output il carattere e passo al successivo.
				fprintf(output, "%c", riga[selez_car]);
				selez_car++;
			}
			else { // Ho trovato una stringa valida in da_sostituire e ho stampato in output il sostituto; salto quindi tutti i caratteri che ho sostituito.
				selez_car += car_ug;
			}
		}
	}

	return;
}