#ifndef _RICETTEH

#include "ingredienti.h"

#define _RICETTEH
#define MAXRIC 255 + 1
#define NOMEFILERIC "ricette.txt"

typedef struct lista *LIST;

LIST creaListRic(TAB tabIng);
void stampaDetRic(FILE *output, LIST listRic, char nome[]);
float costoRic(LIST listRic, char nome[]);
float calorieRic(LIST listRic, char nome[]);
void stampaRicConIng(FILE *output, LIST listRic, TAB tabIng, char nomeIng[]);
void stampaListRic(FILE *output, LIST listRic);
void aggiungiRicetta(LIST listRic, TAB tabIng);
void distruggiListRic(LIST listRic);

#endif
