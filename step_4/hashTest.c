#include <stdio.h>
#include "hash.h"
#include "hash.c"

int main(int argc, char const *argv[])
{

	char txt1[] = "texto 1";
	char txt2[] = "texto 2";
	char txt3[] = "texto 3";
	char txtr[] = "texto 1";

	hash_init();

	hash_insert(txt1, 0, 0, 0);
	printf("print 1\n");
	hash_print();
	hash_insert(txt2, 0, 0, 0);
	printf("print 2\n");
	hash_print();
	hash_insert(txt3, 0, 0, 0);
	printf("print 3\n");
	hash_print();
	hash_insert(txtr, 0, 0, 0);
	printf("print 4\n");
	hash_print();
	hash_insert(txt2, 0, 0, 0);
	printf("print 5\n");
	hash_print();

	printf("ponteiros\n");
	hash_node_t *aux;
	aux = hash_find(txt1);
	printf("%p\n", aux);
	aux = hash_find(txt2);
	printf("%p\n", aux);
	printf("print next do txt2\n");
	aux = aux->next;
	printf("%p\n", aux);
	aux = hash_find(txt3);
	printf("%p\n", aux);
	aux = hash_find(txtr);
	printf("%p\n", aux);
	printf("print next\n");
	aux = aux->next;
	printf("%p\n", aux);

	return 0;
}