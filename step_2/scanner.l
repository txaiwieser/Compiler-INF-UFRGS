
%{

#include "hash.h"
#include "y.tab.h"

int nLines	= 1;
int running	= 1;

%}

%x COMMENT

%%

[ \t]			{ }
byte			{ return KW_BYTE; }
short			{ return KW_SHORT; }
long			{ return KW_LONG; }
float			{ return KW_FLOAT; }		
double			{ return KW_DOUBLE; }
when			{ return KW_WHEN; }
then			{ return KW_THEN; }
else			{ return KW_ELSE; }
while			{ return KW_WHILE; }
for				{ return KW_FOR; }
to				{ return KW_TO; }
read			{ return KW_READ; }
return			{ return KW_RETURN; }
print			{ return KW_PRINT; }

"<="			{ return OPERATOR_LE; }
">="			{ return OPERATOR_GE; }
"=="			{ return OPERATOR_EQ; }
"!="			{ return OPERATOR_NE; }
"&&"			{ return OPERATOR_AND; }
"||"			{ return OPERATOR_OR; }

[A-Za-z_][A-Za-z_0-9]*						{ hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER; }
[0-9]+										{ hashInsert(yytext, LIT_INTEGER); return LIT_INTEGER; }
[0-9]+"."[0-9]+								{ hashInsert(yytext, LIT_REAL); return LIT_REAL; }
'.'  			               				{ hashInsert(yytext, LIT_CHAR); return LIT_CHAR; }
\"(\\.|[^"])*\"                  			{ hashInsert(yytext, LIT_STRING); return LIT_STRING; }

[\-,\;\:\(\)\[\]\{\}\+\*\/\<\>\=\!\&\$\#]	{ return yytext[0]; }

"\n"										{ nLines++; }	
"//".*

"/*"			{ BEGIN(COMMENT); }
<COMMENT>"*/"	{ BEGIN(INITIAL); }
<COMMENT>"\n"	{ nLines++; }
<COMMENT>"\t"	{ }
<COMMENT>.
.				{ return TOKEN_ERROR; }

%%

int 	yywrap() 			{ running = 0; return 1; }
int		isRunning(void) 	{ return running; }
int		getLineNumber(void) { return nLines; }
void 	initMe(void)		{ hashInit(); }