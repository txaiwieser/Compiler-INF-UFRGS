%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "hash.h"
	void yyerror(char *s);
%}

%token KW_BYTE
%token KW_SHORT
%token KW_LONG
%token KW_FLOAT
%token KW_DOUBLE
%token KW_WHEN
%token KW_THEN
%token KW_ELSE
%token KW_WHILE
%token KW_FOR
%token KW_TO
%token KW_READ
%token KW_RETURN
%token KW_PRINT

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_NE
%token OPERATOR_AND
%token OPERATOR_OR

%token <symbol> TK_IDENTIFIER

%token <symbol> LIT_INTEGER
%token <symbol> LIT_REAL
%token <symbol> LIT_CHAR
%token <symbol> LIT_STRING

%token TOKEN_ERROR

%left '-' '+'
%left '*' '/'

%%

program: declarationList
    ;

declarationList: declaration declarationList
    |
    ;

declaration: functionDeclaration | variableDeclaration
    ;

variableDeclaration: TK_IDENTIFIER ':' variableTypeAndValue ';'

variableType: KW_BYTE | KW_SHORT | KW_LONG | KW_FLOAT | KW_DOUBLE
	;

variableTypeAndValue: KW_BYTE LIT_CHAR
    | KW_BYTE LIT_INTEGER
    | KW_SHORT LIT_INTEGER
    | KW_LONG LIT_INTEGER
    | KW_FLOAT LIT_REAL
    | KW_DOUBLE LIT_INTEGER
    | KW_BYTE '[' LIT_INTEGER ']' intList
    | KW_BYTE '[' LIT_INTEGER ']' charList
    | KW_SHORT '[' LIT_INTEGER ']' intList
    | KW_LONG '[' LIT_INTEGER ']' intList
    | KW_FLOAT '[' LIT_INTEGER ']' floatList
    | KW_DOUBLE '[' LIT_INTEGER ']' intList
    ;

intList: LIT_INTEGER intList
    |
    ;

charList: LIT_CHAR charList
    |
    ;

floatList: LIT_REAL floatList
    |
    ;

parameterList: variableType TK_IDENTIFIER ',' parameterList | variableType TK_IDENTIFIER
	|
    ;

functionDeclaration: variableType TK_IDENTIFIER '(' parameterList ')' functionBody
	;

functionBody: command
	;

commandList: command
	| commandList command
	;

command: attribute ';'
    |
    | '{' commandList '}' ';'
    | ';'
    | control
	| KW_READ TK_IDENTIFIER ';'
    | KW_PRINT printList ';'
	| KW_RETURN expression ';'
    ;

printList: printElement printList | printElement
    ;

printElement: LIT_STRING | expression
    ;

control: KW_WHEN '(' expression ')' KW_THEN command
    | KW_WHEN '(' expression ')' KW_THEN command KW_ELSE command
    | KW_WHILE '(' expression ')' command
    | KW_FOR '(' TK_IDENTIFIER '=' expression KW_TO expression ')' command
    ;

attribute: TK_IDENTIFIER '=' expression
    | TK_IDENTIFIER'#'expression '=' expression
    ;


expression:  '(' expression ')'
    | TK_IDENTIFIER
    | TK_IDENTIFIER '[' expression ']'
    | TK_IDENTIFIER '(' args ')'
    | LIT_INTEGER
    | LIT_CHAR
    | LIT_REAL
    | expression operator expression
    ;
	
args: expression argsTail
    |
    ;

argsTail: ',' expression argsTail
	|
	;

operator: OPERATOR_LE
    | OPERATOR_GE
    | OPERATOR_EQ
    | OPERATOR_NE
    | OPERATOR_AND
    | OPERATOR_OR
    | '*'
    | '+'
    | '-'
    | '/'
    | '<'
    | '>'
    | '!' 
	;



%%

void yyerror(char *s) {
	fprintf(stderr, "Sintax Error\n");
    exit(3);
}
