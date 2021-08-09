#ifndef INGREDIENTI_H
	#define INGREDIENTI_H
	#define MAXS 255
	#define NOMEINPUT "ingredienti.txt"

	typedef struct {
		char nome[MAXS];
		float prezzo_k;
		float cal_g;
	} t_ingrediente;

	typedef struct nodo *link;

	typedef struct nodo{
		t_ingrediente val;
		link next;
	} t_nodo;

	typedef struct {
		link head;
		link tail;
	} LIST;

	link creaNodo(t_ingrediente val, link next);
	LIST *creaTabIng(void);
	void insNodoInCoda(LIST *tabIng, t_ingrediente val);
	t_ingrediente cercaIngNome(LIST tabIng, char nome[]);
	void distruggiTabIng(LIST *tabIng);
#endif