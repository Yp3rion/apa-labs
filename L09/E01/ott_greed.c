#include "ott_greed.h"

// Ho supposto che le stringhe abbiano lunghezza massima 255
// e siano prive di spazi
#define LNOMECAT 255 + 1
#define LNOMEELE 255 + 1

typedef struct {
	char *nomeElem;
	float punti;
	float appetibilità;
	float decremento;
	float difficoltà;
} t_elemento;

typedef struct {
	char *nomeCat;
	int numElem;
	int min;
	int max;
	int elemPresi;
	t_elemento *vetElem;
} t_categoria;

typedef struct {
	int idCat;
	int idElem;
} t_rifElem;

static t_categoria *vCat;
static int **vSol;
static t_rifElem *vOrd;
static t_rifElem *vAus;
static int lVet;
static int nElem = 0;

int creaVetCat(void);
int leggiVetCat(FILE *input);
void mergeSort(int modo);
void mergeSortR(int l, int r, int modo);
void merge(int l, int q, int r, int modo);
int creaVetSol(void);
int creaVettoriOrd(void);
void stampaSol(FILE *output);
void distruggiVetCat(void);
void distruggiVetSol(void);
void distruggiVettoriOrd(void);

int preparaOttim(void) {
	if (!creaVetCat()) {
		return 0;
	}
	if (!creaVetSol()) {
		return 0;
	}
	if (!creaVettoriOrd()) {
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
		vCat[c_cat].elemPresi = 0;
		nElem += vCat[c_cat].numElem;
		if ((vCat[c_cat].nomeCat = malloc((strlen(nomeCat) + 1) * sizeof(*vCat[c_cat].nomeCat))) == NULL) {
			return 0;
		}
		strcpy(vCat[c_cat].nomeCat, nomeCat);
		if ((vCat[c_cat].vetElem = malloc(vCat[c_cat].numElem * sizeof(*vCat[c_cat].vetElem))) == NULL) {
			return 0;
		}
		for (c_elem = 0; c_elem < vCat[c_cat].numElem; c_elem++) {
			fscanf(input, "%s %f %f", nomeElem, &vCat[c_cat].vetElem[c_elem].punti, &vCat[c_cat].vetElem[c_elem].difficoltà);
			vCat[c_cat].vetElem[c_elem].decremento = vCat[c_cat].vetElem[c_elem].punti / 2;
			vCat[c_cat].vetElem[c_elem].appetibilità = vCat[c_cat].vetElem[c_elem].punti / vCat[c_cat].vetElem[c_elem].difficoltà;
			if ((vCat[c_cat].vetElem[c_elem].nomeElem = malloc((strlen(nomeElem) + 1) * sizeof(*vCat[c_cat].vetElem[c_elem].nomeElem))) == NULL) {
				return 0;
			}
			strcpy(vCat[c_cat].vetElem[c_elem].nomeElem, nomeElem);
		}
	}

	return 1;
}

int creaVetSol(void) {
	int c_cat;

	if ((vSol = malloc(lVet * sizeof(*vSol))) == NULL) {
		return 0;
	}
	for (c_cat = 0; c_cat < lVet; c_cat++) {
		if ((vSol[c_cat] = malloc(vCat[c_cat].max * sizeof(*vSol[c_cat]))) == NULL) {
			return 0;
		}
	}

	return 1;
}

int creaVettoriOrd(void) {
	int c_cat, c_elem, c_totEl;

	if ((vOrd = malloc(nElem * sizeof(*vOrd))) == NULL) {
		return 0;
	}
	if ((vAus = malloc(nElem * sizeof(*vAus))) == NULL) {
		return 0;
	}
	c_totEl = 0;
	for (c_cat = 0; c_cat < lVet; c_cat++) {
		for (c_elem = 0; c_elem < vCat[c_cat].numElem; c_elem++) {
			vOrd[c_totEl].idCat = c_cat;
			vOrd[c_totEl].idElem = c_elem;
			c_totEl++;
		}
	}
	
	mergeSort(1);
	mergeSort(0);

	return 1;
}

void mergeSort(int modo) {
	int l, r;

	l = 0;
	r = nElem - 1;
	mergeSortR(l, r, modo);

	return;
}

void mergeSortR(int l, int r, int modo) {
	int q;

	q = (l + r) / 2;
	if (r <= l) {
		return;
	}
	mergeSortR(l, q, modo);
	mergeSortR(q + 1, r, modo);
	merge(l, q, r, modo);

	return;
}

void merge(int l, int q, int r, int modo) {
	int i, j, k;

	i = l;
	j = q + 1;
	for (k = l; k <= r; k++) {
		if (i > q) {
			vAus[k] = vOrd[j++];
		}
		else {
			if (j > r) {
				vAus[k] = vOrd[i++];
			}
			else {
				switch (modo) {
				case 0:
					if (vCat[vOrd[i].idCat].vetElem[vOrd[i].idElem].appetibilità >= vCat[vOrd[j].idCat].vetElem[vOrd[j].idElem].appetibilità) {
						vAus[k] = vOrd[i++];
					}
					else {
						vAus[k] = vOrd[j++];
					}
					break;
				case 1:
					if (vCat[vOrd[i].idCat].vetElem[vOrd[i].idElem].difficoltà <= vCat[vOrd[j].idCat].vetElem[vOrd[j].idElem].difficoltà) {
						vAus[k] = vOrd[i++];
					}
					else {
						vAus[k] = vOrd[j++];
					}
					break;
				}
			}
		}
	}
	for (k = l; k <= r; k++) {
		vOrd[k] = vAus[k];
	}

	return;
}

// Problema: quest'algoritmo dà risultati vicini a quelli ottimi per difficoltà massime superiori a 14, ma
// al di sotto di 14 non trova soluzioni.
void eseguiOttim(FILE *output, float maxDiff) {
	float puntTot, puntAtt;
	int c_cat, c_elem, c_ord;

	puntTot = 0;
	puntAtt = 0;
	for (c_cat = 0; c_cat < lVet; c_cat++) {
		c_elem = 0;
		for (c_ord = 0; c_ord < nElem && vCat[c_cat].elemPresi < vCat[c_cat].min && maxDiff >= 0; c_ord++) {
			if (vOrd[c_ord].idCat == c_cat) {
				vSol[c_cat][c_elem] = vOrd[c_ord].idElem;
				c_elem++;
				vCat[c_cat].elemPresi++;
				maxDiff -= vCat[c_cat].vetElem[vOrd[c_ord].idElem].difficoltà;
				puntAtt += vCat[c_cat].vetElem[vOrd[c_ord].idElem].punti;
				if (vCat[c_cat].vetElem[vOrd[c_ord].idElem].punti > 0) {
					vCat[c_cat].vetElem[vOrd[c_ord].idElem].punti -= vCat[c_cat].vetElem[vOrd[c_ord].idElem].decremento;
					vCat[vOrd[c_ord].idCat].vetElem[vOrd[c_ord].idElem].appetibilità = vCat[vOrd[c_ord].idCat].vetElem[vOrd[c_ord].idElem].punti / vCat[vOrd[c_ord].idCat].vetElem[vOrd[c_ord].idElem].difficoltà;
					mergeSort(0);
					c_ord = 0;
				}
			}
		}
	}
	if (maxDiff >= 0) {
		for (c_ord = 0; c_ord < nElem; c_ord++) {
			if (vCat[vOrd[c_ord].idCat].elemPresi < vCat[vOrd[c_ord].idCat].max && (maxDiff - vCat[vOrd[c_ord].idCat].vetElem[vOrd[c_ord].idElem].difficoltà) >= 0) {
				vSol[vOrd[c_ord].idCat][vCat[vOrd[c_ord].idCat].elemPresi] = vOrd[c_ord].idElem;
				vCat[vOrd[c_ord].idCat].elemPresi++;
				maxDiff -= vCat[vOrd[c_ord].idCat].vetElem[vOrd[c_ord].idElem].difficoltà;
				puntAtt += vCat[vOrd[c_ord].idCat].vetElem[vOrd[c_ord].idElem].punti;
				if (vCat[vOrd[c_ord].idCat].vetElem[vOrd[c_ord].idElem].punti > 0) {
					vCat[vOrd[c_ord].idCat].vetElem[vOrd[c_ord].idElem].punti -= vCat[vOrd[c_ord].idCat].vetElem[vOrd[c_ord].idElem].decremento;
					vCat[vOrd[c_ord].idCat].vetElem[vOrd[c_ord].idElem].appetibilità = vCat[vOrd[c_ord].idCat].vetElem[vOrd[c_ord].idElem].punti / vCat[vOrd[c_ord].idCat].vetElem[vOrd[c_ord].idElem].difficoltà;
					mergeSort(0);
					c_ord = 0;
				}
			}
		}
		puntTot = puntAtt;
		for (c_cat = 0; c_cat < lVet; c_cat++) {
			if (vCat[c_cat].elemPresi < vCat[c_cat].max) {
				vSol[c_cat][vCat[c_cat].elemPresi] = -1;
			}
		}
	}

	if (puntTot != 0) {
		fprintf(output, "Uno dei possibili programmi accettabili ha punteggio atteso pari a %.2f ed e' il seguente:\n", puntTot);
		stampaSol(output);
	}
	else {
		fprintf(output, "%s", "L'algoritmo non ha trovato un programma accettabile con questa difficolta' massima.\n");
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
	distruggiVetSol();
	distruggiVettoriOrd();

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

void distruggiVetSol(void) {
	int c_cat;

	for (c_cat = 0; c_cat < lVet; c_cat++) {
		free(vSol[c_cat]);
	}
	free(vSol);

	return;
}

void distruggiVettoriOrd(void) {
	free(vOrd);
	free(vAus);

	return;
}

