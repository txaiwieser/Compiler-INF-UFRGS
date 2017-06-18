%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "astree.h"
    #include "semantic.h"
    
    void yyerror(char *s);
    int  yylex(void);
    int  getLineNumber(void);
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
%type <astree> variableType
%type <astree> variableTypeAndValue
%type <astree> intList
%type <astree> charList
%type <astree> floatList
%type <astree> param 
%type <astree> parameterList
%type <astree> parameters
%type <astree> command
%type <astree> commandList
%type <astree> control
%type <astree> expression
%type <astree> attribute
%type <astree> printList
%type <astree> printElement
%type <astree> arguments
%type <astree> extraArgument



%left OPERATOR_AND OPERATOR_OR
%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_NE
%left '<' '>' '!'
%left '-' '+'
%left '*' '/'

%nonassoc LOWER_THAN_ELSE
%nonassoc KW_ELSE

%%

program: declarationList { tree = $1; fprintf(stderr, "reduziu [declarationList] para [program]\n"); }
    ;

declarationList: declaration declarationList    { $$ = astree_create(ASTREE_DECL_LIST, NULL, $1, $2, 0, 0); fprintf(stderr, "reduziu [declaration declarationList] para [declarationList]\n"); }
    |                                           { $$ = 0; }
    ;

declaration: functionDeclaration    { fprintf(stderr, "reduziu [functionDeclaration] para [declaration]\n"); }
    | variableDeclaration           { fprintf(stderr, "reduziu [variableDeclaration] para [declaration]\n"); }
    ;

variableDeclaration: TK_IDENTIFIER ':' variableTypeAndValue ';' { $$ = astree_create(ASTREE_VAR_DEC, $1, $3, 0, 0, 0); fprintf(stderr, "reduziu [TK_IDENTIFIER=%s : variableTypeAndValue ;] para [variableDeclaration]\n", $1->text); }
    ;

variableType: KW_BYTE   { $$ = astree_create(ASTREE_KW_BYTE, NULL, 0, 0, 0, 0); }
    | KW_SHORT          { $$ = astree_create(ASTREE_KW_SHORT, NULL, 0, 0, 0, 0); }
    | KW_LONG           { $$ = astree_create(ASTREE_KW_LONG, NULL, 0, 0, 0, 0); }
    | KW_FLOAT          { $$ = astree_create(ASTREE_KW_FLOAT, NULL, 0, 0, 0, 0); }
    | KW_DOUBLE         { $$ = astree_create(ASTREE_KW_DOUBLE, NULL, 0, 0, 0, 0); }  
	;

    variableTypeAndValue: variableType LIT_CHAR { $$ = astree_create(ASTREE_CHAR, $2, $1, 0, 0, 0); fprintf(stderr, "reduziu [variableType LIT_CHAR=%s] para [variableTypeAndValue]\n", $2->text); }
    | variableType LIT_INTEGER                  { $$ = astree_create(ASTREE_INT, $2, $1, 0, 0, 0); fprintf(stderr, "reduziu [variableType LIT_INTEGER=%s] para [variableTypeAndValue]\n", $2->text); }
    | variableType LIT_REAL                     { $$ = astree_create(ASTREE_REAL, $2, $1, 0, 0, 0); fprintf(stderr, "reduziu [variableType LIT_REAL=%s] para [variableTypeAndValue]\n", $2->text); }
    | variableType '[' LIT_INTEGER ']' intList       { $$ = astree_create(ASTREE_ARR_INT, $3, $1, $5, 0, 0); fprintf(stderr, "reduziu [variableType '[' LIT_INTEGER ']' intList] para [variableTypeAndValue]\n"); }
    | variableType '[' LIT_INTEGER ']' charList      { $$ = astree_create(ASTREE_ARR_CHAR, $3, $1, $5, 0, 0); fprintf(stderr, "reduziu [variableType '[' LIT_INTEGER ']' charList] para [variableTypeAndValue]\n"); }
    | variableType '[' LIT_INTEGER ']' floatList     { $$ = astree_create(ASTREE_ARR_FLOAT, $3, $1, $5, 0, 0); fprintf(stderr, "reduziu [variableType '[' LIT_INTEGER ']' intRealList] para [variableTypeAndValue]\n"); }
    | variableType '[' LIT_INTEGER ']'               { $$ = astree_create(ASTREE_ARR, $3, $1, 0, 0, 0); fprintf(stderr, "reduziu [variableType '[' LIT_INTEGER ']'] para [variableTypeAndValue]\n"); }
    ;

intList: LIT_INTEGER intList    { $$ = astree_create(ASTREE_INT_LST, $1, $2, 0, 0, 0); fprintf(stderr, "reduziu [LIT_INTEGER=%s intList] para [intList]\n", $1->text); }
    | LIT_INTEGER               { $$ = astree_create(ASTREE_LIT_INT, $1, 0, 0, 0, 0); fprintf(stderr, "reduziu [LIT_INTEGER=%s] para [intList]\n", $1->text); }
    ;

charList: LIT_CHAR charList     { $$ = astree_create(ASTREE_CHAR_LST, $1, $2, 0, 0, 0); fprintf(stderr, "reduziu [LIT_CHAR=%s charList] para [charList]\n", $1->text); }
    | LIT_CHAR                  { $$ = astree_create(ASTREE_LIT_CHAR, $1, 0, 0, 0, 0); fprintf(stderr, "reduziu [LIT_CHAR=%s] para [charList]\n", $1->text); }
    ;

floatList: LIT_REAL floatList   { $$ = astree_create(ASTREE_FLOAT_LST, $1, $2, 0, 0, 0); fprintf(stderr, "reduziu [LIT_REAL=%s charList] para [charList]\n", $1->text); }
    | LIT_REAL                  { $$ = astree_create(ASTREE_LIT_REAL, $1, 0, 0, 0, 0); fprintf(stderr, "reduziu [LIT_REAL=%s] para [charList]\n", $1->text); }
    ;
     
functionDeclaration: variableType TK_IDENTIFIER '(' parameters ')' command ';' { $$ = astree_create(ASTREE_FUNC_DEC, $2, $1, $4, $6, 0); fprintf(stderr, "reduziu [variableType TK_IDENTIFIER=%s ( parameters ) command ;] para [functionDeclaration]\n", $2->text); }
    ;

parameters: parameterList   { fprintf(stderr, "reduziu [parameterList] para [parameters]\n"); }
    |                       { $$ = 0; }
    ;

parameterList: param ',' parameterList  { $$ = astree_create(ASTREE_PARAM_LST, NULL, $1, $3, 0, 0); fprintf(stderr, "reduziu [param=%s , parameterList] para [parameterList]\n", $1->symbol->text); }
    | param                             { fprintf(stderr, "reduziu [param=%s] para [parameterList]\n", $1->symbol->text); }
    ;    

param: variableType TK_IDENTIFIER   { $$ = astree_create(ASTREE_PARAM, $2, $1, 0, 0, 0); fprintf(stderr, "reduziu [variableType TK_IDENTIFIER=%s] para [param]\n", $2->text); }
    ;

commandList: commandList command ';'    { $$ = astree_create(ASTREE_CMD_LST, NULL, $1, $2, 0, 0); fprintf(stderr, "reduziu [commandList command ;] para [commandList]\n"); }
    |                                   { $$ = 0; }
    ;

command: attribute          { fprintf(stderr, "reduziu [attribute] para [command]\n"); }
    | '{' commandList '}'   { $$ = astree_create(ASTREE_CMD_BKTS, NULL, $2, 0, 0, 0); fprintf(stderr, "reduziu [{ commandList }] para [command]\n"); }
    | KW_READ TK_IDENTIFIER { $$ = astree_create(ASTREE_KW_READ, $2, 0, 0, 0, 0); fprintf(stderr, "reduziu [KW_READ TK_IDENTIFIER=%s] para [command]\n", $2->text); }
    | KW_PRINT printList    { $$ = astree_create(ASTREE_KW_PRINT, NULL, $2, 0, 0, 0); fprintf(stderr, "reduziu [KW_READ printList] para [command]\n"); }
    | KW_RETURN expression  { $$ = astree_create(ASTREE_KW_RETURN, NULL, $2, 0, 0, 0); fprintf(stderr, "reduziu [KW_RETURN expression] para [command]\n"); }
    | control
    |                       { $$ = 0; }
    ;

printList: printElement printList   { $$ = astree_create(ASTREE_PRINT_LST, NULL, $1, $2, 0, 0); fprintf(stderr, "reduziu [printElement printList] para [printList]\n"); }
    | printElement                  { fprintf(stderr, "reduziu [printElement] para [printList]\n"); }
    ;

printElement: LIT_STRING            { $$ = astree_create(ASTREE_LIT_STRING, $1, 0, 0, 0, 0); fprintf(stderr, "reduziu [LIT_STRING=%s] para [printElement]\n", $1->text); }
    | expression                    { fprintf(stderr, "reduziu [expression] para [printElement]\n"); }
    ;

control: KW_WHEN '(' expression ')' KW_THEN command %prec LOWER_THAN_ELSE   { $$ = astree_create(ASTREE_KW_WHEN_THEN, NULL, $3, $6, 0, 0); fprintf(stderr, "reduziu [KW_WHEN ( expression ) KW_THEN command] para [control]\n"); }
    | KW_WHEN '(' expression ')' KW_THEN command KW_ELSE command            { $$ = astree_create(ASTREE_KW_WHEN_THEN_ELSE, NULL, $3, $6, $8, 0); fprintf(stderr, "reduziu [KW_WHEN ( expression ) KW_THEN command KW_ELSE] para [control]\n"); }
    | KW_WHILE '(' expression ')' command                                   { $$ = astree_create(ASTREE_KW_WHILE, NULL, $3, $5, 0, 0); fprintf(stderr, "reduziu [KW_WHILE ( expression ) command] para [control]\n"); }
    | KW_FOR '(' TK_IDENTIFIER '=' expression KW_TO expression ')' command  { $$ = astree_create(ASTREE_KW_FOR, $3, $5, $7, $9, 0); fprintf(stderr, "reduziu [KW_FOR ( TK_IDENTIFIER=%s = expression KW_TO expression ) command] para [control]\n", $3->text); }
    ;

attribute: TK_IDENTIFIER '=' expression             { $$ = astree_create(ASTREE_ATTRIB, $1, $3, 0, 0, 0); fprintf(stderr, "reduziu [TK_IDENTIFIER=%s '=' expression] para [attribute]\n", $1->text); }
    | TK_IDENTIFIER '#' expression '=' expression   { $$ = astree_create(ASTREE_ATTRIB_ARR, $1, $3, $5, 0, 0); fprintf(stderr, "reduziu [TK_IDENTIFIER=%s '#' expression '=' expression] para [attribute]\n", $1->text); }
    ;


expression:  '(' expression ')'             { $$ = astree_create(ASTREE_EXP_PARENTHESIS, NULL, $2, 0, 0, 0); fprintf(stderr, "reduziu [(expression)] para [expression]\n"); }      
    | TK_IDENTIFIER                         { $$ = astree_create(ASTREE_TK_ID, $1, 0, 0, 0, 0); fprintf(stderr, "reduziu [TK_IDENTIFIER=%s] para [expression]\n", $1->text); }      
    | TK_IDENTIFIER '[' expression ']'      { $$ = astree_create(ASTREE_ARRAY_CALL, $1, $3, 0, 0, 0); fprintf(stderr, "reduziu [TK_IDENTIFIER=%s [expression]] para [expression]\n", $1->text); }      
    | TK_IDENTIFIER '(' arguments ')'       { $$ = astree_create(ASTREE_FUNC_CALL, $1, $3, 0, 0, 0); fprintf(stderr, "reduziu [TK_IDENTIFIER=%s] [arguments]] para [expression]\n", $1->text); }      
    | LIT_INTEGER                           { $$ = astree_create(ASTREE_LIT_INT, $1, 0, 0, 0, 0); fprintf(stderr, "reduziu [LIT_INTEGER=%s] para [expression]\n", $1->text); }
    | LIT_CHAR                              { $$ = astree_create(ASTREE_LIT_CHAR, $1, 0, 0, 0, 0); fprintf(stderr, "reduziu [LIT_CHAR=%s] para [expression]\n", $1->text); }
    | LIT_REAL                              { $$ = astree_create(ASTREE_LIT_REAL, $1, 0, 0, 0, 0); fprintf(stderr, "reduziu [LIT_REAL=%s] para [expression]\n", $1->text); }
    | expression OPERATOR_LE expression     { $$ = astree_create(ASTREE_LEQ, NULL, $1, $3, 0, 0); fprintf(stderr, "reduziu [OPERATOR_LE] para [expression]\n"); }
    | expression OPERATOR_GE expression     { $$ = astree_create(ASTREE_GTE, NULL, $1, $3, 0, 0); fprintf(stderr, "reduziu [OPERATOR_GE] para [expression]\n"); }
    | expression OPERATOR_EQ expression     { $$ = astree_create(ASTREE_EQU, NULL, $1, $3, 0, 0); fprintf(stderr, "reduziu [OPERATOR_EQ] para [expression]\n"); }
    | expression OPERATOR_NE expression     { $$ = astree_create(ASTREE_NEQ, NULL, $1, $3, 0, 0); fprintf(stderr, "reduziu [OPERATOR_NE] para [expression]\n"); }
    | expression OPERATOR_AND expression    { $$ = astree_create(ASTREE_AND, NULL, $1, $3, 0, 0); fprintf(stderr, "reduziu [OPERATOR_AND] para [expression]\n"); }
    | expression OPERATOR_OR expression     { $$ = astree_create(ASTREE_OR, NULL, $1, $3, 0, 0); fprintf(stderr, "reduziu [OPERATOR_OR] para [expression]\n"); }
    | expression '+' expression             { $$ = astree_create(ASTREE_ADD, NULL, $1, $3, 0, 0); fprintf(stderr, "reduziu [expression + expression] para [expression]\n"); }
    | expression '-' expression             { $$ = astree_create(ASTREE_SUB, NULL, $1, $3, 0, 0); fprintf(stderr, "reduziu [expression - expression] para [expression]\n"); }
    | expression '*' expression             { $$ = astree_create(ASTREE_MUL, NULL, $1, $3, 0, 0); fprintf(stderr, "reduziu [expression * expression] para [expression]\n"); }
    | expression '/' expression             { $$ = astree_create(ASTREE_DIV, NULL, $1, $3, 0, 0); fprintf(stderr, "reduziu [expression / expression] para [expression]\n"); }
    | expression '<' expression             { $$ = astree_create(ASTREE_LES, NULL, $1, $3, 0, 0); fprintf(stderr, "reduziu [expression < expression] para [expression]\n"); }
    | expression '>' expression             { $$ = astree_create(ASTREE_GTR, NULL, $1, $3, 0, 0); fprintf(stderr, "reduziu [expression > expression] para [expression]\n"); }
    ;
	
arguments: expression extraArgument         { $$ = astree_create(ASTREE_FUNC_ARGS, NULL, $1, $2, 0, 0); fprintf(stderr, "reduziu [expression extraArgument] para [arguments]\n"); } 
    |                                       { $$ = 0; }
    ;

extraArgument: ',' expression extraArgument { $$ = astree_create(ASTREE_FUNC_ARGS_EXT, NULL, $2, $3, 0, 0); fprintf(stderr, "reduziu [, expression extraArgument] para [extraArgument]\n"); } 
    |                                       { $$ = 0; }
    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "\nSyntax error at line number: %d.\n\n", getLineNumber());
    exit(3);
}
