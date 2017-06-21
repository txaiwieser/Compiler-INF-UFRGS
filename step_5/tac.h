#ifndef __TAC_H__
#define __TAC_H__

#include "hash.h"

typedef int tac_type_t;
typedef struct tac_s {

	tac_type_t type;
	hash_node_t *res;
	hash_node_t *op1;
	hash_node_t *op2;
	struct tac_s *next;
	struct tac_s *prev;

} tac_t;

tac_t *tac_node_create(tac_type_t type, hash_node_t *res, hash_node_t *op1, hash_node_t *op2);

#endif