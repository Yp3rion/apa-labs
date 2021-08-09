#ifndef _PRINCMOLTHEADER
	#define _PRINCMOLTHEADER
	#define MAXS 255
	#define NOMEINPUT "brani.txt"
	
	typedef struct {
		char **vet_titoli;
		int n_titoli;
	} t_amico;

	typedef struct {
		t_amico *vet_amici;
		int n_amici;
	} t_amici;

	t_amici *creaVettScelte(void);
	t_amici *leggiVettScelte(t_amici *val);
	void distruggiVettScelte(t_amici *val);
	int *creaVettSol(int dim_sol);
	void distruggiVettSol(int *sol);
	void princ_molt(FILE *output, int pos, int *sol, t_amici val);
#endif
