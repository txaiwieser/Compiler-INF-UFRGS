#ifndef __TAC_H__
#define __TAC_H__

#include "hash.h"
#include "astree.h"

typedef enum tac_type_e { //@TODO: incomplete.
	TAC_SYMBOL = 0,

	TAC_VAR,
	TAC_ARR,

	TAC_MOVE,
	
	TAC_INC,
	TAC_ADD,
	TAC_SUB,
	TAC_MUL,
	TAC_DIV,

	TAC_BLE,
	TAC_BGE,
	TAC_BEQ,
	TAC_BNE,
	TAC_AND,
	TAC_OR,
	TAC_BLT,
	TAC_BGT,

	TAC_LABEL,
	TAC_BEGINFUN,
	TAC_ENDFUN,
	TAC_PARAM,
	TAC_IFZ,
	TAC_JUMP,
	TAC_CALL,
	TAC_ARG,
	TAC_RET,
	TAC_PRINT,
	TAC_READ
} tac_type_t;

typedef struct tac_s {

	tac_type_t type;
	hash_node_t *res;
	hash_node_t *op1;
	hash_node_t *op2;
	struct tac_s *next;
	struct tac_s *prev;

} tac_t;

tac_t *tac_generate(astree_t *root);
tac_t *tac_reverse(tac_t *node);
void tac_print_forward(tac_t *head_node);
void tac_print_backward(tac_t *tail_node);

void tac_test();

#endif