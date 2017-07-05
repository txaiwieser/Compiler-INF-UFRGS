//@TODO: inside for declarations should be externalized;
//@TODO: TAC_SYMBOLS shouldn't be printed.

#include "tac.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ATTENTION: this must math with tac_type_e at tac.h.
const char *tac_type_str[] = {
	"TAC_SYMBOL",
	"TAC_VAR",
	"TAC_ARR",
	"TAC_MOVE",
	"TAC_INC",
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
	"TAC_PARAM",
	"TAC_FCALL",
	"TAC_ACALL",
	"TAC_AATTRIB",
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
	tac_t *i;
	for(i = head_node; i; i = i->next)
		printf("TAC(%s, %s, %s, %s)\n", tac_type_str[i->type], i->res ? i->res->text : "", i->op1 ? i->op1->text : "", i->op2 ? i->op2->text : "");
}

void tac_print_backward(tac_t *tail_node) {
	tac_t *i;
	for(i = tail_node; i; i = i->prev)
		printf("TAC(%s, %s, %s, %s)\n", tac_type_str[i->type], i->res ? i->res->text : "", i->op1 ? i->op1->text : "", i->op2 ? i->op2->text : "");
}

tac_t *tac_when(astree_t *node, tac_t *c0, tac_t *c1) {
	hash_node_t* end_l = hash_label();
	tac_t* ifz = tac_create(TAC_IFZ, end_l, c0->res, NULL);
	tac_t* end = tac_create(TAC_LABEL, end_l, NULL, NULL);
	return tac_join(c0, tac_join(ifz, tac_join(c1, end)));
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
	tac_t *tac_end = tac_create(TAC_ENDFUN, NULL, NULL, NULL);
	return tac_join(tac_begin, tac_join(c1, tac_join(c2, tac_end)));
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

tac_t *tac_for(astree_t *node, tac_t *c0, tac_t *c1, tac_t *c2) {

	/*
		for base to end (ATTENTION: only ascendent)

		mov c0->res, base
		:begin
		<code>
		inc c0->res
		beq end, c0->res, end
		jmp begin
		:end
	*/

	hash_node_t *beg_l 	= hash_label();
	hash_node_t *end_l 	= hash_label();

	tac_t *mov = tac_create(TAC_MOVE, node->symbol, c0->res, NULL);
	tac_t *beg = tac_create(TAC_LABEL, beg_l, NULL, NULL);
	tac_t *inc = tac_create(TAC_INC, node->symbol, NULL, NULL);
	tac_t *beq = tac_create(TAC_BEQ, end_l, node->symbol, c1->res);
	tac_t *jmp = tac_create(TAC_JUMP, beg_l, NULL, NULL);
	tac_t *end = tac_create(TAC_LABEL, end_l, NULL, NULL);

	return tac_join(c0, tac_join(c1, tac_join(mov, tac_join(beg, tac_join(c2, tac_join(beq, tac_join(inc, tac_join(jmp, end))))))));
}

tac_t *tac_while(astree_t *node, tac_t *c0, tac_t *c1) {

	/*
		while (a>b)

		:begin
		ifz end, bool
		<code>
		jmp begin
		:end
	*/

	hash_node_t* beg_l = hash_label();
	hash_node_t* end_l = hash_label();

	tac_t *beg = tac_create(TAC_LABEL, beg_l, NULL, NULL);
	tac_t *ifz = tac_create(TAC_IFZ, end_l, c0->res, NULL);
	tac_t *jmp = tac_create(TAC_JUMP, beg_l, NULL, NULL);
	tac_t *end = tac_create(TAC_LABEL, end_l, NULL, NULL);

	return tac_join(c0, tac_join(beg, tac_join(ifz, tac_join(c1 ,tac_join(jmp, end)))));
}

tac_t *tac_var_dec(astree_t *node, tac_t *c0) {

	tac_t *dec;

	switch(node->symbol->nature) {
		case NATURE_VARIABLE:	dec = tac_create(TAC_VAR, node->symbol, node->children[0] ? node->children[0]->symbol : NULL, NULL); break;
		case NATURE_ARRAY:		dec = tac_create(TAC_ARR, node->symbol, node->children[0]->symbol, NULL); break;
	}

    return tac_join(c0, dec);
}

tac_t *tac_id(astree_t *node, tac_t *c0) {
	return tac_join(c0, tac_create(TAC_SYMBOL, node->symbol, 0, 0));
}

tac_t *tac_param(astree_t *node) {
	return tac_create(TAC_PARAM, node->symbol, NULL, NULL);
}

tac_t *tac_func_call(astree_t *node, tac_t *c0) {
	tac_t *cal = tac_create(TAC_FCALL, node->symbol, node->symbol, NULL);
	return tac_join(c0, cal);
}

tac_t *tac_args(astree_t *node, tac_t *c0, tac_t *c1) {
	tac_t *arg = tac_create(TAC_ARG, node->children[0]->symbol, NULL, NULL);
	return c1 ? tac_join(c0, tac_join(c1, arg)) : tac_join(arg, c0);
}

tac_t *tac_array_call(astree_t *node, tac_t *c0) {
	hash_node_t *tmp = hash_temporary();
	tac_t *cal = tac_create(TAC_ACALL, tmp, node->symbol, node->children[0]->symbol);
	return tac_join(c0, cal);
}

tac_t *tac_attrib(astree_t *node, tac_t *c0) {
	tac_t *att = tac_create(TAC_MOVE, node->symbol, c0->res, NULL);
	return tac_join(c0, att);
}

tac_t *tac_attrib_arr(astree_t *node, tac_t *c0, tac_t *c1) {
	tac_t *att = tac_create(TAC_AATTRIB, node->symbol, c0->res, c1->res);
	return tac_join(c0, tac_join(c1, att));
}

tac_t *tac_logical(astree_t *node, tac_t *c0, tac_t *c1) {
	hash_node_t *tmp = hash_temporary();
	tac_t *log = tac_create(node->type == ASTREE_AND ? TAC_AND : TAC_OR, tmp, c0->res, c1->res);
	return tac_join(c0, tac_join(c1, log));
}

void tac_fill_func_calls(tac_t* code) {
	
	tac_t *call;
	for(call = code; call; call = call->prev) {
		if(call->type == TAC_FCALL) {
			tac_t *dec, *ret;
			for(dec = code; !(dec->type == TAC_BEGINFUN && strcmp(call->op1->text, dec->res->text) == 0); dec = dec->prev);
			for(ret = dec; ret->type != TAC_RET; ret = ret->next);
			call->res = ret->res;
			tac_t *tac;
			for(tac = code; tac; tac = tac->prev) {
				if(tac->type != TAC_BEGINFUN && tac->type != TAC_FCALL) {
					if(tac->res && strcmp(tac->res->text, call->op1->text) == 0)
						tac->res = ret->res;
					else if(tac->op1 && strcmp(tac->op1->text, call->op1->text) == 0)
						tac->op1 = ret->res;
					else if(tac->op2 && strcmp(tac->op2->text, call->op1->text) == 0)
						tac->op2 = ret->res;
				}
			}
		}
	}
}

tac_t *tac_parse_astree(astree_t *root) {
	
	if (!root) return NULL;

	int i = 0;
	tac_t* c[MAX_NUMBER_OF_CHILDREN];
	tac_t* r = 0;

	for(i=0; i<MAX_NUMBER_OF_CHILDREN; ++i)
		c[i] = tac_parse_astree(root->children[i]);

	switch (root->type) {
	 	case ASTREE_VAR_DEC:			r = tac_var_dec(root, c[0]); break;
		case ASTREE_LIT_INT:
		case ASTREE_LIT_REAL:
		case ASTREE_LIT_CHAR:
		case ASTREE_LIT_STRING:
		case ASTREE_TK_ID:				r = tac_id(root, c[0]); break;
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
		case ASTREE_AND:
		case ASTREE_OR :				r = tac_logical(root, c[0], c[1]); break;
		case ASTREE_FUNC_DEC:			r = tac_function(root, c[0], c[1], c[2]); break;
		case ASTREE_PARAM:				r = tac_param(root); break;
		case ASTREE_KW_RETURN:			r = tac_return(root, c[0]); break;
		case ASTREE_FUNC_CALL:			r = tac_func_call(root, c[0]); break;
		case ASTREE_FUNC_ARGS:
		case ASTREE_FUNC_ARGS_EXT:		r = tac_args(root, c[0], c[1]); break;
		case ASTREE_ARRAY_CALL:			r = tac_array_call(root, c[0]); break;
		case ASTREE_KW_WHEN_THEN:		r = tac_when(root, c[0], c[1]); break;
		case ASTREE_KW_WHEN_THEN_ELSE:	r = tac_when_else(root, c[0], c[1], c[2]); break;
		case ASTREE_KW_PRINT:			r = tac_print(root, c[0]); break;
		case ASTREE_KW_READ:			r = tac_read(root); break;
		case ASTREE_KW_FOR:				r = tac_for(root, c[0], c[1], c[2]); break;
		case ASTREE_KW_WHILE:			r = tac_while(root, c[0], c[1]); break;
		case ASTREE_ATTRIB:				r = tac_attrib(root, c[0]); break;
		case ASTREE_ATTRIB_ARR:			r = tac_attrib_arr(root, c[0], c[1]); break;
		default: 						r = tac_join(tac_join(tac_join(c[0], c[1]), c[2]), c[3]);
	}

	return r;
}

tac_t *tac_generate(astree_t *root) {
	tac_t *code = tac_parse_astree(root);
	tac_fill_func_calls(code);
	return code;
}