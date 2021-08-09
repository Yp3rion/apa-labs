// LABORATORIO 4 ESERCIZIO 1
// DATA DI REALIZZAZIONE 03/11/2016

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LUNGDATA 10
#define LUNGCOD 9
#define MAXSTR 25
#define NOMEINPUT "catalogo.txt"
#define NUMCOMANDI 10
#define MAXCOMANDO 9

typedef enum {
	e_catalogoV,
	e_catalogoF,
	e_prezzoA,
	e_prezzoD,
	e_codice,
	e_nome,
	e_categoria,
	e_aggiorna,
	e_cercaC,
	e_cercaN,
	e_fine
}t_comando;

typedef enum {
	e_nonOrdinato,
	e_ordinatoCate,
	e_ordinatoPreA,
	e_ordinatoPreD,
	e_ordinatoCod,
	e_ordinatoNom
}t_ordine;

typedef struct {
	char *codice, *nome, *categoria;
	float prezzo;
	int rimasti;
	char *data;
} t_prodotto;

int acquisisciVettProd(t_prodotto** ptr_vett, int* ptr_lung);
int leggiVettore(FILE *input, t_prodotto vett[], int lung);
int allocaStringheProd(t_prodotto* ptr_prod, int l_cod, int l_nom, int l_cat, int l_data);
void stampaMenu(void);
t_comando leggiComando(void);
int eseguiComando(t_comando comando, t_ordine *ptr_ord, t_prodotto vett[], int l_vett);
void stampaCatalogo(FILE *output, t_prodotto vett[], int l_vett);
void ordinaCatPrez(t_prodotto vett[], int l_vett, int modo);
void ordinaCatCod(t_prodotto vett[], int l_vett);
void ordinaCatNom(t_prodotto vett[], int l_vett);
void gestisciElencCateg(t_comando *ptr_comando, t_prodotto vett[], int l_vett);
void ordinaCatCate(t_prodotto vett[], int l_vett);
void stampaPerCate(t_prodotto vett[], int l_vett);
void gestisciAggiornamenti(t_ordine ordine, t_prodotto vett[], int l_vett);
void gestisciCercaCod(t_ordine ordine, t_prodotto vett[], int l_vett);
int cercaCodLin(char *codCercato, t_prodotto vett[], int l_vett);
int cercaCodBin(char *codCercato, t_prodotto vett[], int l_vett);
void gestisciCercaNom(t_ordine ordine, t_prodotto vett[], int l_vett);
int cercaNomLin(char *nomCercato, t_prodotto vett[], int l_vett);
int cercaNomBin(char *nomCercato, t_prodotto vett[], int l_vett);
void svuotaBufStdin(void);

int main(void) {
	t_prodotto *vett;
	t_comando comando;
	t_ordine ordine;
	int l_vett;

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

	ordine = e_nonOrdinato;
	stampaMenu();
	comando = leggiComando();
	while (comando != e_fine) {
		if (eseguiComando(comando, &ordine, vett, l_vett) == 0) {
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

// Funzione che stampa a video il menù principale del programma, che elenca le
// possibili opzioni.
void stampaMenu(void) {
	fprintf(stdout, "%s","Scegliere una delle seguenti opzioni (case sensitive, qualsiasi altro comando termina il programma): \n");
	fprintf(stdout, "%s", "- per stampare a video i contenuti del catalogo, scrivere \"catalogoV\";\n");
	fprintf(stdout, "%s", "- per stampare su un file i contenuti del catalogo, scrivere \"catalogoF\";\n");
	fprintf(stdout, "%s", "- per ordinare il vettore per prezzo ascendente, scrivere \"prezzoA\";\n");
	fprintf(stdout, "%s", "- per ordinare il vettore per prezzo discendente, scrivere \"prezzoD\";\n");
	fprintf(stdout, "%s", "- per ordinare il vettore per codice, scrivere \"codice\";\n");
	fprintf(stdout, "%s", "- per ordinare il vettore per nome, scrivere \"nome\";\n");
	fprintf(stdout, "%s", "- per stampare a video i prodotti divisi per categoria, scrivere \"categoria\";\n");
	fprintf(stdout, "%s", "- per aggiornare la quantita' disponibile di un prodotto, scrivere \"aggiorna\";\n");
	fprintf(stdout, "%s", "- per cercare un prodotto per codice, scrivere \"cercaC\";\n");
	fprintf(stdout, "%s", "- per cercare un prodotto per nome, scrivere \"cercaN\";\n");
	fprintf(stdout, "%s", "Scelta: ");

	return;
}

// Funzione che legge da tastiera il comando dell'utente e lo interpreta, 
// restituendo al main il corrispondente valore dell'enum di tipo t_comando.
t_comando leggiComando(void) {
	char tabCom[NUMCOMANDI][MAXCOMANDO + 1] = { "catalogoV", "catalogoF", "prezzoA", "prezzoD", "codice", "nome", "categoria", "aggiorna", "cercaC", "cercaN" };
	char comLetto[MAXCOMANDO + 1];
	t_comando comando;

	gets(comLetto);

	for (comando = e_catalogoV; strcmp(tabCom[comando], comLetto) != 0 && comando < NUMCOMANDI; comando++);

	return comando;
}

// Funzione che riceve il comando letto da leggiComando, il puntatore alla variabile di tipo
// t_ordine contenuta nel main, il catalogo e la lunghezza del catalogo. Questa funzione
// associa ogni valore di comando a una determinata operazione; restituisce 0 se qualche 
// operazione fallisce, altrimenti 1. Se l'operazione richiesta e' un ordinamento, la funzione
// controlla il valore della variabile puntata da ptr_ord: se il catalogo e' già ordinato nel
// modo giusto, non viene eseguita nessuna operazione e il fatto viene notificato a video, in
// caso contrario si esegue l'ordinamento e si cambia il valore di *ptr_ord in modo opportuno.
// Se l'operazione richiesta e' una ricerca, la funzione usa il valore di *ptr_ord per sapere
// se utilizzare una funzione di ricerca lineare o binaria.
int eseguiComando(t_comando comando, t_ordine *ptr_ord, t_prodotto vett[], int l_vett) {
	char nomeFile[FILENAME_MAX + 1];
	FILE *output;

	switch (comando) {
		case e_catalogoV:
			fprintf(stdout, "%s", "\n");
			stampaCatalogo(stdout, vett, l_vett);
			fprintf(stdout, "%s", "\n");
			break;
		case e_catalogoF:
			fprintf(stdout, "%s", "\n"); 
			fprintf(stdout, "%s", "Inserire il nome del file su cui stampare il catalogo: ");
			fscanf(stdin, "%s", nomeFile);
			if ((output = fopen(nomeFile, "w")) == NULL) {
				return 0;
			}
			stampaCatalogo(output, vett, l_vett);
			svuotaBufStdin();
			fprintf(stdout, "%s", "\n");
			break;
		case e_prezzoA:
			if (*ptr_ord == e_ordinatoPreA) {
				fprintf(stdout, "%s", "Il catalogo e' gia' ordinato in modo ascendente per prezzi.\n");
			}
			else {
				ordinaCatPrez(vett, l_vett, 0);
				*ptr_ord = e_ordinatoPreA;
			}
			fprintf(stdout, "%s", "\n");
			break;
		case e_prezzoD:
			if (*ptr_ord == e_ordinatoPreD) {
				fprintf(stdout, "%s", "Il catalogo e' gia' ordinato in modo discendente per prezzi.\n");
			}
			else {
				ordinaCatPrez(vett, l_vett, 1);
				*ptr_ord = e_ordinatoPreD;
			}
			fprintf(stdout, "%s", "\n");
			break;
		case e_codice:
			if (*ptr_ord == e_ordinatoCod) {
				fprintf(stdout, "%s", "Il catalogo e' gia' ordinato per codici.\n");
			}
			else {
				ordinaCatCod(vett, l_vett);
				*ptr_ord = e_ordinatoCod;
			}
			fprintf(stdout, "%s", "\n");
			break;
		case e_nome:
			if (*ptr_ord == e_ordinatoNom) {
				fprintf(stdout, "%s", "Il catalogo e' gia' ordinato per nomi.\n");
			}
			else {
				ordinaCatNom(vett, l_vett);
				*ptr_ord = e_ordinatoNom;
			}
			fprintf(stdout, "%s", "\n");
			break;
		case e_categoria:
			fprintf(stdout, "%s", "\n");
			gestisciElencCateg(ptr_ord, vett, l_vett);
			break;
		case e_aggiorna:
			fprintf(stdout, "%s", "\n");
			gestisciAggiornamenti(*ptr_ord, vett, l_vett);
			svuotaBufStdin();
			fprintf(stdout, "%s", "\n");
			break;
		case e_cercaC:
			fprintf(stdout, "%s", "\n");
			gestisciCercaCod(*ptr_ord, vett, l_vett);
			svuotaBufStdin();
			fprintf(stdout, "%s", "\n");
			break;
		case e_cercaN:
			fprintf(stdout, "%s", "\n");
			gestisciCercaNom(*ptr_ord, vett, l_vett);
			svuotaBufStdin();
			fprintf(stdout, "%s", "\n");
			break;
	}

	return 1;
}

// Funzione che stampa il catalogo di lunghezza l_vett sul file puntato
// da output.
void stampaCatalogo(FILE *output, t_prodotto vett[], int l_vett) {
	int c_vett;

	for (c_vett = 0; c_vett < l_vett; c_vett++) {
		fprintf(output, "%s %s %s %.2f %d %s\n", vett[c_vett].codice, vett[c_vett].nome, vett[c_vett].categoria, vett[c_vett].prezzo, vett[c_vett].rimasti, vett[c_vett].data);
	}

	return;
}

// Funzione che ordina il catalogo di lunghezza l_vett in base ai 
// prezzi con algoritmo shell sort, in ordine ascendente o discendente 
// a seconda del valore di modo (0 ascendente, 1 discendente).
void ordinaCatPrez(t_prodotto vett[], int l_vett, int modo) {
	t_prodotto tmp;
	int h, c_selez, c_prec;

	h = 1;
	while (h < l_vett / 3) {
		h = 3 * h + 1;
	}

	while (h >= 1) {
		for (c_selez = h; c_selez < l_vett; c_selez++) {
			tmp = vett[c_selez];
			if (modo == 0) {
				for (c_prec = c_selez; c_prec >= h && tmp.prezzo < vett[c_prec - h].prezzo; c_prec -= h) {
					vett[c_prec] = vett[c_prec - h];
				}
			}
			if (modo == 1) {
				for (c_prec = c_selez; c_prec >= h && tmp.prezzo > vett[c_prec - h].prezzo; c_prec -= h) {
					vett[c_prec] = vett[c_prec - h];
				}
			}
			vett[c_prec] = tmp;
		}
		h = h / 3;
	}
		
	return;
}

// Funzione che ordina il catalogo di lunghezza l_vett in base ai 
// codici prodotto con algoritmo shell sort.
void ordinaCatCod(t_prodotto vett[], int l_vett) {
	t_prodotto tmp;
	int h, c_selez, c_prec;

	h = 1;
	while (h < l_vett / 3) {
		h = 3 * h + 1;
	}

	while (h >= 1) {
		for (c_selez = h; c_selez < l_vett; c_selez++) {
			tmp = vett[c_selez];
			for (c_prec = c_selez; c_prec >= h && strcmp(tmp.codice, vett[c_prec - h].codice) < 0; c_prec -= h) {
				vett[c_prec] = vett[c_prec - h];
			}
			vett[c_prec] = tmp;
		}
		h = h / 3;
	}

	return;
}

// Funzione che ordina il catalogo di lunghezza l_vett in base ai 
// nomi dei prodotti con algoritmo shell sort.
void ordinaCatNom(t_prodotto vett[], int l_vett) {
	t_prodotto tmp;
	int h, c_selez, c_prec;

	h = 1;
	while (h < l_vett / 3) {
		h = 3 * h + 1;
	}

	while (h >= 1) {
		for (c_selez = h; c_selez < l_vett; c_selez++) {
			tmp = vett[c_selez];
			for (c_prec = c_selez; c_prec >= h && strcmp(tmp.nome, vett[c_prec - h].nome) < 0; c_prec -= h) {
				vett[c_prec] = vett[c_prec - h];
			}
			vett[c_prec] = tmp;
		}
		h = h / 3;
	}

	return;
}

// Questa funzione riceve il catalogo, la sua lunghezza e il puntatore
// alla variabile che contiene informazioni sull'attuale ordinamento del
// catalogo stesso. Se il catalogo è già ordinato per categorie, chiama la
// stampaPerCate, altrimenti prima di chiamare la suddetta funzione 
// lo ordina per categorie. Se il catalogo era già stato ordinato su richiesta
// dell'utente, al termine dell'operazione di stampa lo si riordina nella
// configurazione iniziale.
void gestisciElencCateg(t_comando *ptr_comando, t_prodotto vett[], int l_vett) {
	if (*ptr_comando == e_ordinatoCate) {
		stampaPerCate(vett, l_vett);
	}
	else {
		ordinaCatCate(vett, l_vett);
		stampaPerCate(vett, l_vett);
		switch (*ptr_comando) {
		case e_ordinatoPreA:
			ordinaCatPrez(vett, l_vett, 0);
			break;
		case e_ordinatoPreD:
			ordinaCatPrez(vett, l_vett, 1);
			break;
		case e_ordinatoCod:
			ordinaCatCod(vett, l_vett);
			break;
		case e_ordinatoNom:
			ordinaCatNom(vett, l_vett);
			break;
		case e_nonOrdinato:
			*ptr_comando = e_ordinatoCate;
			break;
		}
	}

	return;
}

// Funzione che ordina per categorie con algoritmo shell sort il 
// vettore di puntatori a struct t_prodotto vetPunt, di lunghezza l_vett.
void ordinaCatCate(t_prodotto vett[], int l_vett) {
	t_prodotto tmp;
	int h, c_selez, c_prec;

	h = 1;
	while (h < l_vett / 3) {
		h = 3 * h + 1;
	}

	while (h >= 1) {
		for (c_selez = h; c_selez < l_vett; c_selez++) {
			tmp = vett[c_selez];
			for (c_prec = c_selez; c_prec >= h && strcmp(tmp.categoria, vett[c_prec - h].categoria) < 0; c_prec -= h) {
				vett[c_prec] = vett[c_prec - h];
			}
			vett[c_prec] = tmp;
		}
		h = h / 3;
	}

	return;
}

// Funzione che scorre il vettore di puntatori a struct t_prodotto
// vetPunt, di lunghezza l_vett, già ordinato per categorie dalla
// ordinaVetPtrCate, e stampa a video i prodotti suddivisi per
// categorie.
void stampaPerCate(t_prodotto vett[], int l_vett) {
	char cateAtt[MAXSTR + 1] = "";
	int c_vett;

	c_vett = 0;
	while (c_vett < l_vett) {
		if (strcmp(cateAtt, vett[c_vett].categoria) != 0) {
			strcpy(cateAtt, vett[c_vett].categoria);
			fprintf(stdout, "Prodotti appartenenti alla categoria %s: \n", cateAtt);
		}
		while (c_vett < l_vett && strcmp(cateAtt, vett[c_vett].categoria) == 0) {
			fprintf(stdout, "%s %s %s %.2f %d %s\n", vett[c_vett].codice, vett[c_vett].nome, vett[c_vett].categoria, vett[c_vett].prezzo, vett[c_vett].rimasti, vett[c_vett].data);
			c_vett++;
		}
		fprintf(stdout, "%s", "\n");
	}

	return;
}

// Funzione che chiede il codice del prodotto da aggiornare, chiama la funzione di
// ricerca del codice opportuna a seconda dell'ordinamento attuale del vettore e
// se la ricerca ha successo chiede i nuovi dati del prodotto all'utente, in particolare
// il nuovo numero di unità disponibili del prodotto e la data odierna.
void gestisciAggiornamenti(t_ordine ordine, t_prodotto vett[], int l_vett) {
	char codCercato[LUNGCOD + 1];
	int idTrov;
	int newTot;
	char newData[LUNGDATA + 1];

	fprintf(stdout, "%s", "Inserire il codice del prodotto da aggiornare: ");
	fscanf(stdin, "%s", codCercato);
	if (ordine == e_ordinatoCod) {
		idTrov = cercaCodBin(codCercato, vett, l_vett);
	}
	else {
		idTrov = cercaCodLin(codCercato, vett, l_vett);
	}
	if (idTrov == -1) {
		fprintf(stdout, "%s", "Il codice richiesto non e' stato trovato.\n");
	}
	else {
		fprintf(stdout, "%s", "Inserire il nuovo numero di unita' disponibili: ");
		fscanf(stdin, "%d", &newTot);
		fprintf(stdout, "%s", "Inserire la data di oggi (formato: gg/mm/aaaa): ");
		fscanf(stdin, "%s", newData);
		vett[idTrov].rimasti = newTot;
		strcpy(vett[idTrov].data, newData);
	}

	return;
}

// Funzione che si occupa di chiedere il codice da cercare,
// chiamare la funzione di ricerca opportuna a seconda dell'ordinamento
// attuale del vettore e stamparne il risultato.
void gestisciCercaCod(t_ordine ordine, t_prodotto vett[], int l_vett) {
	char codCercato[LUNGCOD + 1];
	int idTrov;

	fprintf(stdout, "%s", "Inserire il codice da cercare: ");
	fscanf(stdin, "%s", codCercato);
	if (ordine == e_ordinatoCod) {
		idTrov = cercaCodBin(codCercato, vett, l_vett);
	}
	else {
		idTrov = cercaCodLin(codCercato, vett, l_vett);
	}
	if (idTrov == -1) {
		fprintf(stdout, "%s", "Il codice richiesto non e' stato trovato.\n");
	}
	else {
		fprintf(stdout, "%s", "Il codice richiesto e' stato trovato. I dati relativi sono i seguenti: \n");
		fprintf(stdout, "%s %s %s %.2f %d %s\n", vett[idTrov].codice, vett[idTrov].nome, vett[idTrov].categoria, vett[idTrov].prezzo, vett[idTrov].rimasti, vett[idTrov].data);
	}

	return;
}

// Funzione che cerca un determinato codice nel catalogo
// con algoritmo lineare. Restituisce -1 se il codice non viene
// trovato, altrimenti restituisce l'indice del prodotto
// corrispondente nel vettore di struct.
int cercaCodLin(char *codCercato, t_prodotto vett[], int l_vett) {
	int c_vett;

	for (c_vett = 0; c_vett < l_vett && strcmp(vett[c_vett].codice, codCercato) != 0; c_vett++);

	if (c_vett == l_vett) {
		c_vett = -1;
	}

	return c_vett;
}

// Funzione che cerca un determinato codice nel catalogo
// con algoritmo binario. Restituisce -1 se il codice non viene
// trovato, altrimenti restituisce l'indice del prodotto
// corrispondente nel vettore di struct.
int cercaCodBin(char *codCercato, t_prodotto vett[], int l_vett) {
	int m, l, r;

	l = 0;
	r = l_vett - 1;
	while (l <= r) {
		m = l + (r - l) / 2;
		if (strcmp(vett[m].codice, codCercato) == 0) {
			return m;
		}
		if (strcmp(vett[m].codice, codCercato) < 0) {
			l = m + 1;
		}
		else {
			r = m - 1;
		}

	}

	return -1;
}

// Funzione che si occupa di chiedere il nome da cercare, chiamare la
// funzione di ricerca opportuna a seconda dell'ordinamento attuale del vettore
// e stampare il suo risultato.
void gestisciCercaNom(t_ordine ordine, t_prodotto vett[], int l_vett) {
	char nomCercato[MAXSTR + 1];
	int idTrov;

	fprintf(stdout, "%s", "Inserire il nome da cercare: ");
	fscanf(stdin, "%s", nomCercato);
	if (ordine == e_ordinatoNom) {
		idTrov = cercaNomBin(nomCercato, vett, l_vett);
	}
	else {
		idTrov = cercaNomLin(nomCercato, vett, l_vett);
	}
	if (idTrov == -1) {
		fprintf(stdout, "%s", "Il nome richiesto non e' stato trovato.\n");
	}
	else {
		fprintf(stdout, "%s", "Il nome richiesto e' stato trovato. I dati relativi sono i seguenti: \n");
		fprintf(stdout, "%s %s %s %.2f %d %s\n", vett[idTrov].codice, vett[idTrov].nome, vett[idTrov].categoria, vett[idTrov].prezzo, vett[idTrov].rimasti, vett[idTrov].data);
	}

	return;
}

// Funzione che cerca un determinato nome nel catalogo
// con algoritmo lineare. Restituisce -1 se il codice non viene
// trovato, altrimenti restituisce l'indice del prodotto
// corrispondente nel vettore di struct.
int cercaNomLin(char *nomCercato, t_prodotto vett[], int l_vett) {
	int c_vett, l_cercato;

	l_cercato = strlen(nomCercato);
	for (c_vett = 0; c_vett < l_vett && strncmp(vett[c_vett].nome, nomCercato, l_cercato) != 0; c_vett++);

	if (c_vett == l_vett) {
		c_vett = -1;
	}

	return c_vett;
}

// Funzione che cerca un determinato codice nel catalogo
// con algoritmo binario. Restituisce -1 se il codice non viene
// trovato, altrimenti restituisce l'indice del prodotto
// corrispondente nel vettore di struct.
int cercaNomBin(char *nomCercato, t_prodotto vett[], int l_vett) {
	int m, l, r;
	int l_cercato;

	l = 0;
	r = l_vett - 1;
	l_cercato = strlen(nomCercato);
	while (l <= r) {
		m = l + (r - l) / 2;
		if (strncmp(vett[m].nome, nomCercato, l_cercato) == 0) {
			return m;
		}
		if (strncmp(vett[m].nome, nomCercato, l_cercato) < 0) {
			l = m + 1;
		}
		else {
			r = m - 1;
		}

	}

	return -1;
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