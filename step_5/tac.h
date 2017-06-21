#ifndef __TAC_H__
#define __TAC_H__

#include "hash.h"
#include "astree.h"

typedef enum tac_type_e { //@TODO: incomplete.
	TAC_SYMBOL = 0,
	TAC_MOVE,
	TAC_ADD,
	TAC_MUL,
	TAC_LABEL,
	TAC_BEGINFUN,
	TAC_ENDFUN,
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
void tac_print_forward(tac_t *head_node);
void tac_print_backward(tac_t *tail_node);

void tac_test();

#endif