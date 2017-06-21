#ifndef __TAC_H__
#define __TAC_H__

#include "hash.h"

enum tac_type {
	TAC_LABEL = 0,
	//TAC_SYMBOL,	// <tac_temp>: symbol name, data_type, data_value	// parecido com TAC_VARDEC, só que declara constantes

	TAC_BEGINFUN,	// <tac_temp>: symbol (identifier) of the function, N_args, return type
	TAC_ENDFUN,		//
	TAC_CALL,		// <tac_label>: function label to jump to, N_args, <tac_temp>: where to put function results
	//TAC_PARAM,	// <tac_label>: parent function, param_number, data_type
	TAC_ARG,		// position (left to right), <tac_temp>: temporary symbol with argument value

	//TAC_RETURN,	// Não vai haver um TAC_RETURN, mas sim um TAC_MOVE

	TAC_VARDEC,		// <tac_temp>, data_type, init_value: símbolo com o literal
	TAC_VECDEC,		// <tac_temp>, data_type, size
	TAC_VEC_INIT,	// <tac_temp>, position, data_value // seta valor do vetor para cada posição

	TAC_MOVE,		// <tac_temp>: destination, (<tac_temp>: offset (<tac_temp>: source is a pointer))
	//TAC_CHG_TYPE,	// <tac_temp>: destination, <tac_temp>: original, data_type: new data type to convert to
	TAC_WRITE_VEC,
	TAC_READ_VEC,
	TAC_ADD,		// <tac_temp>: destination, <tac_temp>: op1, <tac_temp>: op2
	TAC_SUB,		// <tac_temp>: destination, <tac_temp>: op1, <tac_temp>: op2
	TAC_MUL,		// <tac_temp>: destination, <tac_temp>: op1, <tac_temp>: op2
	TAC_DIV,		// <tac_temp>: destination, <tac_temp>: op1, <tac_temp>: op2

	TAC_EQ,			// <tac_temp>: destination, <tac_temp>: op1, <tac_temp>: op2
	TAC_NE,			// <tac_temp>: destination, <tac_temp>: op1, <tac_temp>: op2
	TAC_GT,			// <tac_temp>: destination, <tac_temp>: op1, <tac_temp>: op2
	TAC_GE,			// <tac_temp>: destination, <tac_temp>: op1, <tac_temp>: op2
	TAC_LT,			// <tac_temp>: destination, <tac_temp>: op1, <tac_temp>: op2
	TAC_LE,			// <tac_temp>: destination, <tac_temp>: op1, <tac_temp>: op2

	TAC_AND,		// <tac_temp>: destination, <tac_temp>: op1, <tac_temp>: op2
	TAC_OR,			// <tac_temp>: destination, <tac_temp>: op1, <tac_temp>: op2
	TAC_NOT,		// <tac_temp>: destination, <tac_temp>: op1

	TAC_GOTO,		// <tac_label>: where to jump
	TAC_JMP_F,		// <tac_label>: where to jump, <tac_temp>: the symbol which is compared with false

	TAC_READ,		// <tac_temp>: label to put read value
	TAC_PRINT		// <tac_temp>: variável com o dado para imprimir, data_type
};

typedef int tac_type_t;
typedef struct tac_s {

	tac_type_t type;
	hash_node_t *res;
	hash_node_t *op1;
	hash_node_t *op2;
	struct tac_s *next;
	struct tac_s *prev;

} tac_t;

void tac_test();

#endif