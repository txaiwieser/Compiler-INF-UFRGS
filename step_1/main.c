#include <stdio.h>
#include "lex.yy.h"
#include "tokens.h"
#include "hash.h"

extern int getLineNumber();
extern void initMe();
extern int isRunning();

int main(int argc, char ** argv) {
	FILE* file;
	if (!(file = fopen("test.txt", "r"))) {
		printf ("Erro ao abrir arquivo!");
	}
	yyin = file;
	initMe();
	while(isRunning()) {
		switch (yylex()) {

			case KW_BYTE:		printf("KW_BYTE at line %d\n", getLineNumber());	break;
			case KW_SHORT:		printf("KW_SHORT at line %d\n", getLineNumber());	break;
			case KW_LONG:		printf("KW_LONG at line %d\n", getLineNumber());	break;
			case KW_FLOAT:		printf("KW_FLOAT at line %d\n", getLineNumber());	break;
			case KW_DOUBLE:		printf("KW_DOUBLE at line %d\n", getLineNumber());	break;
			case KW_WHEN:		printf("KW_WHEN at line %d\n", getLineNumber());	break;
			case KW_THEN:		printf("KW_THEN at line %d\n", getLineNumber());	break;
			case KW_ELSE:		printf("KW_ELSE at line %d\n", getLineNumber());	break;
			case KW_WHILE:		printf("KW_WHILE at line %d\n", getLineNumber());	break;
			case KW_FOR:		printf("KW_FOR	 at line %d\n", getLineNumber());	break;
			case KW_READ:		printf("KW_READ at line %d\n", getLineNumber());	break;
			case KW_RETURN:		printf("KW_RETURN at line %d\n", getLineNumber());	break;
			case KW_PRINT:		printf("KW_PRINT at line %d\n", getLineNumber());	break;

			case TOKEN_ERROR: 	printf("TOKEN_ERROR (%s) at line %d\n", yytext, getLineNumber());	break;
			default:			printf("TOKEN (%s) at line %d\n", yytext, getLineNumber());			break;

		}
	}
	hashPrint();
}
