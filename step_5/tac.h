#ifndef __TAC_H__
#define __TAC_H__

#include "hash.h"

typedef struct tac_s {

	hash_node_t *op1;
	hash_node_t *op2;
	hash_node_t *res;

} tac_t;

#endif