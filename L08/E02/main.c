#include <stdio.h>
#include <stdlib.h>
#include "ingredienti.h"
#include "ricette.h"

#define MAXCOMANDO 15 + 1
#define NUMCOMANDI 8

typedef enum {
	e_stampaRic,
	e_stampaIng,
	e_costo,
	e_calorie,
	e_elencaRic,
	e_elencaIng,
	e_elencaRicConIng,
	e_aggiungi,
	e_fine
} t_comando;

void stampaMenu(void);
t_comando leggiComando(void);
void eseguiComando(t_comando comando, LIST listRic, TAB tabIng);
void svuotaBufStdin(void);

int main(void) {
	TAB tabIng;
	LIST listRic;
	t_comando comando;

	if ((tabIng = creaTabIng()) == NULL) {
		fprintf(stdout, "%s", "Qualcosa e' andato storto durante la creazione della tabella degli ingredienti.\n");
		getchar();
		return EXIT_FAILURE;
	}
	if ((listRic = creaListRic(tabIng)) == NULL) {
		fprintf(stdout, "%s", "Qualcosa e' andato storto durante la creazione della lista delle ricette.\n");
		getchar();
		return EXIT_FAILURE;
	}

	stampaMenu();
	while ((comando = leggiComando()) != e_fine) {
		eseguiComando(comando, listRic, tabIng);
		stampaMenu();
	}

	fprintf(stdout, "%s", "\nChiusura del programma...\n");

	distruggiListRic(listRic);
	distruggiTabIng(tabIng);

	getchar();

	return EXIT_SUCCESS;
}

void stampaMenu(void) {
	fprintf(stdout, "%s", "Scegliere una delle seguenti opzioni(qualsiasi altro comando termina il programma):\n");
	fprintf(stdout, "%s", "- per stampare a video i dettagli di una ricetta, scrivere \"stampaRic\";\n");
	fprintf(stdout, "%s", "- per stampare a video i dettagli di un ingrediente, scrivere \"stampaIng\";\n");
	fprintf(stdout, "%s", "- per calcolare il costo di una ricetta, scrivere \"costo\";\n");
	fprintf(stdout, "%s", "- per calcolare l'apporto calorico di una ricetta, scrivere \"calorie\";\n");
	fprintf(stdout, "%s", "- per stampare a video l'elenco di tutte le ricette, scrivere \"elencaRic\";\n");
	fprintf(stdout, "%s", "- per stampare a video l'elenco di tutti gli ingredienti, scrivere \"elencaIng\";\n");
	fprintf(stdout, "%s", "- per stampare a video l'elenco delle ricette contenenti un certo ingrediente, scrivere \"elencaRicConIng\";\n");
	fprintf(stdout, "%s", "- per aggiungere una nuova ricetta, scrivere \"aggiungi\";\n");
	fprintf(stdout, "%s", "Scelta: ");

	return;
}

t_comando leggiComando(void) {
	char tabCom[NUMCOMANDI][MAXCOMANDO] = { "stampaRic", "stampaIng", "costo", "calorie", "elencaRic", "elencaIng", "elencaRicConIng", "aggiungi" };
	char comLetto[MAXCOMANDO];
	t_comando comando;

	gets(comLetto);

	for (comando = e_stampaRic; comando != e_fine && strcmp(tabCom[comando], comLetto) != 0; comando++);

	return comando;
}

void eseguiComando(t_comando comando, LIST listRic, TAB tabIng) {
	char nomeRic[MAXRIC], nomeIng[MAXING];
	float prezzoTot, calorieTot;
	
	switch (comando) {
		case e_stampaRic:
			fprintf(stdout, "%s", "\n");
			fprintf(stdout, "%s", "Inserire il nome della ricetta: ");
			fscanf(stdin, "%s", nomeRic);
			stampaDetRic(stdout, listRic, nomeRic);
			fprintf(stdout, "%s", "\n");
			svuotaBufStdin();
			break;
		case e_stampaIng:
			fprintf(stdout, "%s", "\n");
			fprintf(stdout, "%s", "Inserire il nome dell'ingrediente: ");
			fscanf(stdin, "%s", nomeIng);
			stampaDetIng(stdout, tabIng, nomeIng);
			fprintf(stdout, "%s", "\n");
			svuotaBufStdin();
			break;
		case e_costo:
			fprintf(stdout, "%s", "\n");
			fprintf(stdout, "%s", "Inserire il nome della ricetta: ");
			fscanf(stdin, "%s", nomeRic);
			if ((prezzoTot = costoRic(listRic, nomeRic)) == -1) {
				fprintf(stdout, "La ricetta %s non e' presente nella tabella.\n", nomeRic);
			}
			else {
				fprintf(stdout, "Il costo della ricetta %s e' di %.2f euro.\n", nomeRic, prezzoTot);
			}
			fprintf(stdout, "%s", "\n");
			svuotaBufStdin();
			break;
		case e_calorie:
			fprintf(stdout, "%s", "\n");
			fprintf(stdout, "%s", "Inserire il nome della ricetta: ");
			fscanf(stdin, "%s", nomeRic);
			if ((calorieTot = calorieRic(listRic, nomeRic)) == -1) {
				fprintf(stdout, "La ricetta %s non e' presente nella tabella.\n", nomeRic);
			}
			else {
				fprintf(stdout, "L'apporto calorico della ricetta %s e' di %.2f calorie.\n", nomeRic, calorieTot);
			}
			fprintf(stdout, "%s", "\n");
			svuotaBufStdin();
			break;
		case e_elencaRic:
			fprintf(stdout, "%s", "\n");
			stampaListRic(stdout, listRic);
			fprintf(stdout, "%s", "\n");
			break;
		case e_elencaIng:
			fprintf(stdout, "%s", "\n");
			stampaTabIng(stdout, tabIng);
			fprintf(stdout, "%s", "\n");
			break;
		case e_elencaRicConIng:
			fprintf(stdout, "%s", "\n");
			fprintf(stdout, "%s", "Inserire il nome dell'ingrediente: ");
			fscanf(stdin, "%s", nomeIng);
			stampaRicConIng(stdout, listRic, tabIng, nomeIng);
			fprintf(stdout, "%s", "\n");
			svuotaBufStdin();
			break;
		case e_aggiungi:
			fprintf(stdout, "%s", "\n");
			aggiungiRicetta(listRic, tabIng);
			fprintf(stdout, "%s", "\n");
			svuotaBufStdin();
	}

	return;
}

void svuotaBufStdin(void) {
	char fineBuf;

	do {
		fscanf(stdin, "%c", &fineBuf);
	} while (fineBuf != '\n');

	return;
}