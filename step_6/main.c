#include <stdio.h>
#include "lex.yy.h"
#include "hash.h"
#include "astree.h"
#include "semantic.h"
#include "tac.h"
#include "compiler.h"

extern int 	yyparse();
extern int  getLineNumber();
extern void initMe();
extern int  isRunning();


int string_to_file(char *filePath, char *string) {
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
#define COMPILATION_ERROR 5

int main(int argc, char **argv) {
	if (argc < 2) {
        printf("Error: invalid arguments.\nUsage: <input file> <output file (optional)>\n");
        return FILE_NOT_INFORMED;
    }

	FILE* file;
	if (!(file = fopen(argv[1], "r"))) {
		printf ("Error: file %s can't be opened.\n", argv[1]);
		return FILE_NOT_FOUND;
	}

	initMe();
	yyin = file;	
	yyparse();

	astree_print(tree, 0);

	semantic_set_declarations(tree);
	semantic_check(tree);

	fprintf(stderr, "Program accepted!\n");

	printf("Printing intermediary code from source file into stdout: %s...\n", argv[1]);
	tac_t *intermediary_code; 
	tac_print_backward(intermediary_code = tac_reverse(tac_generate(tree)));
	printf("Done!\n");

	if(argc == 3) {
		fprintf(stderr, "Printing assembly generated from %s into %s... ", argv[1], argv[2]);
		if(comp_asm_generate(intermediary_code, argv[2]) > 0) {
			printf("Compiler error: unknown intermediary code found.\n");
			return COMPILATION_ERROR;
		}
		printf("done!\n");
	}

	hash_print();

	return SUCCESS;
}