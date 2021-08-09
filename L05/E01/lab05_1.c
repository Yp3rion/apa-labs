// LABORATORIO 5 ESERCIZIO 1
// DATA DI REALIZZAZIONE: 6/11/2016

#include <stdio.h>
#include <stdlib.h>

int hofstadterQseq(int n);
int hofstadterQseqR(int x, int risultati[]);
void svuotaBufStdin(void);

int main(void) {
	int n, c_passi;
	int risultato;

	fprintf(stdout, "%s", "Inserire il numero di termini della sequenza Q da visualizzare: ");
	fscanf(stdin, "%d", &n);

	fprintf(stdout, "I primi %d termini della sequenza Q sono:\n", n);
	for (c_passi = 1; c_passi <= n && (risultato = hofstadterQseq(c_passi)) != -1; c_passi++) {
		fprintf(stdout, "%d ", risultato);
	}

	if (risultato == -1) {
		fprintf(stdout, "\nErrore di allocazione durante calcolo del %do elemento.\n", c_passi);
		svuotaBufStdin();
		getchar();
		return EXIT_FAILURE;
	}

	svuotaBufStdin();
	getchar();

	return EXIT_SUCCESS;
}

// Funzione wrapper che alloca il vettore, di dimensione n+1, in cui conservare
// i risultati parziali, inizializza i suoi elementi a 0 e chiama la vera e 
// propria funzione ricorsiva. Restituisce il risultato della chiamata a funzione
// ricorsiva se l'allocazione del vettore avviene con successo, altrimenti 
// restituisce -1.
// N.B.: Il vettore ha dimensione n+1 per poter sfruttare la corrispondenza
// indice-dato.
int hofstadterQseq(int n) {
	int *risultati;
	int c_ris;

	if ((risultati = (int *)malloc((n + 1) * sizeof(*risultati))) == NULL) {
		return -1;
	}
	for (c_ris = 0; c_ris < n + 1; c_ris++) {
		risultati[c_ris] = 0;
	}
	
	return hofstadterQseqR(n, risultati);
}

// Funzione ricorsiva che dato x calcola il numero intero che occupa
// la posizione x nella sequenza Q di Hofstadter. Se l'elemento di 
// indice x nel vettore dei risultati parziali contiene già un risultato
// viene restituito tale risultato, altrimenti si calcola il risultato
// e lo si inserisce nell'elemento del vettore dei risultati parziali
// di indice x, quindi lo si restituisce.
int hofstadterQseqR(int x, int risultati[]) {
	
	if (risultati[x] != 0) {
		return risultati[x];
	}
	
	if (x == 2 || x == 1) {
		risultati[x] = 1;
		return risultati[x];
	}
	else {
		risultati[x] = (hofstadterQseqR(x - hofstadterQseqR(x - 1, risultati), risultati) + hofstadterQseqR(x - hofstadterQseqR(x - 2, risultati), risultati));
		return risultati[x];
	}
}

// Funzione che ha lo scopo di far funzionare la getchar finale,
// che altrimenti assorbirebbe eventuali whitespace ignorati 
// dalle fscanf precedenti.
void svuotaBufStdin(void) {
	char fineBuf;

	do {
		fscanf(stdin, "%c", &fineBuf);
	} while (fineBuf != '\n');

	return;
}