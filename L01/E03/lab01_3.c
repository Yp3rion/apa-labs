// LABORATORIO 1 ESERCIZIO 3
// DATA DI REALIZZAZIONE: 8/10/2016

// Nel programma non vengono effettuati controlli sugli input forniti da tastiera.

#include <stdio.h>
#include <stdlib.h>

#define MAXGRADO 10

float valutaIntegrale(float P[MAXGRADO + 1], int a, int b, int M);
float valutaHorner(float P[MAXGRADO + 1], float x);
void svuota_buffer(void);

int main(void) {
	int grado;
	int conta_passaggi;
	float polinomio[MAXGRADO + 1]; // Immagazzina i coefficienti da cn fino a c0.
	int exs, exd;
	int sottoint;

	fprintf(stdout, "%s", "Inserire il grado del polinomio: ");
	fscanf(stdin, "%d", &grado);
	// Porta a 0 i coefficienti che non utilizzerà:
	for (conta_passaggi = 0; conta_passaggi < MAXGRADO - grado; conta_passaggi++) {
		polinomio[conta_passaggi] = 0;
	}
	// Acquisisce i coefficienti rimanenti:
	for (; conta_passaggi < MAXGRADO + 1; conta_passaggi++) {
		fprintf(stdout, "Inserire il coefficiente c%d: ", MAXGRADO - conta_passaggi);
		fscanf(stdin, "%f", &polinomio[conta_passaggi]);
	}
	// Acquisisce gli estremi di integrazione:
	fprintf(stdout, "%s", "Inserire <estremo sinistro> <estremo destro> dell'intervallo di integrazione: ");
	fscanf(stdin, "%d%d", &exs, &exd);
	// Acquisisce il numero di sottointervalli:
	fprintf(stdout, "%s", "Inserire il numero di sottointervalli in cui dividere l'intervallo di integrazione: ");
	fscanf(stdin, "%d", &sottoint);

	fprintf(stdout, "L'integrale dato vale %.2f\n", valutaIntegrale(polinomio, exs, exd, sottoint));

	svuota_buffer();
	getchar();

	return EXIT_SUCCESS;
}

// Funzione che esegue il nucleo di calcolo dell'integrale a partire da polinomio,
// estremi di integrazione a e b e numero di sottointervalli M. Restituisce il 
// risultato.
float valutaIntegrale(float P[MAXGRADO + 1], int a, int b, int M) {
	float h;
	float sommatoria_polival; // Contiene la sommatoria delle valutazioni di P nel punto medio di ciascun sottointervallo.
	int conta_passaggi;

	h = (a + b) / ((float) M);
	sommatoria_polival = 0;
	for (conta_passaggi = 0; conta_passaggi < M; conta_passaggi++) {
		sommatoria_polival += valutaHorner(P, conta_passaggi * h + h / 2);
	}
	
	return h * sommatoria_polival;
}

// Funzione che valuta in x, mediante il metodo di Horner, il polinomio P di grado 
// n (max MAXGRADO) individuato dai suoi n+1 (max MAXGRADO+1) coefficienti reali.
float valutaHorner(float P[MAXGRADO + 1], float x) {
	int conta_coeff;
	float px;

	// Scarto coefficienti nulli fino a penultimo:
	for (conta_coeff= 0; P[conta_coeff] == 0 && conta_coeff < MAXGRADO - 1; conta_coeff++);
	// Eseguo il primo passo della valutazione fuori dal ciclo per attribuire a risultato un valore iniziale:
	px = P[conta_coeff] * x + P[conta_coeff + 1];
	conta_coeff++;
	// N.B.: Se ho già considerato anche gli ultimi due coefficienti non entro nel ciclo.
	for (; conta_coeff < MAXGRADO; conta_coeff++) {
		px = px * x + P[conta_coeff + 1];
	}

	return px;
}

// Funzione che si occupa di svuotare il buffer su stdin in maniera tale
// da garantire il funzionamento della getchar finale.
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