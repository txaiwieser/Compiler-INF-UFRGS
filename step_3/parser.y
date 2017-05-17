%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "astree.h"

    void yyerror(char *s);
    int  yylex(void);
    int  getLineNumber(void);

    astree_t astree;
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

%union { hash_node_t *symbol; struct astree_s *astree; }

%token <symbol> LIT_INTEGER
%token <symbol> LIT_REAL
%token <symbol> LIT_CHAR
%token <symbol> LIT_STRING

%token <symbol> TK_IDENTIFIER
%token TOKEN_ERROR

%type <astree> program
%type <astree> declarationList
%type <astree> declaration
%type <astree> functionDeclaration
%type <astree> variableDeclaration
%type <astree> variableTypeAndValue
%type <astree> intList
%type <astree> charList
%type <astree> intRealList
%type <astree> intReal

%left OPERATOR_AND OPERATOR_OR
%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_NE
%left '<' '>' '!'
%left '-' '+'
%left '*' '/'

%nonassoc LOWER_THAN_ELSE
%nonassoc KW_ELSE

%%

program: declarationList { astree = *$1; fprintf(stderr, "reduziu [declarationList] para [program]\n"); }
    ;

declarationList: declaration declarationList
    | declaration                               { fprintf(stderr, "reduziu [declaration] para [declarationList]\n"); }
    ;

declaration: functionDeclaration
    | variableDeclaration           { fprintf(stderr, "reduziu [variableDeclaration] para [declaration]\n"); }
    ;

variableDeclaration: TK_IDENTIFIER ':' variableTypeAndValue ';' { $$ = astree_create(ASTREE_VAR_DEC, $1, $3, 0, 0, 0); fprintf(stderr, "reduziu [TK_IDENTIFIER=%s : variableTypeAndValue ;] para [variableDeclaration]\n", $1->text); }
    ;

variableType: KW_BYTE
    | KW_SHORT
    | KW_LONG
    | KW_FLOAT
    | KW_DOUBLE
	;

variableTypeAndValue: KW_BYTE LIT_CHAR      { $$ = astree_create(ASTREE_BYTE_CHAR, $2, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_BYTE LIT_CHAR=%s] para [variableTypeAndValue]", $2->text); }
    | KW_BYTE LIT_INTEGER                   { $$ = astree_create(ASTREE_BYTE_INT, $2, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_BYTE LIT_INTEGER=%s] para [variableTypeAndValue]", $2->text); }
    | KW_SHORT LIT_INTEGER                  { $$ = astree_create(ASTREE_SHORT_INT, $2, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_SHORT LIT_INTEGER=%s] para [variableTypeAndValue]", $2->text); }
    | KW_LONG LIT_INTEGER                   { $$ = astree_create(ASTREE_LONG_INT, $2, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_LONG LIT_INTEGER=%s] para [variableTypeAndValue]", $2->text); }
    | KW_FLOAT LIT_REAL                     { $$ = astree_create(ASTREE_FLOAT_REAL, $2, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_FLOAT LIT_REAL=%s] para [variableTypeAndValue]", $2->text); }
    | KW_FLOAT LIT_INTEGER                  { $$ = astree_create(ASTREE_FLOAT_INT, $2, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_FLOAT LIT_INTEGER=%s] para [variableTypeAndValue]", $2->text); }
    | KW_DOUBLE LIT_INTEGER                 { $$ = astree_create(ASTREE_DOUBLE_INT, $2, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_DOUBLE LIT_INTEGER=%s] para [variableTypeAndValue]", $2->text); }
    | KW_DOUBLE LIT_REAL                    { $$ = astree_create(ASTREE_DOUBLE_REAL, $2, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_DOUBLE LIT_REAL=%s] para [variableTypeAndValue]", $2->text); }
    | KW_BYTE '[' LIT_INTEGER ']' intList       { $$ = astree_create(ASTREE_BYTE_ARR_INT, $3, $5, 0, 0, 0); fprintf(stderr, "reduziu [KW_BYTE '[' LIT_INTEGER ']' intList] para [variableTypeAndValue]\n"); }
    | KW_BYTE '[' LIT_INTEGER ']' charList      { $$ = astree_create(ASTREE_BYTE_ARR_CHAR, $3, $5, 0, 0, 0); fprintf(stderr, "reduziu [KW_BYTE '[' LIT_INTEGER ']' charList] para [variableTypeAndValue]\n"); }
    | KW_SHORT '[' LIT_INTEGER ']' intList      { $$ = astree_create(ASTREE_SHORT_ARR_INT, $3, $5, 0, 0, 0); fprintf(stderr, "reduziu [KW_SHORT '[' LIT_INTEGER ']' intList] para [variableTypeAndValue]\n"); }
    | KW_LONG '[' LIT_INTEGER ']' intList       { $$ = astree_create(ASTREE_LONG_ARR_INT, $3, $5, 0, 0, 0); fprintf(stderr, "reduziu [KW_LONG '[' LIT_INTEGER ']' intList] para [variableTypeAndValue]\n"); }
    | KW_FLOAT '[' LIT_INTEGER ']' intRealList  { $$ = astree_create(ASTREE_FLOAT_ARR_RI, $3, $5, 0, 0, 0); fprintf(stderr, "reduziu [KW_FLOAT '[' LIT_INTEGER ']' intRealList] para [variableTypeAndValue]\n"); }
    | KW_DOUBLE '[' LIT_INTEGER ']' intRealList { $$ = astree_create(ASTREE_DOUBLE_ARR_RI, $3, $5, 0, 0, 0); fprintf(stderr, "reduziu [KW_DOUBLE '[' LIT_INTEGER ']' intRealList] para [variableTypeAndValue]\n"); }
    | KW_BYTE '[' LIT_INTEGER ']'               { $$ = astree_create(ASTREE_BYTE_ARR, $3, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_BYTE '[' LIT_INTEGER ']'] para [variableTypeAndValue]\n"); }
    | KW_SHORT '[' LIT_INTEGER ']'              { $$ = astree_create(ASTREE_SHORT_ARR, $3, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_SHORT '[' LIT_INTEGER ']'] para [variableTypeAndValue]\n"); }
    | KW_LONG '[' LIT_INTEGER ']'               { $$ = astree_create(ASTREE_LONG_ARR, $3, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_LONG '[' LIT_INTEGER ']'] para [variableTypeAndValue]\n"); }
    | KW_FLOAT '[' LIT_INTEGER ']'              { $$ = astree_create(ASTREE_FLOAT_ARR, $3, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_FLOAT '[' LIT_INTEGER ']'] para [variableTypeAndValue]\n"); }
    | KW_DOUBLE '[' LIT_INTEGER ']'             { $$ = astree_create(ASTREE_DOUBLE_ARR, $3, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_DOUBLE '[' LIT_INTEGER ']'] para [variableTypeAndValue]\n"); }
    ;

intList: LIT_INTEGER intList    { $$ = astree_create(ASTREE_INT_LST, $1, $2, 0, 0, 0); fprintf(stderr, "reduziu [LIT_INTEGER=%s intList ] para intList\n", $1->text); }
    | LIT_INTEGER               { $$ = astree_create(ASTREE_LIT_INT, $1, 0, 0, 0, 0); fprintf(stderr, "reduziu [LIT_INTEGER=%s] para [intList]\n", $1->text); }
    ;

charList: LIT_CHAR charList     { $$ = astree_create(ASTREE_CHAR_LST, $1, $2, 0, 0, 0); fprintf(stderr, "reduziu [LIT_CHAR=%s charList ] para charList\n", $1->text); }
    | LIT_CHAR                  { $$ = astree_create(ASTREE_LIT_CHAR, $1, 0, 0, 0, 0); fprintf(stderr, "reduziu [LIT_CHAR=%s] para [charList]\n", $1->text); }
    ;

intRealList: intReal intRealList    { $$ = astree_create(ASTREE_FLOAT_LST, NULL, $1, $2, 0, 0); fprintf(stderr, "reduziu [intReal=%s intRealList] para [intRealList]\n", $1->symbol->text); }
    | intReal           { fprintf(stderr, "reduziu [intReal=%s] para [intRealList]\n", $1->symbol->text); }
    ;

intReal: LIT_INTEGER    { $$ = astree_create(ASTREE_LIT_INT, $1, 0, 0, 0, 0); fprintf(stderr, "reduziu [LIT_INTEGER=%s] para [intReal]\n", $1->text); }
    | LIT_REAL          { $$ = astree_create(ASTREE_LIT_REAL, $1, 0, 0, 0, 0); fprintf(stderr, "reduziu [LIT_REAL=%s] para [intReal]\n", $1->text); }

parameterList: param ',' parameterList | param
    ;

param: variableType TK_IDENTIFIER
    ;
     
functionDeclaration: variableType TK_IDENTIFIER '(' parameters ')' command ';' { astree_t *node; $$ = node; }
    ;

parameters: parameterList
    |
    ;

commandList: commandList command ';'
    |
    ;

command: attribute
    | '{' commandList '}'
    | KW_READ TK_IDENTIFIER
    | KW_PRINT printList
    | KW_RETURN expression
    | control
    |
    ;

printList: printElement printList | printElement
    ;

printElement: LIT_STRING | expression
    ;

control: KW_WHEN '(' expression ')' KW_THEN command %prec LOWER_THAN_ELSE
    | KW_WHEN '(' expression ')' KW_THEN command KW_ELSE command
    | KW_WHILE '(' expression ')' command
    | KW_FOR '(' TK_IDENTIFIER '=' expression KW_TO expression ')' command
    ;

attribute: TK_IDENTIFIER '=' expression
    | TK_IDENTIFIER '#' expression '=' expression
    ;


expression:  '(' expression ')'
    | TK_IDENTIFIER
    | TK_IDENTIFIER '[' expression ']'
    | TK_IDENTIFIER '(' arguments ')'
    | LIT_INTEGER
    | LIT_CHAR
    | LIT_REAL
    | expression operator expression
    ;
	
arguments: expression extraArgument
    |
    ;

extraArgument: ',' expression extraArgument
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
    fprintf(stderr, "\nErro na linha %d!\n\n", getLineNumber());
    exit(3);
}
