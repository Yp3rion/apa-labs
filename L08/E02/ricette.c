#include <stdio.h>
#include <stdlib.h>
#include "ricette.h"

typedef struct nodoIng *linkIng;
typedef struct nodoRic *linkRic;

typedef struct {
	ING p_ing;
	int grammi;
} t_ingInRic;

typedef struct nodoIng {
	t_ingInRic valIng;
	linkIng nextIng;
} t_nodoIng;

typedef struct {
	linkIng headIng;
	linkIng tailIng;
} listIng;

typedef struct {
	char *nome;
	int minuti;
	listIng ingredienti;
} t_ricetta;

typedef struct nodoRic {
	t_ricetta valRic;
	linkRic nextRic;
} t_nodoRic;

struct lista {
	linkRic head;
	linkRic tail;
};

LIST leggiListRic(LIST listRic, TAB tabIng);
linkIng creaNodoIng(t_ingInRic valIng, linkIng nextIng);
linkRic creaNodoRic(t_ricetta valRic, linkRic nextRic);

// Funzione che si occupa di allocare dinamicamente l'involucro
// della lista, quindi chiama la leggiListRic per completare
// la creazione della lista. Restituisce puntatore all'involucro
// o NULL se qualche passaggio fallisce.
LIST creaListRic(TAB tabIng) {
	LIST listRic;

	if ((listRic = malloc(sizeof(*listRic))) == NULL) {
		return NULL;
	}
	listRic->head = listRic->tail = NULL;
	if ((leggiListRic(listRic, tabIng)) == NULL) {
		return NULL;
	}

	return listRic;
}

// Funzione che apre il file ricette.txt e si occupa di allocare
// dinamicamente la lista vera e propria. Restituisce NULL se 
// l'apertura del file o qualche allocazione fallisce, altrimenti
// restituisce invariato il puntatore all'involucro della lista.
LIST leggiListRic(LIST listRic, TAB tabIng) {
	FILE *input;
	int n_ric, c_ric, n_ing, c_ing;
	char nomeIng[MAXING];
	t_ricetta valRic;
	t_ingInRic valIng;

	if ((input = fopen(NOMEFILERIC, "r")) == NULL) {
		return NULL;
	}
	fscanf(input, "%d", &n_ric);
	for (c_ric = 0; c_ric < n_ric; c_ric++) {
		if ((valRic.nome = malloc(MAXRIC * sizeof(*valRic.nome))) == NULL) {
			return NULL;
		}
		fscanf(input, "%s %d %d", valRic.nome, &valRic.minuti, &n_ing);
		valRic.ingredienti.headIng = valRic.ingredienti.tailIng = NULL;
		for (c_ing = 0; c_ing < n_ing; c_ing++) {
			fscanf(input, "%s %d", nomeIng, &valIng.grammi);
			if ((valIng.p_ing = cercaIngPerNomeBin(tabIng, nomeIng)) == NULL) {
				return NULL;
			}
			if (valRic.ingredienti.headIng == NULL) {
				if ((valRic.ingredienti.headIng = valRic.ingredienti.tailIng = creaNodoIng(valIng, NULL)) == NULL) {
					return NULL;
				}
			}
			else {
				if ((valRic.ingredienti.tailIng->nextIng = creaNodoIng(valIng, NULL)) == NULL) {
					return NULL;
				}
				valRic.ingredienti.tailIng = valRic.ingredienti.tailIng->nextIng;
			}
		}
		if (listRic->head == NULL) {
			if ((listRic->head = listRic->tail = creaNodoRic(valRic, NULL)) == NULL) {
				return NULL;
			}
		}
		else {
			if ((listRic->tail->nextRic = creaNodoRic(valRic, NULL)) == NULL) {
				return NULL;
			}
			listRic->tail = listRic->tail->nextRic;
		}
	}
	fclose(input);

	return listRic;
}

// Funzione che crea un nodo della lista di ingredienti contenente
// le informazioni passate per parametro. Restituisce NULL se 
// allocazione dinamica nodo fallisce, altrimenti restituisce il
// puntatore al nodo stesso.
linkIng creaNodoIng(t_ingInRic valIng, linkIng nextIng) {
	linkIng h;

	if ((h = malloc(sizeof(*h))) == NULL) {
		return NULL;
	}
	h->valIng = valIng;
	h->nextIng = nextIng;

	return h;
}

// Funzione che crea un nodo della lista di ricette contenente
// le informazioni passate per parametro. Restituisce NULL se 
// allocazione dinamica nodo fallisce, altrimenti restituisce il
// puntatore al nodo stesso.
linkRic creaNodoRic(t_ricetta valRic, linkRic nextRic) {
	linkRic h;

	if ((h = malloc(sizeof(*h))) == NULL) {
		return NULL;
	}
	h->valRic = valRic;
	h->nextRic = nextRic;

	return h;
}

// Funzione che cerca nella lista delle ricette la ricetta col nome
// dato e se la trova restituisce il puntatore ad essa, altrimenti
// restituisce NULL.
linkRic cercaRicPerNome(LIST listRic, char nome[]) {
	linkRic x;

	for (x = listRic->head; x != NULL && strcmp(nome, x->valRic.nome) != 0; x = x->nextRic);

	return x;
}

// Funzione che stampa sul file di output i dettagli di una ricerca cercata
// per nome, informando il chiamante se la ricetta non esiste.
void stampaDetRic(FILE *output, LIST listRic, char nome[]) {
	linkRic trovata;
	linkIng c_ing;

	if ((trovata = cercaRicPerNome(listRic, nome)) == NULL) {
		fprintf(output, "%s", "La ricetta cercata non e' presente in lista.\n");
	}
	else {
		fprintf(output, "Nome ricetta: %s\n"
						"Tempo di preparazione: %d minuti\n"
						"Ingredienti:\n", trovata->valRic.nome, trovata->valRic.minuti);
		for (c_ing = trovata->valRic.ingredienti.headIng; c_ing != NULL; c_ing = c_ing->nextIng) {
			fprintf(output, "%d grammi di ", c_ing->valIng.grammi);
			stampaNomeIng(output, c_ing->valIng.p_ing);
		}
	}

	return;
}

// Funzione che cerca nella lista la ricetta con nome dato e 
// se la trova ne restituisce il prezzo complessivo. Se la ricetta
// non viene trovata, si restituisce invece -1.
float costoRic(LIST listRic, char nome[]) {
	linkRic trovata;
	linkIng c_ing;
	float prezzoTot;

	if ((trovata = cercaRicPerNome(listRic, nome)) == NULL) {
		return -1.0;
	}
	else {
		prezzoTot = 0.0;
		for (c_ing = trovata->valRic.ingredienti.headIng; c_ing != NULL; c_ing = c_ing->nextIng) {
			prezzoTot += costoIng(c_ing->valIng.p_ing, c_ing->valIng.grammi);
		}
	}

	return prezzoTot;
}

// Funzione che cerca nella lista la ricetta con nome dato e 
// se la trova ne restituisce l'apporto calorico complessivo. Se la ricetta
// non viene trovata, si restituisce invece -1.
float calorieRic(LIST listRic, char nome[]) {
	linkRic trovata;
	linkIng c_ing;
	float calorieTot;

	if ((trovata = cercaRicPerNome(listRic, nome)) == NULL) {
		return -1.0;
	}
	else {
		calorieTot = 0.0;
		for (c_ing = trovata->valRic.ingredienti.headIng; c_ing != NULL; c_ing = c_ing->nextIng) {
			calorieTot += calorieIng(c_ing->valIng.p_ing, c_ing->valIng.grammi);
		}
	}

	return calorieTot;
}

// Funzione che stampa l'elenco di ricette contenenti l'ingrediente
// dato. Se l'ingrediente dato non e' presente nella tabella o nessuna
// ricetta lo contiene ciò viene notificato.
void stampaRicConIng(FILE *output, LIST listRic, TAB tabIng, char nomeIng[]) {
	ING trovato;
	linkRic c_ric;
	linkIng c_ing;
	int c_ricConIng;

	if ((trovato = cercaIngPerNomeBin(tabIng, nomeIng)) == NULL) {
		fprintf(output, "%s", "L'ingrediente richiesto non si trova nella tabella degli ingredienti.\n");
		return;
	}
	
	c_ricConIng = 0;
	for (c_ric = listRic->head; c_ric != NULL; c_ric = c_ric->nextRic) {
		for (c_ing = c_ric->valRic.ingredienti.headIng; c_ing != NULL && c_ing->valIng.p_ing != trovato; c_ing = c_ing->nextIng);
		if (c_ing != NULL) {
			fprintf(output, "La ricetta %s contiene l'ingrediente %s.\n", c_ric->valRic.nome, nomeIng);
			c_ricConIng++;
		}
	}
	if (c_ricConIng == 0) {
		fprintf(output, "Nessuna ricetta contiene l'ingrediente %s.", nomeIng);
	}

	return;
}

// Funzione che stampa sul file dato l'elenco delle ricette nella
// lista, notificando se la lista e' vuota.
void stampaListRic(FILE *output, LIST listRic) {
	linkRic c_ric;

	if (listRic->head == NULL) {
		fprintf(output, "%s", "La lista delle ricette e' vuota.\n");
		return;
	}
	fprintf(output, "%s", "Le ricette presenti nella lista sono:\n");
	for (c_ric = listRic->head; c_ric != NULL; c_ric = c_ric->nextRic) {
		fprintf(output, "%s\n", c_ric->valRic.nome);
	}

	return;
}

void aggiungiRicetta(LIST listRic, TAB tabIng) {
	t_ricetta valRic;
	t_ingInRic valIng;
	char nomeIng[MAXING];
	int c_ing, n_ing;
	linkIng x, t;

	if ((valRic.nome = malloc(MAXRIC * sizeof(*valRic.nome))) == NULL) {
		fprintf(stdout, "%s", "Errore nell'allocazione della stringa per il nome della ricetta.\n");
		return;
	}
	fprintf(stdout, "%s", "Inserire il nome della ricetta: ");
	fscanf(stdin, "%s", valRic.nome);
	fprintf(stdout, "%s", "Inserire il tempo di preparazione in minuti della ricetta: ");
	fscanf(stdin, "%d", &valRic.minuti);
	fprintf(stdout, "%s", "Inserire il numero di ingredienti nella ricetta: ");
	fscanf(stdin, "%d", &n_ing);
	valRic.ingredienti.headIng = valRic.ingredienti.tailIng = NULL;
	for (c_ing = 0; c_ing < n_ing; c_ing++) {
		fprintf(stdout, "Inserire il nome del %do ingrediente (l'ingrediente deve essere presente nella tabella): ", c_ing + 1);
		fscanf(stdin, "%s", nomeIng);
		if ((valIng.p_ing = cercaIngPerNomeBin(tabIng, nomeIng)) == NULL) {
			fprintf(stdout, "%s", "L'ingrediente dato non e' presente nella tabella.\n");
			free(valRic.nome);
			return;
		}
		fprintf(stdout, "Inserire la quantita' in grammi del %do ingrediente: ", c_ing + 1);
		fscanf(stdin, "%d", &valIng.grammi);
		if (valRic.ingredienti.headIng == NULL) {
			if ((valRic.ingredienti.headIng = valRic.ingredienti.tailIng = creaNodoIng(valIng, NULL)) == NULL) {
				fprintf(stdout, "%s", "Errore nell'aggiunta dell'ingrediente.\n");
				free(valRic.nome);
				return;
			}
		}
		else {
			if ((valRic.ingredienti.tailIng->nextIng = creaNodoIng(valIng, NULL)) == NULL) {
				fprintf(stdout, "%s", "Errore nell'aggiunta dell'ingrediente.\n");
				free(valRic.nome);
				x = valRic.ingredienti.headIng;
				while (x != NULL) {
					t = x;
					x = x->nextIng;
					free(t);
				}
				return;
			}
			valRic.ingredienti.tailIng = valRic.ingredienti.tailIng->nextIng;
		}
	}
	if (listRic->head == NULL) {
		if ((listRic->head = listRic->tail = creaNodoRic(valRic, NULL)) == NULL) {
			fprintf(stdout, "%s", "Errore nell'aggiunta della ricetta.\n");
			free(valRic.nome);
			x = valRic.ingredienti.headIng;
			while (x != NULL) {
				t = x;
				x = x->nextIng;
				free(t);
			}
			return;
		}
	}
	else {
		if ((listRic->tail->nextRic = creaNodoRic(valRic, NULL)) == NULL) {
			fprintf(stdout, "%s", "Errore nell'aggiunta della ricetta.\n");
			free(valRic.nome);
			x = valRic.ingredienti.headIng;
			while (x != NULL) {
				t = x;
				x = x->nextIng;
				free(t);
			}
			return;
		}
		listRic->tail = listRic->tail->nextRic;
	}

	return;
}

// Funzione che libera la memoria occupata dalla lista di ricette.
void distruggiListRic(LIST listRic) {
	linkRic t_ric, x_ric;
	linkIng t_ing, x_ing;

	x_ric = listRic->head;
	while (x_ric != NULL) {
		free(x_ric->valRic.nome);
		x_ing = x_ric->valRic.ingredienti.headIng;
		while (x_ing != NULL) {
			t_ing = x_ing;
			x_ing = x_ing->nextIng;
			free(t_ing);
		}
		t_ric = x_ric;
		x_ric = x_ric->nextRic;
		free(t_ric);
	}

	free(listRic);

	return;
}