#include <stdio.h>
#include "lex.yy.h"
#include "hash.h"
#include "astree.h"
#include "semantic.h"

extern int 	yyparse();
extern int  getLineNumber();
extern void initMe();
extern int  isRunning();


int writeStringToFile(char *filePath, char *string) {
	FILE* f;
	if ((f = fopen(filePath, "w")) != NULL) {
		fprintf(f, "%s", string);
		return 1;
	} else {
		fprintf(stderr, "ERROR: Couldn't open %s\n", filePath);
		exit(1);
	}
}

#define SUCCESS 0
#define FILE_NOT_INFORMED 1
#define FILE_NOT_FOUND 2
#define SINTAX_ERROR 3
#define SEMANTIC_ERROR 4

int main(int argc, char **argv) {
	if (argc < 3) {
        printf("Error: invalid arguments\n");
        return FILE_NOT_INFORMED;
    }

	FILE* file;
	if (!(file = fopen(argv[1], "r"))) {
		printf ("Erro ao abrir arquivo!");
		return FILE_NOT_FOUND;
	}

	initMe();
	yyin = file;	
	yyparse();

	astree_print(tree, 0);
	astree_t *root = tree;

	// char *decompiledTree = decompileTree(root);
	// writeStringToFile(argv[2], decompiledTree);

	semanticSetDeclarations(root);
	semanticCheck(root);

	fprintf(stderr, "Programa aceito!\n");
	return SUCCESS;
}
