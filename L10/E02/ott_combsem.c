#include "ott_combsem.h"

typedef struct {
	char nome[MAXLNOMEOGG];
	char categoria[MAXLNOMECAT];
	int nr;
	int nc;
	int area;
	int utilità;
} t_oggetto;

typedef struct {
	int idOgg;
	int r;
	int c;
} t_scelta;

void creaInv(void);
void initInv(void);
void creaVettScelte(void);
void leggiVettScelte(FILE *input);
void creaVettSol(void);
void combSem(int pos, int start, int areaInvLib, int utTotCorr);
int oggRichiesti(void);
int disponiOgg(void);
void ordinaSolAreaDecQS(int lSol);
void quickSortR(int l, int r);
int partition(int l, int r);
void stampaSol(void);
void distruggiVetScelte(void);
void distruggiVetSol(void);
void distruggiInv(void);

static t_oggetto *vScelte;
static t_scelta *vSol;
static t_scelta *vSolCorr;
static int *vOrdinato;
static int nOgg;
static int **inv;
static int nrInv;
static int ncInv;
static int utTot;

void initOtt(int nrInvLetto, int ncInvLetto) {
	nrInv = nrInvLetto;
	ncInv = ncInvLetto;

	creaInv();
	creaVettScelte();
	creaVettSol();

	return;
}

void creaInv(void) {
	int crInv;
	
	inv = malloc(nrInv * sizeof(*inv));
	for (crInv = 0; crInv < nrInv; crInv++) {
		inv[crInv] = malloc(ncInv * sizeof(*inv[crInv]));
	}
	initInv();

	return;
}

void initInv(void) {
	int crInv, ccInv;

	for (crInv = 0; crInv < nrInv; crInv++) {
		for (ccInv = 0; ccInv < ncInv; ccInv++) {
			inv[crInv][ccInv] = -1;
		}
	}

	return;
}

void creaVettScelte(void) {
	FILE *input;

	input = fopen(NOMEINPUT, "r");
	fscanf(input, "%d", &nOgg);
	vScelte = malloc(nOgg * sizeof(*vScelte));
	leggiVettScelte(input);
	fclose(input);

	return;
}

void leggiVettScelte(FILE *input) {
	int cOgg;

	for (cOgg = 0; cOgg < nOgg; cOgg++) {
		fscanf(input, "%s %s %d %d %d", vScelte[cOgg].nome, vScelte[cOgg].categoria, &vScelte[cOgg].nr, &vScelte[cOgg].nc, &vScelte[cOgg].utilità);
		vScelte[cOgg].area = vScelte[cOgg].nr * vScelte[cOgg].nc;
	}

	return;
}

void creaVettSol(void) {
	vSol = malloc(nOgg * sizeof(*vSol));
	vSolCorr = malloc(nOgg * sizeof(*vSolCorr));
	vOrdinato = malloc(nOgg * sizeof(*vOrdinato));

	return;
}

void eseguiOtt(void) {
	combSem(0, 0, nrInv * ncInv, 0);

	fprintf(stdout, "%s", "\n");
	if (utTot == 0) {
		fprintf(stdout, "%s", "Non sono state trovate soluzioni valide...\n");
	}
	else {
		fprintf(stdout, "La soluzione ottima trovata ha utilita' complessiva %d ed e' la seguente: \n", utTot);
		stampaSol();
	}
	fprintf(stdout, "%s", "\n");

	return;
}

void combSem(int pos, int start, int areaInvLib, int utTotCorr) {
	int i;
	int cScelte;

	for (i = start; i < nOgg; i++) {
		if (vScelte[i].nr <= nrInv && vScelte[i].nc <= ncInv && areaInvLib - vScelte[i].area >= 0) {
			vSolCorr[pos].idOgg = i;
			combSem(pos + 1, i + 1, areaInvLib - vScelte[i].area, utTotCorr + vScelte[i].utilità);
		}
	}
	if (utTotCorr > utTot) {
		if (pos < nOgg) {
			vSolCorr[pos].idOgg = -1;
		}
		if (oggRichiesti() && disponiOgg()) {
			utTot = utTotCorr;
			for (cScelte = 0; cScelte < nOgg && vOrdinato[cScelte] != -1; cScelte++) {
				vSol[cScelte] = vSolCorr[vOrdinato[cScelte]];
			}
			if (cScelte < nOgg) {
				vSol[cScelte].idOgg = -1;
			}
		}
	}

	return;
}

int oggRichiesti(void) {
	int arma, armatura, pozione;
	int cScelte;

	arma = armatura = pozione = 0;
	for (cScelte = 0; cScelte < nOgg && vSolCorr[cScelte].idOgg != -1; cScelte++) {
		if (strcmp(vScelte[vSolCorr[cScelte].idOgg].categoria, "arma") == 0) {
			arma = 1;
		}
		else {
			if (strcmp(vScelte[vSolCorr[cScelte].idOgg].categoria, "armatura") == 0) {
				armatura = 1;
			}
			else {
				if (strcmp(vScelte[vSolCorr[cScelte].idOgg].categoria, "pozione") == 0) {
					pozione = 1;
				}
			}
		}
	}
	if (arma && armatura && pozione) {
		return 1;
	}

	return 0;
}

int disponiOgg(void) {
	int cScelte;
	int crInv, crLib;
	int ccInv, ccLib;
	int trovato, spazioAdeguato;

	for (cScelte = 0; cScelte < nOgg && vSolCorr[cScelte].idOgg != -1; cScelte++) {
		vOrdinato[cScelte] = cScelte;
	}
	if (cScelte < nOgg) {
		vOrdinato[cScelte] = -1;
	}
	ordinaSolAreaDecQS(cScelte);

	trovato = 1;
	for (cScelte = 0; cScelte < nOgg && vOrdinato[cScelte] != -1 && trovato; cScelte++) {
		trovato = 0;
		for (crInv = 0; crInv < nrInv - vScelte[vSolCorr[vOrdinato[cScelte]].idOgg].nr + 1 && !trovato; crInv++) {
			for (ccInv = 0; ccInv < ncInv - vScelte[vSolCorr[vOrdinato[cScelte]].idOgg].nc + 1 && !trovato; ccInv++) {
				if (inv[crInv][ccInv] == -1) {
					spazioAdeguato = 1;
					for (crLib = 0; crInv + crLib < nrInv && inv[crInv + crLib][ccInv] == -1 && spazioAdeguato; crLib++) {
						for (ccLib = 0; ccInv + ccLib < ncInv && inv[crInv][ccInv + ccLib] == -1; ccLib++);	
						if (ccLib < vScelte[vSolCorr[vOrdinato[cScelte]].idOgg].nc) {
							spazioAdeguato = 0;
						}
					}
					if (!spazioAdeguato) {
						crLib--;
					}
					if (crLib >= vScelte[vSolCorr[vOrdinato[cScelte]].idOgg].nr) {
						trovato = 1;
						vSolCorr[vOrdinato[cScelte]].r = crInv;
						vSolCorr[vOrdinato[cScelte]].c = ccInv;
						while (crInv < vSolCorr[vOrdinato[cScelte]].r + vScelte[vSolCorr[vOrdinato[cScelte]].idOgg].nr) {
							while (ccInv < vSolCorr[vOrdinato[cScelte]].c + vScelte[vSolCorr[vOrdinato[cScelte]].idOgg].nc) {
								inv[crInv][ccInv] = vSolCorr[vOrdinato[cScelte]].idOgg;
								ccInv++;
							}
							crInv++;
							ccInv = vSolCorr[vOrdinato[cScelte]].c;
						}
					}
				}
			}
		}
	}

	initInv();
	if (!trovato) {
		return 0;
	}

	return 1;
}

//ORDINAMENTO MODIFICA VETTORE!
void ordinaSolAreaDecQS(int lSol) {
	int l, r;

	l = 0;
	r = lSol - 1;
	quickSortR(l, r);

	return;
}

void quickSortR(int l, int r) {
	int q;

	if (l >= r) {
		return;
	}
	q = partition(l, r);
	quickSortR(l, q - 1);
	quickSortR(q + 1, r);

	return;
}

int partition(int l, int r) {
	int i, j;
	t_scelta x;
	int t;

	i = l - 1;
	j = r;
	x = vSolCorr[vOrdinato[r]];
	for (; ;) {
		while (vScelte[vSolCorr[vOrdinato[++i]].idOgg].area > vScelte[x.idOgg].area);
		while (vScelte[vSolCorr[vOrdinato[--j]].idOgg].area < vScelte[x.idOgg].area) {
			if (j == l) {
				break;
			}
		}
		if (i >= j) {
			break;
		}
		t = vOrdinato[i];
		vOrdinato[i] = vOrdinato[j];
		vOrdinato[j] = t;
	}
	t = vOrdinato[i];
	vOrdinato[i] = vOrdinato[r];
	vOrdinato[r] = t;

	return i;
}

void stampaSol(void) {
	int cScelte;

	for (cScelte = 0; cScelte < nOgg && vSol[cScelte].idOgg != -1; cScelte++) {
		fprintf(stdout, "- %s con angolo in alto a sinistra in posizione %d %d;\n", vScelte[vSol[cScelte].idOgg].nome, vSol[cScelte].r, vSol[cScelte].c);
	}

	return;
}

void terminaOtt(void) {
	distruggiInv();
	distruggiVetScelte();
	distruggiVetSol();

	return;
}

void distruggiInv(void) {
	int crInv;

	for (crInv = 0; crInv < nrInv; crInv++) {
		free(inv[crInv]);
	}
	free(inv);

	return;
}

void distruggiVetScelte(void) {
	free(vScelte);

	return;
}

void distruggiVetSol(void) {
	free(vSol);
	free(vSolCorr);
	free(vOrdinato);

	return;
}