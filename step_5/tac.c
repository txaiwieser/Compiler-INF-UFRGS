#include "tac.h"
#include "hash.h"
#include <stdlib.h>

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
	for(i = node; i->prev == NULL; i = i->prev);
	return i;
}

tac_t *tac_tail_node(tac_t *node) {
	tac_t *i;
	for(i = node; i->next == NULL; i = i->next);
	return i;
}