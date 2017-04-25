#include <stdio.h>
#include "lex.yy.h"
#include "tokens.h"
#include "hash.h"

extern int getLineNumber();
extern void initMe();
extern int isRunning();

int main(int argc, char ** argv) {
	FILE* file;
	if (!(file = fopen("sample.txt", "r"))) {
		printf ("Erro ao abrir arquivo!");
		return 0;
	}
	initMe();

	yyparse();

	fprintf(stderr, "Programa Aceito\n");

	return 1;
}
