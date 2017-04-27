#include <stdio.h>
#include "lex.yy.h"
#include "hash.h"

extern int 	yyparse();
extern int  getLineNumber();
extern void initMe();
extern int  isRunning();

int main(int argc, char ** argv) {
	FILE* file;
	if (!(file = fopen("sample.txt", "r"))) {
		printf ("Erro ao abrir arquivo!");
		return 0;
	}
	initMe();
	yyin = file;
	
	yyparse();

	fprintf(stderr, "Programa aceito!\n");

	// hashPrint();

	return 1;
}
