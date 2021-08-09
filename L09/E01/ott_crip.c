// MODULO REALIZZATO IN STILE QUASI ADT - SERVONO SOLO UN
// VETTORE DELLE SCELTE E UN VETTORE DELLE SOLUZIONI.

#include "ott_crip.h"

// Ho supposto che le stringhe abbiano lunghezza massima 255
// e siano prive di spazi
#define LNOMECAT 255 + 1
#define LNOMEELE 255 + 1

typedef struct {
	char *nomeElem;
	float punti;
	float decremento;
	float difficoltà;
} t_elemento;

typedef struct {
	char *nomeCat;
	int numElem;
	int min;
	int max;
	t_elemento *vetElem;
} t_categoria;

static t_categoria *vCat;
static int **vSol;
static int **vSolCorr;
static int lVet;

int creaVetCat(void);
int leggiVetCat(FILE *input);
int creaVettoriSol(void);
void eseguiOttimR(int posCat, float pTotAtt, float *pTotOtt, float maxDiff);
void combRipR(int posCat, int posElem, int lSol, int start, float pTotAtt, float *pTotOtt, float maxDiff);
void nuovaSolOtt(void);
void stampaSol(FILE *output);
void distruggiVetCat(void);
void distruggiVettoriSol(void);

int preparaOttim(void) {
	if (!creaVetCat()) {
		return 0;
	}
	if (!creaVettoriSol()) {
		return 0;
	}

	return 1;
}

int creaVetCat(void) {
	FILE *input;

	if ((input = fopen(NOMEINPUT, "r")) == NULL) {
		return 0;
	}

	fscanf(input, "%d", &lVet);
	if ((vCat = malloc(lVet * sizeof(*vCat))) == NULL) {
		return 0;
	}
	if (leggiVetCat(input) == 0) {
		return 0;
	}

	fclose(input);

	return 1;
}

int leggiVetCat(FILE *input) {
	int c_cat, c_elem;
	char nomeCat[LNOMECAT], nomeElem[LNOMEELE];

	for (c_cat = 0; c_cat < lVet; c_cat++) {
		fscanf(input, "%s %d %d %d", nomeCat, &vCat[c_cat].numElem, &vCat[c_cat].min, &vCat[c_cat].max);
		if ((vCat[c_cat].nomeCat = malloc((strlen(nomeCat) + 1) * sizeof(*vCat[c_cat].nomeCat))) == NULL) {
			return 0;
		}
		strcpy(vCat[c_cat].nomeCat, nomeCat);
		if ((vCat[c_cat].vetElem = malloc(vCat[c_cat].numElem * sizeof(*vCat[c_cat].vetElem))) == NULL) {
			return 0;
		}
		for (c_elem = 0; c_elem < vCat[c_cat].numElem; c_elem++) {
			fscanf(input, "%s %f %f", nomeElem, &vCat[c_cat].vetElem[c_elem].punti, &vCat[c_cat].vetElem[c_elem].difficoltà);
			vCat[c_cat].vetElem[c_elem].decremento = vCat[c_cat].vetElem[c_elem].punti / 2.0;
			if ((vCat[c_cat].vetElem[c_elem].nomeElem = malloc((strlen(nomeElem) + 1) * sizeof(*vCat[c_cat].vetElem[c_elem].nomeElem))) == NULL) {
				return 0;
			}
			strcpy(vCat[c_cat].vetElem[c_elem].nomeElem, nomeElem);
		}
	}

	return 1;
}

int creaVettoriSol(void) {
	int c_cat;

	if ((vSol = malloc(lVet * sizeof(*vSol))) == NULL) {
		return 0;
	}
	if ((vSolCorr = malloc(lVet * sizeof(*vSol))) == NULL) {
		return 0;
	}
	for (c_cat = 0; c_cat < lVet; c_cat++) {
		if ((vSol[c_cat] = malloc(vCat[c_cat].max * sizeof(*vSol[c_cat]))) == NULL) {
			return 0;
		}
		if ((vSolCorr[c_cat] = malloc(vCat[c_cat].max * sizeof(*vSolCorr[c_cat]))) == NULL) {
			return 0;
		}
	}

	return 1;
}

void eseguiOttim(FILE *output, float maxDiff) {
	float puntTot;

	puntTot = 0;
	eseguiOttimR(0, 0.0, &puntTot, maxDiff);

	if (puntTot != 0) {
		fprintf(output, "Il programma ottimale trovato ha punteggio atteso pari a %.2f ed e' il seguente:\n", puntTot);
		stampaSol(output);
	}
	else {
		fprintf(output, "%s", "Non e' possibile trovare una soluzione ottimale con questa difficolta' massima.\n");
	}
	
	return;
}

void eseguiOttimR(int posCat, float pTotAtt, float *pTotOtt, float maxDiff) {
	int k;
	
	if (posCat >= lVet) {
		if (pTotAtt > *pTotOtt) {
			nuovaSolOtt();
			*pTotOtt = pTotAtt;
		}
		return;
	}

	for (k = vCat[posCat].min; k <= vCat[posCat].max; k++) {
		combRipR(posCat, 0, k, 0, pTotAtt, pTotOtt, maxDiff);
	}

	return;
}

void combRipR(int posCat, int posElem, int lSol, int start, float pTotAtt, float *pTotOtt, float maxDiff) {
	int c_el;

	if (posElem >= lSol) {
		if (lSol < vCat[posCat].max) {
			vSolCorr[posCat][posElem] = -1;
		}
		eseguiOttimR(posCat + 1, pTotAtt, pTotOtt, maxDiff);
		return;
	}
	for (c_el = start; c_el < vCat[posCat].numElem; c_el++) {
		if ((maxDiff - vCat[posCat].vetElem[c_el].difficoltà) < 0) {
			;
		}
		else {
			vSolCorr[posCat][posElem] = c_el;
			maxDiff -= vCat[posCat].vetElem[c_el].difficoltà;
			if (vCat[posCat].vetElem[c_el].punti > 0) {
				pTotAtt += vCat[posCat].vetElem[c_el].punti;
			}
			vCat[posCat].vetElem[c_el].punti -= vCat[posCat].vetElem[c_el].decremento;
			combRipR(posCat, posElem + 1, lSol, start, pTotAtt, pTotOtt, maxDiff);
			vCat[posCat].vetElem[c_el].punti += vCat[posCat].vetElem[c_el].decremento;
			maxDiff += vCat[posCat].vetElem[c_el].difficoltà;
			if (vCat[posCat].vetElem[c_el].punti > 0) {
				pTotAtt -= vCat[posCat].vetElem[c_el].punti;
			}
		}
		start++;
	}
	
	return;
}

void nuovaSolOtt(void) {
	int c_cat, c_elem;

	for (c_cat = 0; c_cat < lVet; c_cat++) {
		for (c_elem = 0; c_elem < vCat[c_cat].max && vSolCorr[c_cat][c_elem] != -1; c_elem++) {
			vSol[c_cat][c_elem] = vSolCorr[c_cat][c_elem];
		}
		if (c_elem < vCat[c_cat].max) {
			vSol[c_cat][c_elem] = -1;
		}
	}

	return;
}

void stampaSol(FILE *output) {
	int c_cat, c_elem;

	for (c_cat = 0; c_cat < lVet; c_cat++) {
		fprintf(output, "Categoria: %s\n", vCat[c_cat].nomeCat);
		for (c_elem = 0; c_elem < vCat[c_cat].max && vSol[c_cat][c_elem] != -1; c_elem++) {
			fprintf(output, "- %s\n", vCat[c_cat].vetElem[vSol[c_cat][c_elem]].nomeElem);
		}
	}

	return;
}

void terminaOttim(void) {
	distruggiVetCat();
	distruggiVettoriSol();

	return;
}

void distruggiVetCat(void) {
	int c_cat, c_elem;

	for (c_cat = 0; c_cat < lVet; c_cat++) {
		for (c_elem = 0; c_elem < vCat[c_cat].numElem; c_elem++) {
			free(vCat[c_cat].vetElem[c_elem].nomeElem);
		}
		free(vCat[c_cat].vetElem);
		free(vCat[c_cat].nomeCat);
	}
	free(vCat);

	return;
}

void distruggiVettoriSol(void) {
	int c_cat;

	for (c_cat = 0; c_cat < lVet; c_cat++) {
		free(vSol[c_cat]);
		free(vSolCorr[c_cat]);
	}
	free(vSol);
	free(vSolCorr);

	return;
}