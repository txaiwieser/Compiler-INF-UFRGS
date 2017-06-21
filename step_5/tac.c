#include "tac.h"
#include "hash.h"
#include <stdlib.h>
#include <stdio.h>

// ATTENTION: must math with tac_type_e at tac.h.
char *tac_type_str[] = { //@TODO: this is only the template.
	"TAC_LABEL",
	"TAC_BEGINFUN"
};

tac_t *tac_node_create(tac_type_t type, hash_node_t *res, hash_node_t *op1, hash_node_t *op2) {

	tac_t* new_tac_node = (tac_t *)calloc(1, sizeof(tac_t));

	new_tac_node->type = type;
	new_tac_node->res = res;
	new_tac_node->op1 = op1;
	new_tac_node->op2 = op2;
	new_tac_node->next = NULL;
	new_tac_node->prev = NULL;

	return new_tac_node;
}

tac_t *tac_head_node(tac_t *node) {
	tac_t *i;
	for(i = node; i->prev; i = i->prev);
	return i;
}

tac_t *tac_tail_node(tac_t *node) {
	tac_t *i;
	for(i = node; i->next; i = i->next);
	return i;
}

tac_t *tac_code_join(tac_t *c1, tac_t *c2) {
	tac_t *i;
	if(!c1 && !c2) return NULL; else if(!c1) return c2; else if(!c2) return c1;
	for(i = c2; i->prev; i = i->prev);
	i->prev = c1;
	c1->next = i;
	return c2;
}

tac_t *tac_code_reverse_copy(tac_t *node) {
	tac_t *new_tac_node = tac_node_create(node->type, node->res, node->op1, node->op2);
	if(!node->prev) return new_tac_node;
	else return tac_code_join(new_tac_node, tac_code_reverse_copy(node->prev));
}

void tac_code_print_forward(tac_t *head_node) {
	for(tac_t *i = head_node; i; i = i->next)
		printf("TAC(%d)\n", i->type);
}

void tac_code_print_backward(tac_t *tail_node) {
	for(tac_t *i = tail_node; i; i = i->prev)
		printf("TAC(%d)\n", i->type);
}

void tac_test() {

	tac_t *tac[10];
	
	for(int i = 0; i<10; i++)
		tac[i] = tac_node_create(i, NULL, NULL, NULL);

	tac_t *aux_1 = tac_code_join(tac[0], tac_code_join(tac[1], tac_code_join(tac[2], tac_code_join(tac[3], tac[4]))));
	tac_t *aux_2 = tac_code_join(tac[5], tac_code_join(tac[6], tac_code_join(tac[7], tac_code_join(tac[8], tac[9]))));

	tac_code_print_backward(tac_code_join(aux_1, aux_2));
	printf("\n");
	tac_code_print_forward(tac[0]);

	printf("%d - %s\n", TAC_BEGINFUN, tac_type_str[TAC_BEGINFUN]);
}