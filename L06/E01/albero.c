#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXS 11 /* 10 + 1 per '\0' */

// Definizione delle strutture dati usate
typedef struct nodo* link;

struct nodo {
  char val[MAXS];
  link left;
  link right;
};

// Prototipi delle funzioni di utilità
link new_nodo(char *val);
link leggi_nodo(FILE *in);
link leggi_albero(char *filename);
void libera_albero(link root);
void display_albero(link root);

// Prototipi delle funzioni da implementare
int distanza(link n, char *k1, char *k2);
int contaCompleti(link n, int L1, int L2);
// Funzione aggiuntiva necessaria per il calcolo dei nodi con due figli. La contaCompleti è usata come wrapper.
int contaCompletiR(int pos, link n, int L1, int L2);
// Funzione aggiuntiva che uso solo per garantire il funzionamento della getchar finale.
void svuotaBufStdin(void);

int main(int argc, char **argv) {
  link root;

  // Variabili introdotte per poter invocare le funzioni distanza e contaCompleti:
  char k1[MAXS], k2[MAXS];
  int L1, L2;
  int dist, numCompleti;

  root = leggi_albero(argv[1]);
  display_albero(root);

  // invocazione delle funzioni richieste
  fprintf(stdout, "%s", "\nInserire la prima delle due stringhe tra cui calcolare la distanza: ");
  gets(k1);
  fprintf(stdout, "%s", "Inserire la seconda delle due stringhe tra cui calcolare la distanza: ");
  gets(k2);
  dist = distanza(root, k1, k2);
  if (dist == 0) {
	  fprintf(stdout, "%s", "Le due stringhe coincidono o nessuna delle due e' presente nell'albero.\n");
  }
  if (dist < 0) {
	  fprintf(stdout, "%s", "Una sola delle due stringhe e' presente nell'albero.\n");
  }
  if (dist > 0) {
	  fprintf(stdout, "Il numero di archi tra i nodi contenenti le due stringhe e' pari a %d\n", dist);
  }

  fprintf(stdout, "%s", "\nInserire il primo dei due livelli tra cui cercare nodi con due figli: ");
  fscanf(stdin, "%d", &L1);
  fprintf(stdout, "%s", "Inserire il secondo dei due livelli tra cui cercare nodi con due figli: ");
  fscanf(stdin, "%d", &L2);
  numCompleti = contaCompleti(root, L1, L2);
  fprintf(stdout, "Il numero di nodi con due figli tra il livello %d e il livello %d e' %d\n", L1, L2, numCompleti);

  svuotaBufStdin();
  getchar();

  libera_albero(root);
  return 0;
}

// Implementazione delle funzioni di utilità

link new_nodo(char *val) {
    link n = malloc(1*sizeof(*n));
    strcpy (n->val,val);
    n->left = NULL;
    n->right = NULL;
    return n;
};


void display_albero(link root) {
    if (root == NULL) return;
    printf("nodo %s\n", root->val);
    display_albero(root->left);
    display_albero(root->right);
}

void libera_albero(link root) {
    if (root == NULL) return;
    libera_albero(root->left);
    libera_albero(root->right);
    free(root);
}

link leggi_nodo(FILE *in) {
    char val[MAXS];
    int l, r;
    if (fscanf(in, "%s %d %d", val, &l, &r) != 3) return NULL;
    link n = new_nodo(val);
    if (l!=0) n->left = leggi_nodo(in);
    if (r!=0) n->right = leggi_nodo(in);
    return n;
}

link leggi_albero(char *filename) {
    FILE *in;
	in = fopen(filename, "r");
    link root = leggi_nodo(in);
    fclose(in);
    return root;
}

// Implementazione delle funzioni da aggiungere
int distanza(link n, char *k1, char *k2) {
	int contaSx, contaDx;
	int contaTot;
	int confK1, confK2;

	if (n != NULL) {
		confK1 = strcmp(n->val, k1);
		confK2 = strcmp(n->val, k2);
		if (confK1 >= 0 && confK2 >= 0) {
			contaDx = 0;
			contaSx = contaTot = distanza(n->left, k1, k2);
		}
		else {
			if (confK1 <= 0 && confK2 <= 0) {
				contaSx = 0;
				contaDx = contaTot = distanza(n->right, k1, k2);
			}
			else {
				contaSx = distanza(n->left, k1, k2);
				contaDx = distanza(n->right, k1, k2);
				contaTot = contaSx + contaDx;
			}
		}

		if (confK1 == 0 || confK2 == 0) {
			// Se k1 e k2 sono la stessa stringa, distano sicuramente 0 archi.
			if (confK1 == confK2) {
				return 0;
			}
			if (contaTot == 0) {
				return -1;
			}
			if (contaTot < 0) {
				return -contaTot;
			}
		}
		else {
			if (contaTot == 0) {
				return 0;
			}
			if (contaTot > 0) {
				return contaTot;
			}
			if (contaSx == 0 || contaDx == 0) {
				return contaTot - 1;
			}
			if (contaTot < 0) {
				return -contaTot;
			}
		}
	}
	
	return 0;
}

int contaCompleti(link n, int L1, int L2) {
	int contaTot;

	return contaCompletiR(1, n, L1, L2);
}

int contaCompletiR(int pos, link n, int L1, int L2) {
	int contaTot;

	contaTot = 0;
	if (pos > L2 || n == NULL) {
		return 0;
	}
	contaTot = contaCompletiR(pos + 1, n->left, L1, L2) + contaCompletiR(pos + 1, n->right, L1, L2);
	if (pos >= L1 && n->left != NULL && n->right != NULL) {
		contaTot++;
	}

	return contaTot;
}

void svuotaBufStdin(void) {
	char fineBuf;

	do {
		fscanf(stdin, "%c", &fineBuf);
	} while (fineBuf != '\n');

	return;
}