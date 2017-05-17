#include <stdio.h>
#include "lex.yy.h"
#include "hash.h"
#include "astree.h"

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

int main(int argc, char **argv) {
	if (argc < 3) {
        printf("Error: invalid arguments\n");
        exit(1);
    }

	FILE* file;
	if (!(file = fopen(argv[1], "r"))) {
		printf ("Erro ao abrir arquivo!");
		return 0;
	}

	initMe();
	yyin = file;
	
	yyparse();

	astree_t *root = tree;
	char *decompiledTree = decompileTree(root);

	writeStringToFile(argv[2], decompiledTree);

	fprintf(stderr, "Programa aceito!\n");

	// hashPrint();

	return 1;
}
