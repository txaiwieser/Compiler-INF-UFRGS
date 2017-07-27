#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hash.h"
#include "symcodes.h"

unsigned int label_count = 0;
unsigned int temporary_count = 0;

int hash_address(char *text) {
	int address = 1, i;
	for(i = 0; i< strlen(text); i++)
		address = (address * text[i])%HASH_SIZE + 1;
	return address - 1;
}

int hash_equal(hash_node_t *n1, hash_node_t *n2) {

	if(n1->type == SYMBOL_IDENTIFIER && n2->type == SYMBOL_IDENTIFIER && !strcmp(n1->text, n2->text))
		return 1;
	else
		return 0;
}

hash_node_t *hash_find(hash_node_t *node) {

	hash_node_t *aux = hash_table[hash_address(node->text)];

	while(aux != NULL) {
		if(!hash_equal(node, aux))
			aux = aux->next;
		else
			return aux;		
	}	
	
	return NULL;
}

hash_node_t *hash_insert(char *text, int type, int dataType, int nature) {
	
	hash_node_t *new_node;
	int address = hash_address(text);

	new_node = (hash_node_t *) calloc(1, sizeof(hash_node_t));
	new_node->text = calloc(strlen(text)+1, sizeof(char));

	new_node->type = type;
	new_node->dataType = dataType;
	new_node->nature = nature;
	new_node->parametersNumber = -1;
	new_node->isVariableOrFuncionDeclared = 0;
	new_node->expressionType = 0;

	strcpy(new_node->text, text);

	hash_node_t *aux = hash_find(new_node);

	printf("Debug: trying to insert in hash %s\n", text);

	if(aux == NULL) {
		new_node->next = hash_table[address];
		hash_table[address] = new_node;
		printf("Debug: inserting in hash %s\n", new_node->text);
		return new_node;
	}

	return aux;
}

void hash_print(void) {
	int i;
	hash_node_t *node = 0;
	for(i = 0; i < HASH_SIZE; i++)
		for(node = hash_table[i]; node; node =  node->next)
			printf("Table[%d].text\t %s\t type %d dataType %d nature %d exprType %d isDeclared %d\n",
			 i, node->text, node->type, node->dataType, node->nature, node->expressionType, node->isVariableOrFuncionDeclared);
}

hash_node_t *hash_aux_node(char *text) {

	hash_node_t *new_node;
	new_node = (hash_node_t *) calloc(1, sizeof(hash_node_t));
	
	new_node->text = calloc(strlen(text)+1, sizeof(char));
	strcpy(new_node->text, text);

	new_node->type = SYMBOL_IDENTIFIER;
	new_node->dataType = 0;
	new_node->nature = 0;
	new_node->parametersNumber = 0;
	new_node->isVariableOrFuncionDeclared = 0;
	new_node->expressionType = 0;

	return new_node;
}

hash_node_t *hash_boolean(char *text) {

	hash_node_t *new_node = hash_aux_node(text);
	new_node->type = SYMBOL_LIT_INTEGER;
	new_node->nature = NATURE_BOOLEAN;
	new_node->expressionType = EXPRESSION_BOOLEAN;

	hash_node_t *aux = hash_find(new_node);
	int address = hash_address(new_node->text);
	if(aux == NULL) {
		new_node->next = hash_table[address];
		hash_table[address] = new_node;
		return new_node;
	}

	return aux;
}

hash_node_t *hash_label() {

	char *sufix = malloc(11);
	snprintf(sufix, 10, "%u", label_count);
	label_count++;
	char *label = malloc(8 +strlen(sufix));
	sprintf(label, "__label_%s", sufix);

	hash_node_t *new_node = hash_aux_node(label);
	new_node->nature = NATURE_LABEL;
	free(sufix); free(label);

	return new_node;
}

hash_node_t *hash_temporary() {

	char *sufix = malloc(11);
	snprintf(sufix, 10, "%u", temporary_count);
	temporary_count++;
	char *temporary = malloc(8 +strlen(sufix));
	sprintf(temporary, "__temporary_%s", sufix);

	hash_node_t *new_node = hash_aux_node(temporary);
	new_node->nature = NATURE_TEMPORARY;
	free(sufix); free(temporary);

	hash_node_t *aux = hash_find(new_node);
	int address = hash_address(new_node->text);
	if(aux == NULL) {
		new_node->next = hash_table[address];
		hash_table[address] = new_node;
		return new_node;
	}

	return aux;
}

void hash_init(void) {
	int i;
	for(i = 0; i < HASH_SIZE; i++)
		hash_table[i] = 0;

	hash_true = hash_boolean("1");
	hash_false = hash_boolean("0");
}
