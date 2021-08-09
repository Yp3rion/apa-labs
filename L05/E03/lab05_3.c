// LABORATORIO 5 ESERCIZIO 3
// DATA DI REALIZZAZIONE: 7/11/2016

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LUNGDATA 10
#define LUNGCOD 9
#define MAXSTR 25
#define NOMEINPUT "catalogo.txt"
#define NUMCOMANDI 8
#define MAXCOMANDO 9

typedef enum {
	e_catalogoV,
	e_catalogoF,
	e_categoria,
	e_aggiorna,
	e_cercaC,
	e_cercaN,
	e_inserisci,
	e_cancella,
	e_fine
}t_comando;

typedef struct {
	char *codice, *nome, *categoria;
	float prezzo;
	int rimasti;
	char *data;
	int cancellato;
} t_prodotto;

int acquisisciVettProd(t_prodotto** ptr_vett, int* ptr_lung);
int leggiVettore(FILE *input, t_prodotto vett[], int lung);
int allocaStringheProd(t_prodotto* ptr_prod, int l_cod, int l_nom, int l_cat, int l_data);
int gestisciOrd(t_prodotto ***p_ordPrezA, t_prodotto ***p_ordPrezD, t_prodotto ***p_ordCod, t_prodotto ***p_ordNom, t_prodotto ***p_ordCate, t_prodotto catalogo[], int l_vett, int maxvett);
void stampaMenu(void);
t_comando leggiComando(void);
int eseguiComando(t_comando comando, t_prodotto **p_vett, t_prodotto ***p_ordPrezA, t_prodotto ***p_ordPrezD[], t_prodotto ***p_ordCod, t_prodotto ***p_ordNom, t_prodotto ***p_ordCate, int *l_vett, int *maxvett);
void stampaCatalogo(FILE *output, t_prodotto vett[], int l_vett);
void ordinaCatPrez(t_prodotto *vett[], int l_vett, int modo);
void ordinaCatCod(t_prodotto *vett[], int l_vett);
void ordinaCatNom(t_prodotto *vett[], int l_vett);
void ordinaCatCate(t_prodotto *vett[], int l_vett);
void stampaPerCate(t_prodotto *ordCate[], int l_vett);
void gestisciAggiornamenti(t_prodotto *ordCod[], int l_vett);
void gestisciCercaCod(t_prodotto *ordCod[], int l_vett);
int cercaCodBin(char *codCercato, t_prodotto *ordCod[], int l_vett);
void gestisciCercaNom(t_prodotto *ordNom[], int l_vett);
int cercaNomBin(char *nomCercato, t_prodotto *ordNom[], int l_vett);
int gestisciInserimento(t_prodotto ***p_ordPrezA, t_prodotto ***p_ordPrezD, t_prodotto ***p_ordCod, t_prodotto ***p_ordNom, t_prodotto ***p_ordCate, t_prodotto **p_vett, int l_vett, int *maxvett);
int creaNuovoEl(t_prodotto *p_el);
void gestisciCancella(t_prodotto *ordCod[], int l_vett);
void svuotaBufStdin(void);

int main(void) {
	t_prodotto *vett, **ordPrezA, **ordPrezD, **ordCod, **ordNom, **ordCate;
	t_comando comando;
	int l_vett, maxvett;

	switch (acquisisciVettProd(&vett, &l_vett)) {
	case -1:
		fprintf(stdout, "Il file %s non e' stato aperto con successo.\n", NOMEINPUT);
		getchar();
		return EXIT_FAILURE;
	case 0:
		fprintf(stdout, "L'allocazione in memoria del catalogo non e' stata eseguita con successo.\n");
		getchar();
		return EXIT_FAILURE;
	}

	maxvett = l_vett;

	if (gestisciOrd(&ordPrezA, &ordPrezD, &ordCod, &ordNom, &ordCate, vett, l_vett, maxvett) == 0) {
		fprintf(stdout, "%s", "L'allocazione in memoria di uno dei vettori di puntatori agli elementi del catalogo e' fallita.\n");
		getchar();
		return EXIT_FAILURE;
	}

	stampaMenu();
	comando = leggiComando();
	while (comando != e_fine) {
		if (eseguiComando(comando, &vett, &ordPrezA, &ordPrezD, &ordCod, &ordNom, &ordCate, &l_vett, &maxvett) == 0) {
			fprintf(stdout, "%s", "E' stato riscontrato un errore durante l'esecuzione del comando.\n");
			getchar();
			return EXIT_FAILURE;
		}
		stampaMenu();
		comando = leggiComando();
	}

	fprintf(stdout, "%s", "Operazione terminata.\n");
	getchar();

	return EXIT_SUCCESS;
}

// Funzione che si occupa di allocare dinamicamente e riempire
// il vettore di strutture. Riceve dal main il puntatore al
// puntatore a cui si assegnerà la malloc e il puntatore alla variabile
// che dovrà contenere le dimensioni del vettore, restituisce -1 se 
// fallisce l'apertura del file di input, 0 se fallisce l'allocazione,
// 1 altrimenti.
int acquisisciVettProd(t_prodotto** ptr_vett, int* l_vett) {
	FILE *input;
	t_prodotto* vett;

	if ((input = fopen(NOMEINPUT, "r")) == NULL) {
		return -1;
	}

	fscanf(input, "%d", l_vett);
	if ((vett = (t_prodotto *)malloc(*l_vett * sizeof(*vett))) == NULL) {
		return 0;
	}

	if (leggiVettore(input, vett, *l_vett) == 0) {
		return 0;
	}
	*ptr_vett = vett;

	return 1;
}

// Funzione che legge riga per riga il catalogo vero e proprio, inserisce i 
// dati appena letti in variabili temporanee ed infine alloca dinamicamente 
// le stringhe e copia i dati letti nella struct del vettore attualmente considerata.
// Restituisce 0 se l'allocazione delle stringhe fallisce, altrimenti restituisce 1.
int leggiVettore(FILE *input, t_prodotto vett[], int lung) {
	char cod[LUNGCOD + 1], nome[MAXSTR + 1], cat[MAXSTR + 1];
	float prezzo;
	int rimasti;
	char data[LUNGDATA + 1];
	int c_vett;

	for (c_vett = 0; c_vett < lung; c_vett++) {
		fscanf(input, "%s %s %s %f %d %s", cod, nome, cat, &prezzo, &rimasti, data);
		if (allocaStringheProd(&vett[c_vett], LUNGCOD + 1, strlen(nome) + 1, strlen(cat) + 1, LUNGDATA + 1) == 0) {
			return 0;
		}
		strcpy(vett[c_vett].codice, cod);
		strcpy(vett[c_vett].nome, nome);
		strcpy(vett[c_vett].categoria, cat);
		vett[c_vett].prezzo = prezzo;
		vett[c_vett].rimasti = rimasti;
		strcpy(vett[c_vett].data, data);
		vett[c_vett].cancellato = 0;
	}

	return 1;
}

// Funzione che riceve il puntatore alla struct in cui allocare le stringhe e le lunghezze
// da allocare ed esegue l'operazione di allocazione dinamica. Restituisce 0 se l'allocazione
// fallisce, altrimenti 1.
int allocaStringheProd(t_prodotto* ptr_prod, int l_cod, int l_nom, int l_cat, int l_data) {
	char *ptr_cod, *ptr_nom, *ptr_cat, *ptr_data;

	if ((ptr_cod = (char *)malloc(l_cod * sizeof(*ptr_cod))) == NULL) {
		return 0;
	}
	if ((ptr_nom = (char *)malloc(l_nom * sizeof(*ptr_nom))) == NULL) {
		return 0;
	}
	if ((ptr_cat = (char *)malloc(l_cat * sizeof(*ptr_cat))) == NULL) {
		return 0;
	}
	if ((ptr_data = (char *)malloc(l_data * sizeof(*ptr_data))) == NULL) {
		return 0;
	}

	ptr_prod->codice = ptr_cod;
	ptr_prod->nome = ptr_nom;
	ptr_prod->categoria = ptr_cat;
	ptr_prod->data = ptr_data;

	return 1;
}

// Funzione che alloca dinamicamente i vettori di puntatori a t_prodotto che riceve
// by reference, inizializza tali vettori facendo puntare i loro elementi agli elementi
// corrispondenti del vettore di t_prodotto contenente il catalogo, infine esegue i
// veri e propri ordinamenti sui vettori di puntatori in modo opportuno.
int gestisciOrd(t_prodotto ***p_ordPrezA, t_prodotto ***p_ordPrezD, t_prodotto ***p_ordCod, t_prodotto ***p_ordNom, t_prodotto ***p_ordCate, t_prodotto catalogo[], int l_vett, int maxvett) {
	t_prodotto **ordPrezA, **ordPrezD, **ordCod, **ordNom, **ordCate;
	int c_vett;

	if ((ordPrezA = (t_prodotto**)malloc(maxvett * sizeof(*ordPrezA))) == NULL) {
		return 0;
	}
	if ((ordPrezD = (t_prodotto**)malloc(maxvett * sizeof(*ordPrezD))) == NULL) {
		return 0;
	}
	if ((ordCod = (t_prodotto**)malloc(maxvett * sizeof(*ordCod))) == NULL) {
		return 0;
	}
	if ((ordNom = (t_prodotto**)malloc(maxvett * sizeof(*ordNom))) == NULL) {
		return 0;
	}
	if ((ordCate = (t_prodotto**)malloc(maxvett * sizeof(*ordCate))) == NULL) {
		return 0;
	}

	for (c_vett = 0; c_vett < l_vett; c_vett++) {
		ordPrezA[c_vett] = ordPrezD[c_vett] = ordCod[c_vett] = ordNom[c_vett] = ordCate[c_vett] = &catalogo[c_vett];
	}

	ordinaCatPrez(ordPrezA, l_vett, 0);
	ordinaCatPrez(ordPrezD, l_vett, 1);
	ordinaCatCod(ordCod, l_vett);
	ordinaCatNom(ordNom, l_vett);
	ordinaCatCate(ordCate, l_vett);

	*p_ordPrezA = ordPrezA;
	*p_ordPrezD = ordPrezD;
	*p_ordCod = ordCod;
	*p_ordNom = ordNom;
	*p_ordCate = ordCate;

	return 1;
}

// Funzione che stampa a video il menù principale del programma, che elenca le
// possibili opzioni.
void stampaMenu(void) {
	fprintf(stdout, "%s", "Scegliere una delle seguenti opzioni (case sensitive, qualsiasi altro comando termina il programma): \n");
	fprintf(stdout, "%s", "- per stampare a video i contenuti del catalogo, scrivere \"catalogoV\";\n");
	fprintf(stdout, "%s", "- per stampare su un file i contenuti del catalogo, scrivere \"catalogoF\";\n");
	fprintf(stdout, "%s", "- per stampare a video i prodotti divisi per categoria, scrivere \"categoria\";\n");
	fprintf(stdout, "%s", "- per aggiornare la quantita' disponibile di un prodotto, scrivere \"aggiorna\";\n");
	fprintf(stdout, "%s", "- per cercare un prodotto per codice, scrivere \"cercaC\";\n");
	fprintf(stdout, "%s", "- per cercare un prodotto per nome, scrivere \"cercaN\";\n");
	fprintf(stdout, "%s", "- per inserire un nuovo prodotto, scrivere \"inserisci\";\n");
	fprintf(stdout, "%s", "- per cancellare un prodotto dalla lista, scrivere \"cancella\";\n");
	fprintf(stdout, "%s", "Scelta: ");

	return;
}

// Funzione che legge da tastiera il comando dell'utente e lo interpreta, 
// restituendo al main il corrispondente valore dell'enum di tipo t_comando.
t_comando leggiComando(void) {
	char tabCom[NUMCOMANDI][MAXCOMANDO + 1] = { "catalogoV", "catalogoF", "categoria", "aggiorna", "cercaC", "cercaN", "inserisci", "cancella" };
	char comLetto[MAXCOMANDO + 1];
	t_comando comando;

	gets(comLetto);

	for (comando = e_catalogoV; strcmp(tabCom[comando], comLetto) != 0 && comando < NUMCOMANDI; comando++);

	return comando;
}

// Funzione che riceve il comando letto da leggiComando, il puntatore al vettore contenente 
// il catalogo, i puntatori ai vari vettori di puntatori a t_prodotto che
// contengono gli ordinamenti ed infine la lunghezza del catalogo. Questa funzione
// associa ogni valore di comando a una determinata operazione; restituisce 0 se qualche 
// operazione fallisce, altrimenti 1.
int eseguiComando(t_comando comando, t_prodotto **p_vett, t_prodotto ***p_ordPrezA, t_prodotto ***p_ordPrezD[], t_prodotto ***p_ordCod, t_prodotto ***p_ordNom, t_prodotto ***p_ordCate, int *l_vett, int *maxvett) {
	char nomeFile[FILENAME_MAX + 1];
	FILE *output;

	switch (comando) {
	case e_catalogoV:
		fprintf(stdout, "%s", "\n");
		stampaCatalogo(stdout, *p_vett, *l_vett);
		fprintf(stdout, "%s", "\n");
		break;
	case e_catalogoF:
		fprintf(stdout, "%s", "\n");
		fprintf(stdout, "%s", "Inserire il nome del file su cui stampare il catalogo: ");
		fscanf(stdin, "%s", nomeFile);
		if ((output = fopen(nomeFile, "w")) == NULL) {
			return 0;
		}
		stampaCatalogo(output, *p_vett, *l_vett);
		svuotaBufStdin();
		fprintf(stdout, "%s", "\n");
		break;
	case e_categoria:
		fprintf(stdout, "%s", "\n");
		stampaPerCate(*p_ordCate, *l_vett);
		break;
	case e_aggiorna:
		fprintf(stdout, "%s", "\n");
		gestisciAggiornamenti(*p_ordCod, *l_vett);
		svuotaBufStdin();
		fprintf(stdout, "%s", "\n");
		break;
	case e_cercaC:
		fprintf(stdout, "%s", "\n");
		gestisciCercaCod(*p_ordCod, *l_vett);
		svuotaBufStdin();
		fprintf(stdout, "%s", "\n");
		break;
	case e_cercaN:
		fprintf(stdout, "%s", "\n");
		gestisciCercaNom(*p_ordNom, *l_vett);
		svuotaBufStdin();
		fprintf(stdout, "%s", "\n");
		break;
	case e_inserisci:
		fprintf(stdout, "%s", "\n");
		(*l_vett)++;
		if (gestisciInserimento(p_ordPrezA, p_ordPrezD, p_ordCod, p_ordNom, p_ordCate, p_vett, *l_vett, maxvett) == 0) {
			return 0;
		}
		svuotaBufStdin();
		fprintf(stdout, "%s", "\n");
		break;
	case e_cancella:
		fprintf(stdout, "%s", "\n");
		gestisciCancella(*p_ordCod, *l_vett);
		svuotaBufStdin();
		fprintf(stdout, "%s", "\n");
		break;
	}

	return 1;
}

// Funzione che stampa gli elemenenti di vett, vettore di t_prodotto 
// di lunghezza l_vett, sul file puntato da output.
void stampaCatalogo(FILE *output, t_prodotto vett[], int l_vett) {
	int c_vett;

	for (c_vett = 0; c_vett < l_vett; c_vett++) {
		if (!vett[c_vett].cancellato) {
			fprintf(output, "%s %s %s %.2f %d %s\n", vett[c_vett].codice, vett[c_vett].nome, vett[c_vett].categoria, vett[c_vett].prezzo, vett[c_vett].rimasti, vett[c_vett].data);
		}
	}

	return;
}

// Funzione che ordina per prezzi con algoritmo shell sort il 
// vettore di puntatori a struct t_prodotto vett, di lunghezza l_vett,
// in ordine ascendente o discendente  a seconda del valore di modo 
// (0 ascendente, 1 discendente).
void ordinaCatPrez(t_prodotto *vett[], int l_vett, int modo) {
	t_prodotto *tmp;
	int h, c_selez, c_prec;

	h = 1;
	while (h < l_vett / 3) {
		h = 3 * h + 1;
	}

	while (h >= 1) {
		for (c_selez = h; c_selez < l_vett; c_selez++) {
			tmp = vett[c_selez];
			if (modo == 0) {
				for (c_prec = c_selez; c_prec >= h && tmp->prezzo < vett[c_prec - h]->prezzo; c_prec -= h) {
					vett[c_prec] = vett[c_prec - h];
				}
			}
			if (modo == 1) {
				for (c_prec = c_selez; c_prec >= h && tmp->prezzo > vett[c_prec - h]->prezzo; c_prec -= h) {
					vett[c_prec] = vett[c_prec - h];
				}
			}
			vett[c_prec] = tmp;
		}
		h = h / 3;
	}

	return;
}

// Funzione che ordina per codici con algoritmo shell sort il 
// vettore di puntatori a struct t_prodotto vett, di lunghezza l_vett.
void ordinaCatCod(t_prodotto *vett[], int l_vett) {
	t_prodotto *tmp;
	int h, c_selez, c_prec;

	h = 1;
	while (h < l_vett / 3) {
		h = 3 * h + 1;
	}

	while (h >= 1) {
		for (c_selez = h; c_selez < l_vett; c_selez++) {
			tmp = vett[c_selez];
			for (c_prec = c_selez; c_prec >= h && strcmp(tmp->codice, vett[c_prec - h]->codice) < 0; c_prec -= h) {
				vett[c_prec] = vett[c_prec - h];
			}
			vett[c_prec] = tmp;
		}
		h = h / 3;
	}

	return;
}

// Funzione che ordina per nomi con algoritmo shell sort il 
// vettore di puntatori a struct t_prodotto vett, di lunghezza l_vett.
void ordinaCatNom(t_prodotto *vett[], int l_vett) {
	t_prodotto *tmp;
	int h, c_selez, c_prec;

	h = 1;
	while (h < l_vett / 3) {
		h = 3 * h + 1;
	}

	while (h >= 1) {
		for (c_selez = h; c_selez < l_vett; c_selez++) {
			tmp = vett[c_selez];
			for (c_prec = c_selez; c_prec >= h && strcmp(tmp->nome, vett[c_prec - h]->nome) < 0; c_prec -= h) {
				vett[c_prec] = vett[c_prec - h];
			}
			vett[c_prec] = tmp;
		}
		h = h / 3;
	}

	return;
}

// Funzione che ordina per categorie con algoritmo shell sort il 
// vettore di puntatori a struct t_prodotto vett, di lunghezza l_vett.
void ordinaCatCate(t_prodotto *vett[], int l_vett) {
	t_prodotto *tmp;
	int h, c_selez, c_prec;

	h = 1;
	while (h < l_vett / 3) {
		h = 3 * h + 1;
	}

	while (h >= 1) {
		for (c_selez = h; c_selez < l_vett; c_selez++) {
			tmp = vett[c_selez];
			for (c_prec = c_selez; c_prec >= h && strcmp(tmp->categoria, vett[c_prec - h]->categoria) < 0; c_prec -= h) {
				vett[c_prec] = vett[c_prec - h];
			}
			vett[c_prec] = tmp;
		}
		h = h / 3;
	}

	return;
}

// Funzione che scorre il vettore di puntatori a struct t_prodotto
// ordCate, di lunghezza l_vett, già ordinato per categorie, 
// e stampa a video i prodotti suddivisi per categorie.
void stampaPerCate(t_prodotto *ordCate[], int l_vett) {
	char cateAtt[MAXSTR + 1] = "";
	int c_vett;

	c_vett = 0;
	while (c_vett < l_vett) {
		if (ordCate[c_vett]->cancellato) {
			c_vett++;
		}
		else {
			if (strcmp(cateAtt, ordCate[c_vett]->categoria) != 0) {
				strcpy(cateAtt, ordCate[c_vett]->categoria);
				fprintf(stdout, "Prodotti appartenenti alla categoria %s: \n", cateAtt);
			}
			while (c_vett < l_vett && strcmp(cateAtt, ordCate[c_vett]->categoria) == 0) {
				if (!ordCate[c_vett]->cancellato) {
					fprintf(stdout, "%s %s %s %.2f %d %s\n", ordCate[c_vett]->codice, ordCate[c_vett]->nome, ordCate[c_vett]->categoria, ordCate[c_vett]->prezzo, ordCate[c_vett]->rimasti, ordCate[c_vett]->data);
				}
				c_vett++;
			}
			fprintf(stdout, "%s", "\n");
		}
	}
	return;
}

void gestisciAggiornamenti(t_prodotto *ordCod[], int l_vett) {
	char codCercato[LUNGCOD + 1];
	int idTrov;
	int newTot;
	char newData[LUNGDATA + 1];

	fprintf(stdout, "%s", "Inserire il codice del prodotto da aggiornare: ");
	fscanf(stdin, "%s", codCercato);
	idTrov = cercaCodBin(codCercato, ordCod, l_vett);
	if (idTrov == -1) {
		fprintf(stdout, "%s", "Il codice richiesto non e' stato trovato.\n");
	}
	else {
		fprintf(stdout, "%s", "Inserire il nuovo numero di unita' disponibili: ");
		fscanf(stdin, "%d", &newTot);
		fprintf(stdout, "%s", "Inserire la data di oggi (formato: gg/mm/aaaa): ");
		fscanf(stdin, "%s", newData);
		ordCod[idTrov]->rimasti = newTot;
		strcpy(ordCod[idTrov]->data, newData);
	}

	return;
}

// Funzione che si occupa di chiedere il codice da cercare,
// chiamare la cercaCodBin e stampare i suoi risultati.
void gestisciCercaCod(t_prodotto *ordCod[], int l_vett) {
	char codCercato[LUNGCOD + 1];
	int idTrov;

	fprintf(stdout, "%s", "Inserire il codice da cercare: ");
	fscanf(stdin, "%s", codCercato);
	idTrov = cercaCodBin(codCercato, ordCod, l_vett);
	if (idTrov == -1) {
		fprintf(stdout, "%s", "Il codice richiesto non e' stato trovato.\n");
	}
	else {
		fprintf(stdout, "%s", "Il codice richiesto e' stato trovato. I dati relativi sono i seguenti: \n");
		fprintf(stdout, "%s %s %s %.2f %d %s\n", ordCod[idTrov]->codice, ordCod[idTrov]->nome, ordCod[idTrov]->categoria, ordCod[idTrov]->prezzo, ordCod[idTrov]->rimasti, ordCod[idTrov]->data);
	}

	return;
}

// Funzione che cerca un determinato codice nel catalogo
// con algoritmo binario. Restituisce -1 se il codice non viene
// trovato, altrimenti restituisce l'indice del prodotto
// corrispondente nel vettore di puntatori ordinato in base
// ai nomi.
int cercaCodBin(char *codCercato, t_prodotto *ordCod[], int l_vett) {
	int m, l, r;

	l = 0;
	r = l_vett - 1;
	while (l <= r) {
		m = l + (r - l) / 2;
		if (strcmp(ordCod[m]->codice, codCercato) == 0 && !ordCod[m]->cancellato) {
			return m;
		}
		if (strcmp(ordCod[m]->codice, codCercato) < 0) {
			l = m + 1;
		}
		else {
			r = m - 1;
		}

	}

	return -1;
}

// Funzione che si occupa di chiedere il nome da cercare, chiamare la
// cercaNomBin e stampare il suo risultato.
void gestisciCercaNom(t_prodotto *ordNom[], int l_vett) {
	char nomCercato[MAXSTR + 1];
	int idTrov;

	fprintf(stdout, "%s", "Inserire il nome da cercare: ");
	fscanf(stdin, "%s", nomCercato);
	idTrov = cercaNomBin(nomCercato, ordNom, l_vett);
	if (idTrov == -1) {
		fprintf(stdout, "%s", "Il nome richiesto non e' stato trovato.\n");
	}
	else {
		fprintf(stdout, "%s", "Il nome richiesto e' stato trovato. I dati relativi sono i seguenti: \n");
		fprintf(stdout, "%s %s %s %.2f %d %s\n", ordNom[idTrov]->codice, ordNom[idTrov]->nome, ordNom[idTrov]->categoria, ordNom[idTrov]->prezzo, ordNom[idTrov]->rimasti, ordNom[idTrov]->data);
	}

	return;
}

// Funzione che cerca un determinato codice nel catalogo
// con algoritmo binario. Restituisce -1 se il codice non viene
// trovato, altrimenti restituisce l'indice del prodotto
// corrispondente nel vettore di puntatori ordinato in base
// ai codici.
int cercaNomBin(char *nomCercato, t_prodotto *ordNom[], int l_vett) {
	int m, l, r;
	int l_cercato;

	l = 0;
	r = l_vett - 1;
	l_cercato = strlen(nomCercato);
	while (l <= r) {
		m = l + (r - l) / 2;
		if (strncmp(ordNom[m]->nome, nomCercato, l_cercato) == 0 && !ordNom[m]->cancellato) {
			return m;
		}
		if (strncmp(ordNom[m]->nome, nomCercato, l_cercato) < 0) {
			l = m + 1;
		}
		else {
			r = m - 1;
		}

	}

	return -1;
}

// Funzione che libera la memoria occupata dai vettori di puntatori, rialloca
// il vettore di struct aggiungendo posto per il nuovo elemento, quindi chiama
// la gestisciOrd per ricreare ad hoc i vettori di puntatori e riordinarli.
// Restituisce 0 se qualche allocazione fallisce, altrimenti restituisce 1.
int gestisciInserimento(t_prodotto ***p_ordPrezA, t_prodotto ***p_ordPrezD, t_prodotto ***p_ordCod, t_prodotto ***p_ordNom, t_prodotto ***p_ordCate, t_prodotto **p_vett, int l_vett, int *maxvett) {
	if (l_vett > *maxvett) {
		free(*p_ordPrezA);
		free(*p_ordPrezD);
		free(*p_ordCod);
		free(*p_ordNom);
		free(*p_ordCate);
		
		*maxvett *= 2;

		if ((*p_vett = realloc(*p_vett, *maxvett * sizeof(**p_vett))) == NULL) {
			return 0;
		}

		if (creaNuovoEl(&(*p_vett)[l_vett - 1]) == 0) {
			return 0;
		}

		if ((gestisciOrd(p_ordPrezA, p_ordPrezD, p_ordCod, p_ordNom, p_ordCate, *p_vett, l_vett, *maxvett)) == 0) {
			return 0;
		}
	}
	else {
		(*p_ordPrezA)[l_vett - 1] = (*p_ordPrezD)[l_vett - 1] = (*p_ordCod)[l_vett - 1] = (*p_ordNom)[l_vett - 1] = (*p_ordCate)[l_vett - 1] = &(*p_vett)[l_vett - 1];
		
		if (creaNuovoEl(&(*p_vett)[l_vett - 1]) == 0) {
			return 0;
		}

		ordinaCatPrez(*p_ordPrezA, l_vett, 0);
		ordinaCatPrez(*p_ordPrezD, l_vett, 1);
		ordinaCatCod(*p_ordCod, l_vett);
		ordinaCatNom(*p_ordNom, l_vett);
		ordinaCatCate(*p_ordCate, l_vett);
	}

	return 1;
}

// Funzione che riceve il puntatore p_el all'elemento aggiunto in coda
// al vettore catalogo ed inserisce in tale elemento i dati forniti
// dall'utente. Restituisce 0 se l'allocazione dinamica delle stringhe
// fallisce, altrimenti restituisce 1.
int creaNuovoEl(t_prodotto *p_el) {
	char codice[LUNGCOD + 1], nome[MAXSTR + 1], categoria[MAXSTR + 1], data[LUNGDATA + 1];
	float prezzo;
	int rimasti;

	fprintf(stdout, "%s", "Inserire il codice del prodotto: ");
	fscanf(stdin, "%s", codice);
	fprintf(stdout, "%s", "Inserire il nome del prodotto: ");
	fscanf(stdin, "%s", nome);
	fprintf(stdout, "%s", "Inserire la categoria merceologica del prodotto: ");
	fscanf(stdin, "%s", categoria);
	fprintf(stdout, "%s", "Inserire il prezzo del prodotto: ");
	fscanf(stdin, "%f", &prezzo);
	fprintf(stdout, "%s", "Inserire la quantita' di unita' del prodotto rimaste: ");
	fscanf(stdin, "%d", &rimasti);
	fprintf(stdout, "%s", "Inserire la data di oggi (formato gg/mm/aaaa): ");
	fscanf(stdin, "%s", data);

	if (allocaStringheProd(p_el, LUNGCOD + 1, strlen(nome), strlen(categoria), LUNGDATA + 1) == 0) {
		return 0;
	}
	strcpy(p_el->codice, codice);
	strcpy(p_el->nome, nome);
	strcpy(p_el->categoria, categoria);
	p_el->prezzo = prezzo;
	p_el->rimasti = rimasti;
	strcpy(p_el->data, data);
	p_el->cancellato = 0;


	return 1;
}

// Funzione che si occupa di chiedere il codice del prodotto che
// si vuole cancellare, cercarlo chiamando la cercaCodBin e, in
// caso lo trovi, porre a 1 la sua flag cancellato.
void gestisciCancella(t_prodotto *ordCod[], int l_vett) {
	char codCercato[LUNGCOD + 1];
	int idTrov;

	fprintf(stdout, "%s", "Inserire il codice del prodotto da cancellare: ");
	fscanf(stdin, "%s", codCercato);
	idTrov = cercaCodBin(codCercato, ordCod, l_vett);
	if (idTrov == -1) {
		fprintf(stdout, "%s", "Il codice richiesto non e' stato trovato.\n");
	}
	else {
		ordCod[idTrov]->cancellato = 1;
	}

	return;
}

// Funzione che percorre il buffer di stdin fino all'eventuale '\n'
// (che suppongo essere l'ultimo carattere) e scarta tutti i caratteri 
// in esso. Utile per garantire il funzionamento della getchar finale e della
// gets di leggiComando, che altrimenti assorbirebbero gli whitespace residui ignorati
// dalle varie fscanf.
void svuotaBufStdin(void) {
	char fineBuf;

	do {
		fscanf(stdin, "%c", &fineBuf);
	} while (fineBuf != '\n');

	return;
}