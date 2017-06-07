#ifndef __HASH_H__
#define __HASH_H__

typedef struct hash_node {
	int type;
	int dataType;
	int nature;
	int expressionType; 
	int parametersNumber;
	int isVariableOrFuncionDeclared;
	char *text;
	struct hash_node *next;
} hash_node_t;

#define DATATYPE_BYTE 1
#define DATATYPE_SHORT 2
#define DATATYPE_LONG 3
#define DATATYPE_FLOAT 4
#define DATATYPE_DOUBLE 5

#define NATURE_VARIABLE 1
#define NATURE_ARRAY 2
#define NATURE_FUNCTION 3

#define EXPRESSION_INTEGER 1
#define EXPRESSION_REAL 2
#define EXPRESSION_CHAR 3
#define EXPRESSION_STRING 4
#define EXPRESSION_BOOLEAN 5

void hash_init(void);
int hash_address(char *text);
hash_node_t *hash_find(char *text);
hash_node_t *hash_insert(char *text, int type, int dataType, int nature);
void hash_print(void);

#endif