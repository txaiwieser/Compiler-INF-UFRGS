#ifndef __ASTREE_H__
#define __ASTREE_H__

#include <stdlib.h>
#include <stdio.h>

#include "hash.h"

#define MAX_SONS 4

#define ASTREE_PROG_START			0

// variableTypeAndValue:
#define ASTREE_VAR_DEC				1
#define ASTREE_BYTE_CHAR			2
#define ASTREE_BYTE_INT				3
#define ASTREE_SHORT_INT			4
#define ASTREE_LONG_INT				5
#define ASTREE_FLOAT_INT 			6
#define ASTREE_FLOAT_REAL			7
#define ASTREE_DOUBLE_INT			8
#define ASTREE_DOUBLE_REAL			8

// variableTypeAndValue list:
#define ASTREE_BYTE_ARR_INT			9
#define ASTREE_BYTE_ARR_CHAR		10
/*
Existe essa possibilidade análoga ao que foi feito com o float array.
#define ASTREE_BYTE_ARR_CI			10
*/
#define ASTREE_SHORT_ARR_INT		12
#define ASTREE_LONG_ARR_INT			14
#define ASTREE_FLOAT_ARR_RI			16
#define ASTREE_DOUBLE_ARR_RI		19

#define ASTREE_BYTE_ARR				11
#define ASTREE_SHORT_ARR			13
#define ASTREE_LONG_ARR				15
#define ASTREE_FLOAT_ARR			18
#define ASTREE_DOUBLE_ARR			20

#define ASTREE_INT_LST				21
#define ASTREE_CHAR_LST				23
#define ASTREE_FLOAT_LST			25

// functionDeclaration:
#define ASTREE_FUNC_DEC				27
#define ASTREE_PARAM_LST			28
#define ASTREE_NO_PARAM_LST
#define ASTREE_PARAM				29

// literals:
#define ASTREE_LIT_INT				22
#define ASTREE_LIT_REAL				26
#define ASTREE_LIT_CHAR				24
#define ASTREE_LIT_STRING			41

// command:
#define ASTREE_CMD_LST				35
#define ASTREE_CMD_BKTS				36

#define ASTREE_KW_READ				37
#define ASTREE_KW_PRINT				38
#define ASTREE_PRINT_LST			40
#define ASTREE_KW_RETURN			39

#define ASTREE_ATTRIB 				46
#define ASTREE_ATTRIB_ARR			47

// keyword terminals:
#define ASTREE_KW_BYTE				30
#define ASTREE_KW_SHORT				31
#define ASTREE_KW_LONG				32
#define ASTREE_KW_FLOAT				33
#define ASTREE_KW_DOUBLE			34

// non terminal productions:
#define ASTREE_KW_WHEN_THEN			42
#define ASTREE_KW_WHEN_THEN_ELSE	43
#define ASTREE_KW_WHILE				44
#define ASTREE_KW_FOR				45

// expression related:
#define ASTREE_EXP_PARENTHESIS		48
#define ASTREE_TK_ID 				49
#define ASTREE_ARRAY_CALL			50
#define ASTREE_FUNC_CALL			51
#define ASTREE_FUNC_ARGS			64
#define ASTREE_FUNC_ARGS_EXT		65

#define ASTREE_LEQ					52
#define ASTREE_GTE					53
#define ASTREE_EQU					54
#define ASTREE_NEQ					55
#define ASTREE_AND					56
#define ASTREE_OR 					57
#define ASTREE_ADD					59
#define ASTREE_SUB					60
#define ASTREE_MUL					58
#define ASTREE_DIV					61
#define ASTREE_LES					62
#define ASTREE_GTR					63
#define ASTREE_NOT					64


typedef struct astree_s {
    int type;
    hash_node_t *symbol;
    struct astree_s *son[MAX_SONS];
} astree_t;

astree_t* astree_create(int type, hash_node_t *symbol, astree_t *son_1, astree_t *son_2, astree_t *son_3, astree_t *son_4);
void astree_print(astree_t *node, int level);
char* decompileTree(astree_t* ast);


#endif