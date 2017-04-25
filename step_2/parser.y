%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "hash.h"
	void yyerror(char *s);
%}

%%

program:
	;

%%

void yyerror(char *s) {
    exit(3);
}
