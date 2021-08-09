#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ingredienti.h"

#define GRAMMIPERCHILO 1000.0

void leggiSceltaStdin(char *p_scelta);
void valutaRicetta(LIST tabIng);
void svuotaBufStdin(void);

int main(void) {
	LIST *tabIng;
	char scelta;

	if ((tabIng = creaTabIng()) == NULL) {
		fprintf(stdout, "%s", "Qualcosa e' andato storto nella creazione della tabella degli ingredienti.\n");
		getchar();
		return EXIT_FAILURE;
	}

	fprintf(stdout, "%s", "Valutare una ricetta? S/N (case sensitive): ");
	leggiSceltaStdin(&scelta);
	while (scelta == 'S') {
		fprintf(stdout, "%s", "\n");
		valutaRicetta(*tabIng);
		fprintf(stdout, "%s", "\n");
		svuotaBufStdin();
		fprintf(stdout, "%s", "Valutare un'altra ricetta? S/N (case sensitive): ");
		leggiSceltaStdin(&scelta);
	}


	fprintf(stdout, "%s", "L'esecuzione del programma e' stata terminata.\n");
	distruggiTabIng(tabIng);
	getchar();

	return EXIT_SUCCESS;
}

// Funzione che ha il solo scopo di controllare
// l'input del carattere di scelta usato nel main:
// essa riceve il puntatore a tale carattere e legge
// il carattere da stdin, quindi se il carattere non
// e' ammesso avverte l'utente e ricomincia il ciclo.
// Se il carattere letto era diverso dal carattere di
// newline viene inoltre chiamata la svuotaBufStdin
// per scartare gli eventuali altri caratteri immessi
// dall'utente.
void leggiSceltaStdin(char *p_scelta) {
	do {
		fscanf(stdin, "%c", p_scelta);
		if (*p_scelta != 'S' && *p_scelta != 'N') {
			fprintf(stdout, "%s", "Inserire S o N: ");
		}
		if (*p_scelta != '\n') {
			svuotaBufStdin();
		}
	} while (*p_scelta != 'S' && *p_scelta != 'N');

	return;
}

// Funzione che riceve la lista di ingredienti contenuta nella struct involucro 
// tabIng. La funzione chiede all'utente nome ricetta, tempo di preparazione e
// ingredienti necessari: di ogni ingrediente che trova nella tabIng chiede la 
// quantità usata nella ricetta in grammi, quindi aggiorna in modo opportuno
// i valori di prezTot e calTot. Alla fine stampa su stdout i dati della ricetta
// e restituisce il controllo alla funzione chiamante.
void valutaRicetta(LIST tabIng) {
	t_ingrediente cercato;
	char nomeRicetta[MAXS];
	float minuti;
	char chiave[MAXS];
	float grammi;
	float prezTot, calTot;

	prezTot = 0;
	calTot = 0;
	fprintf(stdout, "%s", "Inserire il nome della ricetta: ");
	fscanf(stdin, "%s", nomeRicetta);
	fprintf(stdout, "%s", "Inserire il tempo di preparazione (in minuti): ");
	fscanf(stdin, "%f", &minuti);
	fprintf(stdout, "%s", "Inserire il nome di un ingrediente della ricetta o Fine se non ci sono altri ingredienti: ");
	fscanf(stdin, "%s", chiave);
	while (strcmp(chiave, "Fine") != 0) {
		cercato = cercaIngNome(tabIng, chiave);
		if ((strcmp(cercato.nome, "")) == 0) {
			fprintf(stdout, "%s", "L'ingrediente cercato non e' stato trovato nella tabella.\n");
		}
		else {
			fprintf(stdout, "Inserire la quantita' di %s necessaria (in grammi): ", chiave);
			fscanf(stdin, "%f", &grammi);
			prezTot += grammi * cercato.prezzo_k / GRAMMIPERCHILO;
			calTot += grammi * cercato.cal_g;
		}
		fprintf(stdout, "%s", "Inserire il nome di un ingrediente della ricetta o Fine se non ci sono altri ingredienti: ");
		fscanf(stdin, "%s", chiave);
	}
	fprintf(stdout, "%s si prepara in %.2f minuti, costa %.2f euro ed ha un apporto calorico di %.2f calorie.\n", nomeRicetta, minuti, prezTot, calTot);
	
	return;
}

// Funzione che scarta gli eventuali whitespace ignorati dalle occorrenze di
// fscanf.
void svuotaBufStdin(void) {
	char fineBuf;

	do {
		fscanf(stdin, "%c", &fineBuf);
	} while (fineBuf != '\n');

	return;
}
