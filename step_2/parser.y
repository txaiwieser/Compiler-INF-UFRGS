%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "hash.h"

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

%union { hash_node_t *symbol; }

%token <symbol> LIT_INTEGER
%token <symbol> LIT_REAL
%token <symbol> LIT_CHAR
%token <symbol> LIT_STRING

%token <symbol> TK_IDENTIFIER
%token TOKEN_ERROR

%type <symbol> program
%type <symbol> declarationList
%type <symbol> declaration
%type <symbol> functionDeclaration
%type <symbol> variableDeclaration
%type <symbol> variableType
%type <symbol> variableTypeAndValue
%type <symbol> intList
%type <symbol> charList
%type <symbol> intRealList
%type <symbol> intReal
%type <symbol> param 
%type <symbol> parameterList
%type <symbol> parameters
%type <symbol> command
%type <symbol> commandList
%type <symbol> control
%type <symbol> expression
%type <symbol> attribute
%type <symbol> printList
%type <symbol> printElement
%type <symbol> arguments
%type <symbol> extraArgument

%left OPERATOR_AND OPERATOR_OR
%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_NE
%left '<' '>'
%left '-' '+'
%left '*' '/'

%nonassoc LOWER_THAN_ELSE
%nonassoc KW_ELSE

%%

program: declarationList { fprintf(stderr, "reduziu [declarationList] para [program]\n"); }
    ;

declarationList: declaration declarationList    { fprintf(stderr, "reduziu [declaration declarationList] para [declarationList]\n"); }
    |                                           { $$ = 0; }
    ;

declaration: functionDeclaration    { fprintf(stderr, "reduziu [functionDeclaration] para [declaration]\n"); }
    | variableDeclaration           { fprintf(stderr, "reduziu [variableDeclaration] para [declaration]\n"); }
    ;

variableDeclaration: TK_IDENTIFIER ':' variableTypeAndValue ';' { fprintf(stderr, "reduziu [TK_IDENTIFIER=%s : variableTypeAndValue ;] para [variableDeclaration]\n", $1->text); }
    ;

variableType: KW_BYTE { $$ = 0; }
    | KW_SHORT        { $$ = 0; }
    | KW_LONG         { $$ = 0; }
    | KW_FLOAT        { $$ = 0; }
    | KW_DOUBLE       { $$ = 0; }
    ;

variableTypeAndValue: KW_BYTE LIT_CHAR      { fprintf(stderr, "reduziu [KW_BYTE LIT_CHAR=%s] para [variableTypeAndValue]\n", $2->text); }
    | KW_BYTE LIT_INTEGER                   { fprintf(stderr, "reduziu [KW_BYTE LIT_INTEGER=%s] para [variableTypeAndValue]\n", $2->text); }
    | KW_SHORT LIT_INTEGER                  { fprintf(stderr, "reduziu [KW_SHORT LIT_INTEGER=%s] para [variableTypeAndValue]\n", $2->text); }
    | KW_LONG LIT_INTEGER                   { fprintf(stderr, "reduziu [KW_LONG LIT_INTEGER=%s] para [variableTypeAndValue]\n", $2->text); }
    | KW_FLOAT LIT_REAL                     { fprintf(stderr, "reduziu [KW_FLOAT LIT_REAL=%s] para [variableTypeAndValue]\n", $2->text); }
    | KW_FLOAT LIT_INTEGER                  { fprintf(stderr, "reduziu [KW_FLOAT LIT_INTEGER=%s] para [variableTypeAndValue]\n", $2->text); }
    | KW_DOUBLE LIT_INTEGER                 { fprintf(stderr, "reduziu [KW_DOUBLE LIT_INTEGER=%s] para [variableTypeAndValue]\n", $2->text); }
    | KW_DOUBLE LIT_REAL                    { fprintf(stderr, "reduziu [KW_DOUBLE LIT_REAL=%s] para [variableTypeAndValue]\n", $2->text); }
    | KW_BYTE '[' LIT_INTEGER ']' intList       { fprintf(stderr, "reduziu [KW_BYTE [ LIT_INTEGER ] intList] para [variableTypeAndValue]\n"); }
    | KW_BYTE '[' LIT_INTEGER ']' charList      { fprintf(stderr, "reduziu [KW_BYTE [ LIT_INTEGER ] charList] para [variableTypeAndValue]\n"); }
    | KW_SHORT '[' LIT_INTEGER ']' intList      { fprintf(stderr, "reduziu [KW_SHORT [ LIT_INTEGER ] intList] para [variableTypeAndValue]\n"); }
    | KW_LONG '[' LIT_INTEGER ']' intList       { fprintf(stderr, "reduziu [KW_LONG [ LIT_INTEGER ] intList] para [variableTypeAndValue]\n"); }
    | KW_FLOAT '[' LIT_INTEGER ']' intRealList  { fprintf(stderr, "reduziu [KW_FLOAT [ LIT_INTEGER ] intRealList] para [variableTypeAndValue]\n"); }
    | KW_DOUBLE '[' LIT_INTEGER ']' intRealList { fprintf(stderr, "reduziu [KW_DOUBLE [ LIT_INTEGER ] intRealList] para [variableTypeAndValue]\n"); }
    | KW_BYTE '[' LIT_INTEGER ']'               { fprintf(stderr, "reduziu [KW_BYTE [ LIT_INTEGER ]] para [variableTypeAndValue]\n"); }
    | KW_SHORT '[' LIT_INTEGER ']'              { fprintf(stderr, "reduziu [KW_SHORT [ LIT_INTEGER ]] para [variableTypeAndValue]\n"); }
    | KW_LONG '[' LIT_INTEGER ']'               { fprintf(stderr, "reduziu [KW_LONG [ LIT_INTEGER ]] para [variableTypeAndValue]\n"); }
    | KW_FLOAT '[' LIT_INTEGER ']'              { fprintf(stderr, "reduziu [KW_FLOAT [ LIT_INTEGER ]] para [variableTypeAndValue]\n"); }
    | KW_DOUBLE '[' LIT_INTEGER ']'             { fprintf(stderr, "reduziu [KW_DOUBLE [ LIT_INTEGER ]] para [variableTypeAndValue]\n"); }
    ;

intList: LIT_INTEGER intList    { fprintf(stderr, "reduziu [LIT_INTEGER=%s intList] para [intList]\n", $1->text); }
    | LIT_INTEGER               { fprintf(stderr, "reduziu [LIT_INTEGER=%s] para [intList]\n", $1->text); }
    ;

charList: LIT_CHAR charList     { fprintf(stderr, "reduziu [LIT_CHAR=%s charList] para [charList]\n", $1->text); }
    | LIT_CHAR                  { fprintf(stderr, "reduziu [LIT_CHAR=%s] para [charList]\n", $1->text); }
    ;

intRealList: intReal intRealList    { fprintf(stderr, "reduziu [intReal=%s intRealList] para [intRealList]\n", $1->text); }
    | intReal           { fprintf(stderr, "reduziu [intReal=%s] para [intRealList]\n", $1->text); }
    ;

intReal: LIT_INTEGER    { fprintf(stderr, "reduziu [LIT_INTEGER=%s] para [intReal]\n", $1->text); }
    | LIT_REAL          { fprintf(stderr, "reduziu [LIT_REAL=%s] para [intReal]\n", $1->text); }
     
functionDeclaration: variableType TK_IDENTIFIER '(' parameters ')' command ';' { fprintf(stderr, "reduziu [variableType TK_IDENTIFIER=%s ( parameters ) command ;] para [functionDeclaration]\n", $2->text); }
    ;

parameters: parameterList   { fprintf(stderr, "reduziu [parameterList] para [parameters]\n"); }
    |                       { $$ = 0; }
    ;

parameterList: param ',' parameterList  { fprintf(stderr, "reduziu [param=%s , parameterList] para [parameterList]\n", $1->text); }
    | param                             { fprintf(stderr, "reduziu [param=%s] para [parameterList]\n", $1->text); }
    ;    

param: variableType TK_IDENTIFIER   { $$ = $2; fprintf(stderr, "reduziu [variableType TK_IDENTIFIER=%s] para [param]\n", $2->text); }
    ;

commandList: commandList command ';'    { fprintf(stderr, "reduziu [commandList command ;] para [commandList]\n"); }
    |                                   { $$ = 0; }
    ;

command: attribute          { fprintf(stderr, "reduziu [attribute] para [command]\n"); }
    | '{' commandList '}'   { fprintf(stderr, "reduziu [{ commandList }] para [command]\n"); }
    | KW_READ TK_IDENTIFIER { fprintf(stderr, "reduziu [KW_READ TK_IDENTIFIER=%s] para [command]\n", $2->text); }
    | KW_PRINT printList    { fprintf(stderr, "reduziu [KW_READ printList] para [command]\n"); }
    | KW_RETURN expression  { fprintf(stderr, "reduziu [KW_RETURN expression] para [command]\n"); }
    | control
    |                       { $$ = 0; }
    ;

printList: printElement printList   { fprintf(stderr, "reduziu [printElement printList] para [printList]\n"); }
    | printElement                  { fprintf(stderr, "reduziu [printElement] para [printList]\n"); }
    ;

printElement: LIT_STRING            { fprintf(stderr, "reduziu [LIT_STRING=%s] para [printElement]\n", $1->text); }
    | expression                    { fprintf(stderr, "reduziu [expression] para [printElement]\n"); }
    ;

control: KW_WHEN '(' expression ')' KW_THEN command %prec LOWER_THAN_ELSE   { fprintf(stderr, "reduziu [KW_WHEN ( expression ) KW_THEN command] para [control]\n"); }
    | KW_WHEN '(' expression ')' KW_THEN command KW_ELSE command            { fprintf(stderr, "reduziu [KW_WHEN ( expression ) KW_THEN command KW_ELSE] para [control]\n"); }
    | KW_WHILE '(' expression ')' command                                   { fprintf(stderr, "reduziu [KW_WHILE ( expression ) command] para [control]\n"); }
    | KW_FOR '(' TK_IDENTIFIER '=' expression KW_TO expression ')' command  { fprintf(stderr, "reduziu [KW_FOR ( TK_IDENTIFIER=%s = expression KW_TO expression ) command] para [control]\n", $3->text); }
    ;

attribute: TK_IDENTIFIER '=' expression             { fprintf(stderr, "reduziu [TK_IDENTIFIER=%s '=' expression] para [attribute]\n", $1->text); }
    | TK_IDENTIFIER '#' expression '=' expression   { fprintf(stderr, "reduziu [TK_IDENTIFIER=%s '#' expression '=' expression] para [attribute]\n", $1->text); }
    ;


expression:  '(' expression ')'             { fprintf(stderr, "reduziu [(expression)] para [expression]\n"); }      
    | TK_IDENTIFIER                         { fprintf(stderr, "reduziu [TK_IDENTIFIER=%s] para [expression]\n", $1->text); }      
    | TK_IDENTIFIER '[' expression ']'      { fprintf(stderr, "reduziu [TK_IDENTIFIER=%s [expression]] para [expression]\n", $1->text); }      
    | TK_IDENTIFIER '(' arguments ')'       { fprintf(stderr, "reduziu [TK_IDENTIFIER=%s] [arguments]] para [expression]\n", $1->text); }      
    | LIT_INTEGER                           { fprintf(stderr, "reduziu [LIT_INTEGER=%s] para [expression]\n", $1->text); }
    | LIT_CHAR                              { fprintf(stderr, "reduziu [LIT_CHAR=%s] para [expression]\n", $1->text); }
    | LIT_REAL                              { fprintf(stderr, "reduziu [LIT_REAL=%s] para [expression]\n", $1->text); }
    | expression OPERATOR_LE expression     { fprintf(stderr, "reduziu [OPERATOR_LE] para [expression]\n"); }
    | expression OPERATOR_GE expression     { fprintf(stderr, "reduziu [OPERATOR_GE] para [expression]\n"); }
    | expression OPERATOR_EQ expression     { fprintf(stderr, "reduziu [OPERATOR_EQ] para [expression]\n"); }
    | expression OPERATOR_NE expression     { fprintf(stderr, "reduziu [OPERATOR_NE] para [expression]\n"); }
    | expression OPERATOR_AND expression    { fprintf(stderr, "reduziu [OPERATOR_AND] para [expression]\n"); }
    | expression OPERATOR_OR expression     { fprintf(stderr, "reduziu [OPERATOR_OR] para [expression]\n"); }
    | expression '+' expression             { fprintf(stderr, "reduziu [expression + expression] para [expression]\n"); }
    | expression '-' expression             { fprintf(stderr, "reduziu [expression - expression] para [expression]\n"); }
    | expression '*' expression             { fprintf(stderr, "reduziu [expression * expression] para [expression]\n"); }
    | expression '/' expression             { fprintf(stderr, "reduziu [expression / expression] para [expression]\n"); }
    | expression '<' expression             { fprintf(stderr, "reduziu [expression < expression] para [expression]\n"); }
    | expression '>' expression             { fprintf(stderr, "reduziu [expression > expression] para [expression]\n"); }
    ;
    
arguments: expression extraArgument         { fprintf(stderr, "reduziu [expression extraArgument] para [arguments]\n"); } 
    |                                       { $$ = 0; }
    ;

extraArgument: ',' expression extraArgument { fprintf(stderr, "reduziu [, expression extraArgument] para [extraArgument]\n"); } 
    |                                       { $$ = 0; }
    ;

%%

void yyerror(char *s) {
   fprintf(stderr, "\nErro na linha %d! \n\n", getLineNumber());
    exit(3);
}
