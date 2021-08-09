#include "sciatori.h"

typedef struct {
	int cardId;
	LIST listAccessi;
} t_sciatore;

typedef struct nodoBST *linkBST;

typedef struct nodoBST {
	t_sciatore val;
	linkBST left;
	linkBST right;
} t_nodoBST;

static linkBST root;
static linkBST z;

linkBST nuovoNodoBST(t_sciatore val, linkBST left, linkBST right);
void freeNodoBST(linkBST n);
void distruggiBSTsciatoriR(linkBST n);

linkBST nuovoNodoBST(t_sciatore val, linkBST left, linkBST right) {
	linkBST h;

	h = malloc(sizeof(*h));
	h->val = val;
	h->left = left;
	h->right = right;

	return h;
}

void freeNodoBST(linkBST n) {
	distruggiListAccessi(n->val.listAccessi);
	free(n);

	return;
}

void initBSTsciatori(void) {
	root = z = NULL;

	return;
}

int authorize(int cardId, char *skiliftId, int time) {
	linkBST p, x;
	t_sciatore val;

	p = NULL;
	x = root;
	while (x != z && x->val.cardId != cardId) {
		p = x;
		if (cardId < x->val.cardId) {
			x = x->left;
		}
		else {
			x = x->right;
		}
	}
	
	if (x != z) {
		if (!insOaggAccesso(x->val.listAccessi, skiliftId, time)) {
			return 0;
		}
	}
	else {
		val.cardId = cardId;
		val.listAccessi = initListAccessi();
		if (p == NULL) {
			root = nuovoNodoBST(val, z, z);
			insOaggAccesso(root->val.listAccessi, skiliftId, time);
		}
		else {
			if (cardId < p->val.cardId) {
				p->left = nuovoNodoBST(val, z, z);
				insOaggAccesso(p->left->val.listAccessi, skiliftId, time);
			}
			else {
				p->right = nuovoNodoBST(val, z, z);
				insOaggAccesso(p->right->val.listAccessi, skiliftId, time);
			}
		}
	}

	return 1;
}

void distruggiBSTsciatori(void) {
	distruggiBSTsciatoriR(root);

	return;
}

void distruggiBSTsciatoriR(linkBST n) {
	if (n == z) {
		return;
	}

	distruggiBSTsciatoriR(n->left);
	distruggiBSTsciatoriR(n->right);
	freeNodoBST(n);

	return;
}