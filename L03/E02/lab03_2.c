// LABORATORIO 3 ESERCIZIO 2
// DATA DI REALIZZAZIONE: 22/10/2016

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 1000
#define MAXSTR 30
#define NOMEINPUT "accessi.txt"
#define MAXCOMANDO 7 // Costante simbolica usata in leggiComando.
#define MAXOPZIONI 5 // Usata in leggiComando.

// IMPORTANTE: le due costanti simboliche che seguono sono usate nella funzione selezionaDati. Esse sono utilizzate
// per stabilire in modo semplice e rapido una relazione d'ordine fra le varie date fornite. In particolare per far
// ciò ipotizzo che ogni mese sia costituito da 31 giorni e di conseguenza ogni anno sia costituito da 372 giorni.
// Una volta posta questa ipotesi (ovviamente erronea, ma valida dato che l'unico utilizzo che farò delle date sarà
// proprio stabilire tra esse relazioni d'ordine), traduco ogni data in un numero intero attraverso l'espressione seguente:
// dataintera = (anno - 1) * PESOANNO + (mese - 1) * PESOMESE + giorno.
#define PESOMESE 31
#define PESOANNO 372

// Scarterò l'ora e il tipo di richiesta nel leggere il file poiché non mi servono ai fini del 
// programma.
typedef struct {
	char indirizzo[MAXSTR + 1];
	char user[MAXSTR + 1];
	int giorno, mese, anno;
	char risorsa[MAXSTR + 1];
	int risposta;
} accesso;

typedef enum {
	r_nome,
	r_IP,
	r_rifiuto,
	r_risorse,
	r_fine
} comando_e;

int leggiTabella(accesso tab[], int maxdim);
comando_e leggiComando(void);
void selezionaDati(accesso tab[], int dim, comando_e comando);
void svuota_buffer(void);

int main(void) {
	accesso tabella[MAXN];
	int nDati;
	comando_e comando;

	if ((nDati = leggiTabella(tabella, MAXN)) == -1) {
		fprintf(stdout, "Il file %s non e' stato aperto correttamente.\n", NOMEINPUT);
		getchar();
		return EXIT_FAILURE;
	}

	while ((comando = leggiComando()) != r_fine) {
		selezionaDati(tabella, nDati, comando);
	}

	fprintf(stdout, "%s", "Operazione terminata.\n");
	svuota_buffer();
	getchar();

	return EXIT_SUCCESS;
}

// Questa funzione legge dal file accessi.txt i dati che poi inserisce nel vettore di struct
// accesso tab, la cui dimensione max è maxdim. Restituisce il numero effettivo di dati letti
// dal file, -1 se il file non e' stato aperto con successo.
int leggiTabella(accesso tab[], int maxdim) {
	FILE *input;
	int dati_letti, c_d;

	if ((input = fopen(NOMEINPUT, "r")) == NULL) {
		dati_letti = -1;
	}
	else {
		fscanf(input, "%d", &dati_letti);
		for (c_d = 0; c_d < dati_letti; c_d++) {
			fscanf(input, "%s%s%d/%d/%d%*s%*s%s%d", tab[c_d].indirizzo, tab[c_d].user, &tab[c_d].giorno, &tab[c_d].mese, &tab[c_d].anno, tab[c_d].risorsa, &tab[c_d].risposta);
		}
	}
	fclose(input);

	return dati_letti;
}

// Questa funzione chiede all'utente di fornire da tastiera un comando tra "nome", "IP", "rifiuto",
// "risorsa" e "fine" e converte tale comando nel corrispondente simbolo enum, che è ciò che restituisce.
comando_e leggiComando(void) {
	comando_e comando;
	char parola[MAXCOMANDO + 1];
	char opzioni[MAXOPZIONI][MAXCOMANDO + 1] = {"nome", "ip", "rifiuto", "risorsa", "fine"};

	fprintf(stdout, "%s","Inserire un comando tra ""nome"", ""IP"", ""rifiuto"", ""risorsa"" e ""fine"" (case sensitive): ");
	fscanf(stdin, "%s", parola); 
	for (comando = r_nome; comando < MAXOPZIONI && strcmp(parola, opzioni[comando]) != 0; comando++);

	return comando;
}

// Questa funzione chiede all'utente le date dell'intervallo da considerare, dopodiché
// cerca nel vettore di struct di tipo accesso tab (di dimensione dim) gli elementi
// individuati da date comprese nell'intervallo. A questo punto a seconda del valore di
// comando stamperà in output campi diversi di tale elemento, in maniera tale da rispondere
// alla richiesta fatta dall'utente.
void selezionaDati(accesso tab[], int dim, comando_e comando) {
	int giorno1, mese1, anno1;
	int giorno2, mese2, anno2;
	int giorno_considerato;
	int conta_dati;

	fprintf(stdout, "%s", "Inserire la prima data dell'intervallo (giorno/mese/anno): ");
	fscanf(stdin, "%d/%d/%d", &giorno1, &mese1, &anno1);
	fprintf(stdout, "%s", "Inserire la seconda data dell'intervallo (giorno/mese/anno):");
	fscanf(stdin, "%d/%d/%d", &giorno2, &mese2, &anno2);
	// Trasformo le date appena acquisite in interi nel modo indicato sopra:
	giorno1 = (anno1 - 1) * PESOANNO + (mese1 - 1) * PESOMESE + giorno1;
	giorno2 = (anno2 - 1) * PESOANNO + (mese2 - 1) * PESOMESE + giorno2;
	for (conta_dati = 0; conta_dati < dim; conta_dati++) {
		// Trasformo temporaneamente la data della struct che sto considerando nel modo indicato sopra:
		giorno_considerato = (tab[conta_dati].anno - 1) * PESOANNO + (tab[conta_dati].mese - 1) * PESOMESE + tab[conta_dati].giorno;
		if (giorno_considerato >= giorno1 && giorno_considerato <= giorno2) {
			switch (comando) {
				case r_nome:
					fprintf(stdout, "%s\n", tab[conta_dati].user);
					break;
				case r_IP:
					fprintf(stdout, "%s\n", tab[conta_dati].indirizzo);
					break;
				case r_rifiuto:
					if (tab[conta_dati].risposta == 401) {
						fprintf(stdout, "%s\n", tab[conta_dati].user);
					}
					break;
				case r_risorse:
					fprintf(stdout, "%s\n", tab[conta_dati].risorsa);
					break;
			}
		}
	}

	return;
}

// Funzione che si occupa di svuotare il buffer di stdin in maniera che la
// getchar non registri i '\n' residui, ma consideri solo un nuovo input dell'utente.
// Per sviluppare questa funzione mi baso sull'ipotesi che '\n' sia sempre
// l'ultimo carattere nel buffer.
void svuota_buffer(void) {
	char fine_buffer;

	do {
		fscanf(stdin, "%c", &fine_buffer);
	} while (fine_buffer != '\n');

	return;
}