#ifndef _INGREDIENTIH

#define _INGREDIENTIH
#define MAXING 255 + 1
#define NOMEFILEING "ingredienti.txt"

typedef struct tabella *TAB;
typedef struct ingrediente *ING;

TAB creaTabIng(void);
ING cercaIngPerNomeBin(TAB tabIng, char nome[]);
void stampaDetIng(FILE *output, TAB tabIng, char nome[]);
void stampaNomeIng(FILE *output, ING p_ing);
float costoIng(ING p_ing, int grammi);
float calorieIng(ING p_ing, int grammi);
void stampaTabIng(FILE *output, TAB tabIng);
void distruggiTabIng(TAB tabIng);

#endif