#include "tac.h"
#include <stdlib.h>
#include <stdio.h>

// ATTENTION: must math with tac_type_e at tac.h.
char *tac_type_str[] = { //@TODO: this is only the template.
	"TAC_SYMBOL",
	"TAC_MOVE",
	"TAC_ADD",
	"TAC_SUB",
	"TAC_MUL",
	"TAC_DIV",
	"TAC_BLE",
	"TAC_BGE",
	"TAC_BEQ",
	"TAC_BNE",
	"TAC_AND",
	"TAC_OR",
	"TAC_BLT",
	"TAC_BGT",
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

	tac_t* new_tac_node = (tac_t *)calloc(1, sizeof(tac_t));

	new_tac_node->type	= type;
	new_tac_node->res	= res;
	new_tac_node->op1	= op1;
	new_tac_node->op2	= op2;
	new_tac_node->next	= NULL;
	new_tac_node->prev	= NULL;

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

tac_t *tac_join(tac_t *c0, tac_t *c1) {
	tac_t *i;
	if(!c0 && !c1) return NULL; else if(!c0) return c1; else if(!c1) return c0;
	for(i = c1; i->prev; i = i->prev);
	i->prev = c0;
	c0->next = i;
	return c1;
}

tac_t *tac_reverse(tac_t *node) {
	tac_t *new_tac_node = tac_create(node->type, node->res, node->op1, node->op2);
	if(!node->prev) return new_tac_node;
	else return tac_join(new_tac_node, tac_reverse(node->prev));
}

void tac_print_forward(tac_t *head_node) {
	for(tac_t *i = head_node; i; i = i->next)
		printf("TAC(%s, %s, %s, %s)\n", tac_type_str[i->type], i->res ? i->res->text : "", i->op1 ? i->op1->text : "", i->op2 ? i->op2->text : "");
}

void tac_print_backward(tac_t *tail_node) {
	for(tac_t *i = tail_node; i; i = i->prev)
		printf("TAC(%s, %s, %s, %s)\n", tac_type_str[i->type], i->res ? i->res->text : "", i->op1 ? i->op1->text : "", i->op2 ? i->op2->text : "");
}

tac_t *tac_when(astree_t* node, tac_t* c0, tac_t* c1) {
	hash_node_t* label_symbol = hash_label();
	tac_t* if_z = tac_create(TAC_IFZ, label_symbol, c0->res, NULL);
	tac_t* label = tac_create(TAC_LABEL, label_symbol, NULL, NULL);
	return tac_join(c0, tac_join(if_z, tac_join(c1, label)));
}

tac_t *tac_when_else(astree_t *node, tac_t *c0, tac_t *c1, tac_t *c2) {

	/*
		ifz else
		<code>
		jmp end
		:else
		<code>
		:end
	*/

	hash_node_t *else_label_sym = hash_label();
	hash_node_t *end_label_sym = hash_label();

	tac_t *if_z = tac_create(TAC_IFZ, else_label_sym, c0->res, NULL);
	tac_t *jump = tac_create(TAC_JUMP, end_label_sym, NULL, NULL);
	tac_t *else_label = tac_create(TAC_LABEL, else_label_sym, NULL, NULL);
	tac_t *end_label = tac_create(TAC_LABEL, end_label_sym, NULL, NULL);
	return tac_join(c0, tac_join(if_z, tac_join(c1, tac_join(jump, tac_join(else_label, tac_join(c2, end_label))))));
}

tac_t *tac_aritimetic(astree_t *node, tac_t *c0, tac_t *c1) {

	hash_node_t *temp = hash_temporary();
	tac_t *tac;

	switch(node->type) {
		case ASTREE_ADD: tac = tac_create(TAC_ADD, temp, c0->res, c1->res); break;
		case ASTREE_SUB: tac = tac_create(TAC_SUB, temp, c0->res, c1->res); break;
		case ASTREE_MUL: tac = tac_create(TAC_MUL, temp, c0->res, c1->res); break;
		case ASTREE_DIV: tac = tac_create(TAC_DIV, temp, c0->res, c1->res); break;
	}

	return tac_join(tac_join(c0, c1), tac);
}

tac_t *tac_boolean(astree_t *node, tac_t *c0, tac_t *c1) {

	/*
		brc label1, op1, op2 (brc = ble, bge, beq, ...)
		temp = 0
		jmp = label2
		:label1
		temp = 1
		:label2
	*/

	hash_node_t *label_1 = hash_label();
	hash_node_t *label_2 = hash_label();
	hash_node_t *temp = hash_temporary();

	tac_t *branch;
	tac_t *mov0			= tac_create(TAC_MOVE, temp, hash_false, NULL);
	tac_t *jump 		= tac_create(TAC_JUMP, label_2, NULL, NULL);
	tac_t *label_1_t 	= tac_create(TAC_LABEL, label_1, NULL, NULL);
	tac_t *mov1			= tac_create(TAC_MOVE, temp, hash_true, NULL);
	tac_t *label_2_t 	= tac_create(TAC_LABEL, label_2, NULL, NULL);

	switch(node->type) {
		case ASTREE_LEQ: branch = tac_create(TAC_BLE, label_1, c0->res, c1->res); break;
		case ASTREE_GTE: branch = tac_create(TAC_BGE, label_1, c0->res, c1->res); break;
		case ASTREE_EQU: branch = tac_create(TAC_BEQ, label_1, c0->res, c1->res); break;
		case ASTREE_NEQ: branch = tac_create(TAC_BNE, label_1, c0->res, c1->res); break;
		case ASTREE_LES: branch = tac_create(TAC_BLT, label_1, c0->res, c1->res); break;
		case ASTREE_GTR: branch = tac_create(TAC_BGT, label_1, c0->res, c1->res); break; 
	}

	tac_t *boolean = tac_create(TAC_SYMBOL, temp, NULL, NULL);
	tac_t *sequenc = tac_join(tac_join(branch, tac_join(mov0, tac_join(jump, tac_join(label_1_t, tac_join(mov1, label_2_t))))), boolean);

	return tac_join(tac_join(c0, c1), sequenc);
}

tac_t *tac_function(astree_t *node, tac_t *c0, tac_t *c1, tac_t *c2) {
	tac_t *tac_begin = tac_create(TAC_BEGINFUN, node->symbol, NULL, NULL);
	tac_t *tac_end = tac_create(TAC_ENDFUN, node->symbol, NULL, NULL);
	return tac_join(tac_begin, tac_join(c1, tac_join(c2, tac_end)));
}

// tac_t *tac_orand(astree_t *node, tac_t *c1, tac_t *c1) {

// 	hash_node_t *

// 	tac_t *cmd;
// 	switch(node->type) {
// 		case ASTREE_OR : cmd = tac_create(TAC_OR , NULL, c0->res, c1->res); break;
// 		case ASTREE_AND: cmd = tac_create(TAC_AND, NULL, c0->res, c1->res); break;
// 	}

// 	return tac_join(c0, tac_join(if_z, tac_join(c1, label)));

// }

tac_t *tac_var_dec(astree_t *node, tac_t *c0, tac_t *c1) {
	return NULL;
}

tac_t *tac_print(astree_t *node, tac_t *c0) {
	tac_t *print = tac_create(TAC_PRINT, c0 ? c0->res : NULL, NULL, NULL);
	return tac_join(c0, print);
}

tac_t *tac_read(astree_t *node) {
	tac_t *read = tac_create(TAC_READ, node->symbol, NULL, NULL);
	return read;
}

tac_t *tac_return(astree_t *node, tac_t *c0) {
	tac_t *ret = tac_create(TAC_RET, c0->res, NULL, NULL);
	return tac_join(c0, ret); 
}

tac_t *tac_generate(astree_t *root) {
	
	if (!root) return NULL;

	int i = 0;
	tac_t* c[MAX_NUMBER_OF_CHILDREN];
	tac_t* r = 0;

	for(i=0; i<MAX_NUMBER_OF_CHILDREN; ++i)
		c[i] = tac_generate(root->children[i]);

	switch (root->type) {
		// case ASTREE_VAR_DEC:			r = tac_var_dec(root, c[0], c[1]);		
		case ASTREE_LIT_INT:
		case ASTREE_LIT_REAL:
		case ASTREE_LIT_CHAR:
		case ASTREE_LIT_STRING:
		case ASTREE_TK_ID:				r = tac_create(TAC_SYMBOL, root->symbol, 0, 0); break;
		case ASTREE_KW_WHEN_THEN:		r = tac_when(root, c[0], c[1]); break;
		case ASTREE_KW_WHEN_THEN_ELSE:	r = tac_when_else(root, c[0], c[1], c[2]); break;
		case ASTREE_KW_RETURN:			r = tac_return(root, c[0]); break;
		case ASTREE_ADD:
		case ASTREE_SUB:
		case ASTREE_MUL:
		case ASTREE_DIV:				r = tac_aritimetic(root, c[0], c[1]); break;
		case ASTREE_LEQ:
		case ASTREE_GTE:
		case ASTREE_EQU:
		case ASTREE_NEQ:
		case ASTREE_LES:
		case ASTREE_GTR:				r = tac_boolean(root, c[0], c[1]); break;
		// case ASTREE_AND:				r = ; break;
		// case ASTREE_OR :				r = ; break;
		case ASTREE_FUNC_DEC:			r = tac_function(root, c[0], c[1], c[2]); break;
		case ASTREE_KW_PRINT:			r = tac_print(root, c[0]); break;
		case ASTREE_KW_READ:			r = tac_read(root); break;
		default: 						r = tac_join(tac_join(tac_join(c[0], c[1]), c[2]), c[3]);
	}

	return r;
}

void tac_test() {

	// tac_t *tac[10];
	
	// for(int i = 0; i<10; i++)
	// 	tac[i] = tac_create(i, NULL, NULL, NULL);

	// tac_t *aux_1 = tac_join(tac[0], tac_join(tac[1], tac_join(tac[2], tac_join(tac[3], tac[4]))));
	// tac_t *aux_2 = tac_join(tac[5], tac_join(tac[6], tac_join(tac[7], tac_join(tac[8], tac[9]))));

	// tac_print_backward(tac_join(aux_1, aux_2));
	// printf("\n");
	// tac_print_forward(tac[0]);

	// printf("%d - %s\n", TAC_MOVE, tac_type_str[TAC_MOVE]);

	// astree_t *n = astree_create(ASTREE_LES, NULL, NULL, NULL, NULL, NULL);
	// tac_print_backward(tac_boolean(n, NULL, NULL));
}