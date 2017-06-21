#include "tac.h"
#include <stdlib.h>
#include <stdio.h>

// ATTENTION: must math with tac_type_e at tac.h.
char *tac_type_str[] = { //@TODO: this is only the template.
	"TAC_SYMBOL",
	"TAC_MOVE",
	"TAC_ADD",
	"TAC_MUL",
	"TAC_LABEL",
	"TAC_BEGINFUN",
	"TAC_ENDFUN",
	"TAC_IFZ",
	"TAC_JUMP",
	"TAC_CALL",
	"TAC_ARG",
	"TAC_RET",
	"TAC_PRINT",
	"TAC_READ"
};

tac_t *tac_create(tac_type_t type, hash_node_t *res, hash_node_t *op1, hash_node_t *op2) {

	fprintf(stderr, "entrou no tac_create\n");

	tac_t* new_tac_node = (tac_t *)calloc(1, sizeof(tac_t));

	new_tac_node->type = type;
	new_tac_node->res = res;
	new_tac_node->op1 = op1;
	new_tac_node->op2 = op2;
	new_tac_node->next = NULL;
	new_tac_node->prev = NULL;

	printf("saiu do tac_create\n");

	return new_tac_node;
}

tac_t *tac_head(tac_t *node) {
	tac_t *i;
	for(i = node; i->prev; i = i->prev);
	return i;
}

tac_t *tac_tail(tac_t *node) {
	tac_t *i;
	for(i = node; i->next; i = i->next);
	return i;
}

tac_t *tac_join(tac_t *c1, tac_t *c2) {
	fprintf(stderr, "entrou no join\n");
	tac_t *i;
	//if(!c1 && !c2) return NULL; else
	if(!c1) return c2; else if(!c2) return c1;
	for(i = c2; i->prev; i = i->prev);
	i->prev = c1;
	c1->next = i;
	fprintf(stderr, "saiu do join\n");
	return c2;
}

tac_t *tac_reverse(tac_t *node) {
	tac_t *new_tac_node = tac_create(node->type, node->res, node->op1, node->op2);
	if(!node->prev) return new_tac_node;
	else return tac_join(new_tac_node, tac_reverse(node->prev));
}

void tac_print_forward(tac_t *head_node) {
	for(tac_t *i = head_node; i; i = i->next)
		printf("TAC(%s)\n", tac_type_str[i->type]);
}

void tac_print_backward(tac_t *tail_node) {
	for(tac_t *i = tail_node; i; i = i->prev)
		printf("TAC(%s)\n", tac_type_str[i->type]);
}

tac_t *tac_when(astree_t* node, tac_t* c0, tac_t* c1) { // studying
	hash_node_t* label_symbol = hash_label();
	tac_t* if_z = tac_create(TAC_IFZ, label_symbol,c0->res, 0);
	tac_t* label = tac_create(TAC_LABEL, label_symbol,0, 0);
	return tac_join(c0, tac_join(if_z, tac_join(c1, label)));
}

tac_t *tac_generate(astree_t *root) { // studying
	
	if (!root) return NULL;

	int i = 0;
	tac_t* cod[MAX_NUMBER_OF_CHILDREN];
	tac_t* res = 0;

	for(i=0; i<MAX_NUMBER_OF_CHILDREN; ++i)
		cod[i] = tac_generate(root->children[i]);

	switch (root->type) {
		case ASTREE_TK_ID:				res = tac_create(TAC_SYMBOL, root->symbol, 0, 0); break;
		case ASTREE_KW_WHEN_THEN:		res = tac_when(root, cod[0], cod[1]); break;
		case ASTREE_KW_RETURN:			res = tac_join(cod[0], tac_create(TAC_RET, NULL, NULL, NULL)); break;
		default: 						res = tac_join(tac_join(tac_join(cod[0], cod[1]), cod[2]), cod[3]);
	}

	return res;
}

void tac_test() {

	tac_t *tac[10];
	
	for(int i = 0; i<10; i++)
		tac[i] = tac_create(i, NULL, NULL, NULL);

	tac_t *aux_1 = tac_join(tac[0], tac_join(tac[1], tac_join(tac[2], tac_join(tac[3], tac[4]))));
	tac_t *aux_2 = tac_join(tac[5], tac_join(tac[6], tac_join(tac[7], tac_join(tac[8], tac[9]))));

	tac_print_backward(tac_join(aux_1, aux_2));
	printf("\n");
	tac_print_forward(tac[0]);

	printf("%d - %s\n", TAC_MOVE, tac_type_str[TAC_MOVE]);
}