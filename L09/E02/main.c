#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "PQ.h"
#include "item.h"

#define NUMCOMANDI 7
#define MAXLUNGCOM 9 + 1

typedef enum {
	c_stampa,
	c_inserisci,
	c_elimina,
	c_sfida,
	c_carica,
	c_salva,
	c_fine,
	c_err
} t_comando;

void stampaMenu(void);
t_comando leggiComando(void);
int eseguiComando(t_comando comando);
int eseguiSfida(void);
void svuotaBufStdin(void);

int main(void) {
	t_comando comando;

	srand((unsigned int)time(NULL));

	if (!PQinit()) {
		fprintf(stdout, "%s", "Errore nell'inizializzazione del programma.\n");
		getchar();
		return EXIT_FAILURE;
	}

	stampaMenu();
	while ((comando = leggiComando()) != c_fine) {
		if (!eseguiComando(comando)) {
			fprintf(stdout, "%s", "Errore critico. Il programma sara' terminato.\n");
			PQdestroy();
			svuotaBufStdin();
			getchar();
			return EXIT_FAILURE;
		}
		stampaMenu();
	}

	PQdestroy();
	svuotaBufStdin();
	getchar();
	
	return EXIT_SUCCESS;
}

void stampaMenu(void) {
	fprintf(stdout, "%s", "Inserire la stringa corrispondente all'opzione desiderata (case sensitive):\n");
	fprintf(stdout, "%s", "- Per stampare a video lo stato attuale del torneo, scrivere \"stampa\";\n");
	fprintf(stdout, "%s", "- Per aggiungere un partecipante al torneo, scrivere \"inserisci\";\n");
	fprintf(stdout, "%s", "- Per eliminare un partecipante dal torneo, scrivere \"elimina\";\n");
	fprintf(stdout, "%s", "- Per far sfidare i due partecipanti con punteggio inferiore, scrivere \"sfida\";\n");
	fprintf(stdout, "%s", "- Per caricare da file lo stato di un torneo, scrivere \"carica\";\n");
	fprintf(stdout, "%s", "- Per salvare su file lo stato attuale del torneo, scrivere \"salva\";\n");
	fprintf(stdout, "%s", "- Per terminare il programma, scrivere \"fine\";\n");
	fprintf(stdout, "%s", "Scelta: ");
}

t_comando leggiComando(void) {
	t_comando comando;
	char opzioni[NUMCOMANDI][MAXLUNGCOM] = { "stampa", "inserisci", "elimina", "sfida", "carica", "salva", "fine" };
	char comLetto[MAXLUNGCOM];

	fscanf(stdin, "%s", comLetto);
	for (comando = c_stampa; comando != c_err && strcmp(comLetto, opzioni[comando]) != 0; comando++);

	return comando;
}

int eseguiComando(t_comando comando) {
	FILE *fp;
	char nomeFile[FILENAME_MAX];
	ITEM tmp;
	char nomCercato[MAXNOMPART];
	int esitoScan;

	fprintf(stdout, "%s", "\n");

	switch (comando) {
		case c_stampa:
			fprintf(stdout, "%s", "Lo stato attuale del torneo e' il seguente:\n");
			PQprint(stdout);
			break;
		case c_inserisci:
			ITEMscan(stdin, &tmp);
			if (ITEMcheckVoid(tmp)) {
				fprintf(stdout, "%s", "E' stato riscontrato qualche errore durante la creazione di un nuovo partecipante.\n");
			}
			else {
				if (!PQinsert(tmp)) {
					fprintf(stdout, "%s", "E' stato riscontrato qualche errore durante l'aggiunta del partecipante al torneo.\n");
				}
				else {
					fprintf(stdout, "%s", "Il partecipante e' stato aggiunto al torneo con successo.\n");
				}
			}
			break;
		case c_elimina:
			fprintf(stdout, "%s", "Inserire il nome del partecipante da eliminare: ");
			fscanf(stdin, "%s", nomCercato);
			if (ITEMcheckVoid(tmp = PQextractByN(nomCercato))) {
				fprintf(stdout, "%s", "Il nome dato non e' stato trovato.\n");
			}
			else {
				fprintf(stdout, "E' stato eliminato dal torneo il partecipante %s, che aveva %.2f punti.\n", KEYNget(tmp), *KEYPget(tmp));
				ITEMfree(tmp);
			}
			break;
		case c_sfida:
			if (!eseguiSfida()) {
				return 0;
			}
			break;
		case c_carica:
			fprintf(stdout, "%s", "Inserire il nome del file da aprire: ");
			fscanf(stdin, "%s", nomeFile);
			if ((fp = fopen(nomeFile, "r")) == NULL) {
				fprintf(stdout, "Il file %s non e' stato aperto con successo.\n", nomeFile);
			}
			else {
				while ((esitoScan = ITEMscan(fp, &tmp)) && !ITEMcheckVoid(tmp)) {
					if (!PQinsert(tmp)) {
						fprintf(stdout, "%s", "Errore durante l'aggiunta di un partecipante al torneo. Il programma sarà reinizializzato.\n");
						PQdestroy();
						if (!PQinit()) {
							return 0;
						}
					}
				}
				fclose(fp);
				if (esitoScan) {
					fprintf(stdout, "%s", "Qualcosa e' andato storto durante il caricamento dei dati. Il programma sarà reinizializzato.\n");
					PQdestroy();
					if (!PQinit()) {
						return 0;
					}
				}
				else {
					fprintf(stdout, "Tutti i dati validi contenuti nel file %s sono stati letti con successo.\n", nomeFile);
				}
			}
			break;
		case c_salva:
			fprintf(stdout, "%s", "Inserire il nome del file da creare: ");
			fscanf(stdin, "%s", nomeFile);
			if ((fp = fopen(nomeFile, "w")) == NULL) {
				fprintf(stdout, "Il file %s non e' stato creato con successo.\n", nomeFile);
			}
			else {
				PQprint(fp);
				fprintf(stdout, "Il file %s contiene ora lo stato attuale del torneo.\n", nomeFile);
				fclose(fp);
			}
			break;
		case c_fine:
			fprintf(stdout, "%s", "Il programma sarà ora terminato.\n");
			break;
		case c_err:
			fprintf(stdout, "%s", "Il comando dato non esiste. Provare un nuovo comando.\n");
			break;
	}

	fprintf(stdout, "%s", "\n");

	return 1;
}

int eseguiSfida(void) {
	ITEM val1, val2;
	float *punti1, *punti2, punti1_prev, punti2_prev, puntiTrasf;

	if (ITEMcheckVoid(val1 = PQextractMin())) {
		fprintf(stdout, "Al torneo non sono iscritti partecipanti; aggiungerne almeno due!\n");
		return 1;
	}
	if (ITEMcheckVoid(val2 = PQextractMin())) {
		fprintf(stdout, "Al torneo e' iscritto un unico partecipante; aggiungerne almeno uno!\n");
		if (!PQinsert(val1)) {
			ITEMfree(val1);
			return 0;
		}
		return 1;
	}
	punti1 = KEYPget(val1);
	punti1_prev = *punti1;
	punti2 = KEYPget(val2);
	punti2_prev = *punti2;
	if (rand() < RAND_MAX / 2) {
		puntiTrasf = (int)(*punti1 / 4) + 1;
		*punti1 -= puntiTrasf;
		*punti2 += puntiTrasf;
	}
	else {
		puntiTrasf = (int)(*punti2 / 4) + 1;
		*punti1 += puntiTrasf;
		*punti2 -= puntiTrasf;
	}
	if (*punti1 == 0) {
		fprintf(stdout, "Eliminato: %s. %s ha ora %.2f punti.\n", KEYNget(val1), KEYNget(val2), *punti2);
		if (!PQinsert(val2)) {
			fprintf(stdout, "%s", "Errore nella modifica del torneo. Sfida annullata.\n");
			*punti1 = punti1_prev;
			*punti2 = punti2_prev;
			if (!PQinsert(val1) || !PQinsert(val2)) {
				ITEMfree(val1);
				ITEMfree(val2);
				return 0;
			}
		}
	}
	else {
		if (*punti2 == 0) {
			fprintf(stdout, "Eliminato: %s. %s ha ora %.2f punti.\n", KEYNget(val2), KEYNget(val1), *punti1);
			if (!PQinsert(val1)) {
				fprintf(stdout, "%s", "Errore nella modifica del torneo. Sfida annullata.\n");
				*punti1 = punti1_prev;
				*punti2 = punti2_prev;
				if (!PQinsert(val1) || !PQinsert(val2)) {
					ITEMfree(val1);
					ITEMfree(val2);
					return 0;
				}
			}
		}
		else {
			fprintf(stdout, "%s ha ora %.2f punti, %s ha %.2f punti.\n", KEYNget(val1), *punti1, KEYNget(val2), *punti2);
			if (!PQinsert(val1) || !PQinsert(val2)) {
				fprintf(stdout, "%s", "Errore nella modifica del torneo. Sfida annullata.\n");
				*punti1 = punti1_prev;
				*punti2 = punti2_prev;
				if (!PQinsert(val1) || !PQinsert(val2)) {
					ITEMfree(val1);
					ITEMfree(val2);
					return 0;
				}
			}
		}
	}

	return 1;
}

void svuotaBufStdin(void) {
	char fineBuf;

	do {
		fscanf(stdin, "%c", &fineBuf);
	} while (fineBuf != '\n');

	return;
}