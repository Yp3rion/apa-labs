// LABORATORIO 1 ESERCIZIO 1
// DATA DI REALIZZAZIONE: 5/10/2016

// Questo programma dà per scontato che i dati in input siano formattati
// nel modo corretto.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXCHAR 5

float operando(char s[], float precedente);
float calcola_exp(char operatore, float operando1, float operando2);
void svuota_buffer(void);

int main(void) {
	char operatore;
	char str_operando1[MAXCHAR + 1], str_operando2[MAXCHAR + 1];
	float ris_prec;

	ris_prec = 0; // La inizializzo per non passare ad operando al primo ciclo una variabile indefinita.
	fprintf(stdout, "%s", "Inserire <operatore> <operando1> <operando2>: ");
	fscanf(stdin, "%c", &operatore);
	while (operatore == '+' || operatore == '-' || operatore == '*' || operatore == '/') {
		fscanf(stdin, "%s%s", str_operando1, str_operando2); 
		svuota_buffer();
		ris_prec = calcola_exp(operatore, operando(str_operando1, ris_prec), operando(str_operando2, ris_prec));
		fprintf(stdout, "Risultato: %f\n", ris_prec);
		fprintf(stdout, "%s", "Inserire <operatore> <operando1> <operando2>: ");
		fscanf(stdin, "%c", &operatore);
	}

	svuota_buffer();
	fprintf(stdout, "%s", "Fine.\n");

	fgetc(stdin);

	return EXIT_SUCCESS;
}

// Funzione che analizza la stringa s: se essa contiene PREV, la funzione 
// restituisce la variabile "precedente" (che contiene il risultato dell'operazione 
// precedente), altrimenti la funzione restituisce il valore float ottenuto
// convertendo s con atof.
float operando(char s[], float precedente) {
	if (strcmp("PREV", s) == 0) {
		return precedente;
	}
	else
		return atof(s);
}

// Funzione che restituisce il risultato dell'espressione ricevendo l'operatore e
// gli operandi.
float calcola_exp(char operatore, float operando1, float operando2) {
	switch (operatore) {
		case '+':
			return (operando1 + operando2);
		case '-':
			return (operando1 - operando2);
		case '*':
			return (operando1 * operando2);
		case '/':
			return (operando1 / operando2);
	}
}

// Funzione che si occupa di svuotare il buffer su stdin in maniera tale
// da garantire il funzionamento della fscanf "%c" e della getchar finale.
// La variabile fine_buffer serve per immagazzinare uno a uno i caratteri 
// contenuti nel buffer man mano che vengono letti, fino a quando non si 
// trova il carattere '\n', che per semplicità ipotizzo essere sempre 
// l'ultimo nel buffer.
void svuota_buffer(void) {
	char fine_buffer;

	do {
		fscanf(stdin, "%c", &fine_buffer);
	} while (fine_buffer != '\n');

	return;
}