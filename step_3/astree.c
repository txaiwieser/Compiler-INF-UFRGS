#include <stdio.h>
#include "astree.h"
#include "strings.h"

astree_t* astree_create(int type, hash_node_t *symbol, astree_t *son_1, astree_t *son_2, astree_t *son_3, astree_t *son_4) {

    astree_t* newnode = 0;
    newnode = (astree_t*) calloc(1, sizeof(astree_t));
    
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = son_1;
    newnode->son[1] = son_2;
    newnode->son[2] = son_3;
    newnode->son[3] = son_4;

    return newnode;
}

void astree_print(astree_t* node, int level)
{
    int i;

    if(!node) return;

    for(i=0; i < level; i++)
        fprintf(stderr, "  ");

    fprintf(stderr, "ASTREE(");
    switch (node->type) {
        case ASTREE_DECL_LIST: fprintf(stderr,"ASTREE_DECL_LIST"); break;

        case ASTREE_VAR_DEC: fprintf(stderr,"ASTREE_VAR_DEC"); break;
        case ASTREE_BYTE_CHAR: fprintf(stderr,"ASTREE_BYTE_CHAR"); break;
        case ASTREE_BYTE_INT: fprintf(stderr,"ASTREE_BYTE_INT"); break;
        case ASTREE_SHORT_INT: fprintf(stderr,"ASTREE_SHORT_INT"); break;
        case ASTREE_LONG_INT: fprintf(stderr,"ASTREE_LONG_INT"); break;
        case ASTREE_FLOAT_INT: fprintf(stderr,"ASTREE_FLOAT_INT"); break;
        case ASTREE_FLOAT_REAL: fprintf(stderr,"ASTREE_FLOAT_REAL"); break;
        case ASTREE_DOUBLE_INT: fprintf(stderr,"ASTREE_DOUBLE_INT"); break;
        case ASTREE_DOUBLE_REAL: fprintf(stderr,"ASTREE_DOUBLE_REAL"); break;
        
        case ASTREE_BYTE_ARR_INT: fprintf(stderr,"ASTREE_BYTE_ARR_INT"); break;
        case ASTREE_BYTE_ARR_CHAR: fprintf(stderr,"ASTREE_BYTE_ARR_CHAR"); break;

        case ASTREE_SHORT_ARR_INT: fprintf(stderr,"ASTREE_SHORT_ARR_INT"); break;
        case ASTREE_LONG_ARR_INT: fprintf(stderr,"ASTREE_LONG_ARR_INT"); break;
        case ASTREE_FLOAT_ARR_RI: fprintf(stderr,"ASTREE_FLOAT_ARR_RI"); break;
        case ASTREE_DOUBLE_ARR_RI: fprintf(stderr,"ASTREE_DOUBLE_ARR_RI"); break;

        case ASTREE_BYTE_ARR: fprintf(stderr,"ASTREE_BYTE_ARR"); break;
        case ASTREE_SHORT_ARR: fprintf(stderr,"ASTREE_SHORT_ARR"); break;
        case ASTREE_LONG_ARR: fprintf(stderr,"ASTREE_LONG_ARR"); break;
        case ASTREE_FLOAT_ARR: fprintf(stderr,"ASTREE_FLOAT_ARR"); break;
        case ASTREE_DOUBLE_ARR: fprintf(stderr,"ASTREE_DOUBLE_ARR"); break;

        case ASTREE_INT_LST: fprintf(stderr,"ASTREE_INT_LST"); break;
        case ASTREE_CHAR_LST: fprintf(stderr,"ASTREE_CHAR_LST"); break;
        case ASTREE_FLOAT_LST: fprintf(stderr,"ASTREE_FLOAT_LST"); break;

        case ASTREE_FUNC_DEC: fprintf(stderr,"ASTREE_FUNC_DEC"); break;
        case ASTREE_PARAM_LST: fprintf(stderr,"ASTREE_PARAM_LST"); break;
        case ASTREE_PARAM: fprintf(stderr,"ASTREE_PARAM"); break;

        case ASTREE_LIT_INT: fprintf(stderr,"ASTREE_LIT_INT"); break;
        case ASTREE_LIT_REAL: fprintf(stderr,"ASTREE_LIT_REAL"); break;
        case ASTREE_LIT_CHAR: fprintf(stderr,"ASTREE_LIT_CHAR"); break;
        case ASTREE_LIT_STRING: fprintf(stderr,"ASTREE_LIT_STRING"); break;

        case ASTREE_CMD_LST: fprintf(stderr,"ASTREE_CMD_LST"); break;
        case ASTREE_CMD_BKTS: fprintf(stderr,"ASTREE_CMD_BKTS"); break;

        case ASTREE_KW_READ: fprintf(stderr,"ASTREE_KW_READ"); break;
        case ASTREE_KW_PRINT: fprintf(stderr,"ASTREE_KW_PRINT"); break;
        case ASTREE_PRINT_LST: fprintf(stderr,"ASTREE_PRINT_LST"); break;
        case ASTREE_KW_RETURN: fprintf(stderr,"ASTREE_KW_RETURN"); break;

        case ASTREE_ATTRIB: fprintf(stderr,"ASTREE_ATTRIB"); break;
        case ASTREE_ATTRIB_ARR: fprintf(stderr,"ASTREE_ATTRIB_ARR"); break;

        case ASTREE_KW_BYTE: fprintf(stderr,"ASTREE_KW_BYTE"); break;
        case ASTREE_KW_SHORT: fprintf(stderr,"ASTREE_KW_SHORT"); break;
        case ASTREE_KW_LONG: fprintf(stderr,"ASTREE_KW_LONG"); break;
        case ASTREE_KW_FLOAT: fprintf(stderr,"ASTREE_KW_FLOAT"); break;
        case ASTREE_KW_DOUBLE: fprintf(stderr,"ASTREE_KW_DOUBLE"); break;

        case ASTREE_KW_WHEN_THEN: fprintf(stderr,"ASTREE_KW_WHEN_THEN"); break;
        case ASTREE_KW_WHEN_THEN_ELSE: fprintf(stderr,"ASTREE_KW_WHEN_THEN_ELSE"); break;
        case ASTREE_KW_WHILE: fprintf(stderr,"ASTREE_KW_WHILE"); break;
        case ASTREE_KW_FOR: fprintf(stderr,"ASTREE_KW_FOR"); break;

        case ASTREE_EXP_PARENTHESIS: fprintf(stderr,"ASTREE_EXP_PARENTHESIS"); break;
        case ASTREE_TK_ID: fprintf(stderr,"ASTREE_TK_ID"); break;
        case ASTREE_ARRAY_CALL: fprintf(stderr,"ASTREE_ARRAY_CALL"); break;
        case ASTREE_FUNC_CALL: fprintf(stderr,"ASTREE_FUNC_CALL"); break;
        case ASTREE_FUNC_ARGS: fprintf(stderr,"ASTREE_FUNC_ARGS"); break;
        case ASTREE_FUNC_ARGS_EXT: fprintf(stderr,"ASTREE_FUNC_ARGS_EXT"); break;

        case ASTREE_LEQ: fprintf(stderr,"ASTREE_LEQ"); break;
        case ASTREE_GTE: fprintf(stderr,"ASTREE_GTE"); break;
        case ASTREE_EQU: fprintf(stderr,"ASTREE_EQU"); break;
        case ASTREE_NEQ: fprintf(stderr,"ASTREE_NEQ"); break;
        case ASTREE_AND: fprintf(stderr,"ASTREE_AND"); break;
        case ASTREE_OR: fprintf(stderr,"ASTREE_OR"); break;
        case ASTREE_ADD: fprintf(stderr,"ASTREE_ADD"); break;
        case ASTREE_SUB: fprintf(stderr,"ASTREE_SUB"); break;
        case ASTREE_MUL: fprintf(stderr,"ASTREE_MUL"); break;
        case ASTREE_DIV: fprintf(stderr,"ASTREE_DIV"); break;
        case ASTREE_LES: fprintf(stderr,"ASTREE_LES"); break;
        case ASTREE_GTR: fprintf(stderr,"ASTREE_GTR"); break;
        case ASTREE_NOT: fprintf(stderr,"ASTREE_NOT"); break;   
    }

    if(node->symbol)
        fprintf(stderr, ",%s)\n", node->symbol->text);
    else
        fprintf(stderr, ",)\n");

    for(i=0; i < MAX_SONS; i++)
        astree_print(node->son[i], level+1);
};

void astree_print_node(astree_t *node) {
    if(!node) return;

    fprintf(stderr, "ASTREE NODE(");
    
    switch(node->type) {
        // case ASTREE_SYMBOL:
            // fprintf(stderr, "ASTREE_SYMBOL");
            // break;
        case ASTREE_ADD:
        fprintf(stderr,"ASTREE_ADD");
        break;
    }

    if(node->symbol)
        fprintf(stderr, ",%s", node->symbol->text); // CHECK print (?)
    else
        fprintf(stderr, ",");
}


char* decompileTree(astree_t* ast) {
    if (ast) {
        switch (ast->type) {

          case ASTREE_DECL_LIST: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char *buffer = (char *) calloc(1 + strlen(son0_source) + strlen(son1_source), sizeof(char));
            sprintf(buffer, "%s%s", son0_source, son1_source);
            return buffer;
        }

        case ASTREE_VAR_DEC: {
            char* son0_source = decompileTree(ast->son[0]);
            char* buffer = (char*)calloc(strlen(ast->symbol->text) + 3 + strlen(son0_source) + 2 + 1,sizeof(char));
            sprintf(buffer,"%s : %s;\n",ast->symbol->text,son0_source);
            return buffer;
        }

        case ASTREE_BYTE_CHAR: {
            char* buffer = (char*)calloc(5 + strlen(ast->symbol->text)+1,sizeof(char));
            sprintf(buffer,"byte %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_BYTE_INT: {
            char* buffer = (char*)calloc(5 + strlen(ast->symbol->text)+1,sizeof(char));
            sprintf(buffer,"byte %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_SHORT_INT: {
            char* buffer = (char*)calloc(6 + strlen(ast->symbol->text)+1,sizeof(char));
            sprintf(buffer,"short %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_LONG_INT: {
            char* buffer = (char*)calloc(5 + strlen(ast->symbol->text)+1,sizeof(char));
            sprintf(buffer,"long %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_FLOAT_INT: {
            char* buffer = (char*)calloc(6 + strlen(ast->symbol->text)+1,sizeof(char));
            sprintf(buffer,"float %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_FLOAT_REAL: {
            char* buffer = (char*)calloc(6 + strlen(ast->symbol->text)+1,sizeof(char));
            sprintf(buffer,"float %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_DOUBLE_INT: {
            char* buffer = (char*)calloc(7 + strlen(ast->symbol->text) + 1,sizeof(char));
            sprintf(buffer,"double %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_DOUBLE_REAL: {
            char *buffer = (char *) calloc(7 + strlen(ast->symbol->text) + 1, sizeof(char));
            sprintf(buffer, "double %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_BYTE_ARR_INT: {
            char* son0_source = decompileTree(ast->son[0]);
            char* buffer = (char*)calloc(6 + strlen(ast->symbol->text) + 2 + strlen(son0_source) + 1,sizeof(char));
            sprintf(buffer,"byte [%s] %s", ast->symbol->text, son0_source);
            return buffer;
        }
        
        case ASTREE_BYTE_ARR_CHAR: {
            char* son0_source = decompileTree(ast->son[0]);
            char* buffer = (char*)calloc(6 + strlen(ast->symbol->text) + 2 + strlen(son0_source) + 1,sizeof(char));
            sprintf(buffer, "byte [%s] %s", ast->symbol->text, son0_source);
            return buffer;
        }

        case ASTREE_SHORT_ARR_INT: {
            char* son0_source = decompileTree(ast->son[0]);
            char* buffer = (char*)calloc(5 + strlen(ast->symbol->text) + 2 + strlen(son0_source) + 1,sizeof(char));
            sprintf(buffer, "int [%s] %s", ast->symbol->text, son0_source);
            return buffer;
        }
        
        case ASTREE_LONG_ARR_INT: {
            char* son0_source = decompileTree(ast->son[0]);
            char* buffer = (char*)calloc(6 + strlen(ast->symbol->text) + 2 + strlen(son0_source) + 1,sizeof(char));
            sprintf(buffer, "long [%s] %s", ast->symbol->text, son0_source);
            return buffer;
        }
        
        case ASTREE_FLOAT_ARR_RI: {
            char* son0_source = decompileTree(ast->son[0]);
            char* buffer = (char*)calloc(7 + strlen(ast->symbol->text) + 2 + strlen(son0_source) + 1,sizeof(char));
            sprintf(buffer, "float [%s] %s", ast->symbol->text, son0_source);
            return buffer;
        }
        
        case ASTREE_DOUBLE_ARR_RI: {
            char* son0_source = decompileTree(ast->son[0]);
            char* buffer = (char*)calloc(8 + strlen(ast->symbol->text) + 2 + strlen(son0_source) + 1,sizeof(char));
            sprintf(buffer, "double [%s] %s", ast->symbol->text, son0_source);
            return buffer;
        }

        case ASTREE_BYTE_ARR: {
            char *buffer = (char *) calloc(6 + strlen(ast->symbol->text) + 1, sizeof(char));
            sprintf(buffer, "byte [%s]", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_SHORT_ARR: {
            char *buffer = (char *) calloc(7 + strlen(ast->symbol->text) + 1, sizeof(char));
            sprintf(buffer, "short [%s]", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_LONG_ARR: {
            char *buffer = (char *) calloc(7 + strlen(ast->symbol->text) + 1, sizeof(char));
            sprintf(buffer, "long [%s]", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_FLOAT_ARR: {
            char *buffer = (char *) calloc(7 + strlen(ast->symbol->text) + 1, sizeof(char));
            sprintf(buffer, "float [%s]", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_DOUBLE_ARR: {
            char *buffer = (char *) calloc(8 + strlen(ast->symbol->text) + 1, sizeof(char));
            sprintf(buffer, "double [%s]", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_INT_LST: {
            return "lalala";

        }
        
        case ASTREE_CHAR_LST: {
            return "incomplete33";
        }
        
        case ASTREE_FLOAT_LST: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);

            char *buffer = (char *)calloc(strlen(son0_source) + 1 + strlen(son1_source), sizeof(char));
            sprintf(buffer,"%s %s", son0_source, son1_source);

            return buffer;
        }

        case ASTREE_FUNC_DEC: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char* son2_source = decompileTree(ast->son[2]);

            // short main(parameters) commando;

            char *buffer = (char *)calloc(strlen(son0_source) + 1 + strlen(ast->symbol->text) + 1 + strlen(son1_source) + 1 + strlen(son2_source) + 1, sizeof(char));
            sprintf(buffer,"%s %s(%s)%s;", son0_source, ast->symbol->text, son1_source, son2_source);

            return buffer;
        }
        
        case ASTREE_PARAM_LST: {
            return "incomplete66";
        }
        
        case ASTREE_PARAM: {
            return "incomplete77";
        }

        case ASTREE_LIT_INT: {
            char* son0_source = decompileTree(ast->son[0]);

            char *buffer = (char *)calloc(strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer,"%s", ast->symbol->text);

            return buffer;
        }
        
        case ASTREE_LIT_REAL: {
                        char* son0_source = decompileTree(ast->son[0]);

            char *buffer = (char *)calloc(strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer,"%s", ast->symbol->text);

            return buffer;
        }
        
        case ASTREE_LIT_CHAR: {
            
            char *buffer = (char *)calloc(strlen(ast->symbol->text) + 2, sizeof(char));
            sprintf(buffer,"'%s'", ast->symbol->text);
            
            return buffer;
        }
        
        case ASTREE_LIT_STRING: {
            
            char *buffer = (char *)calloc(strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "%s", ast->symbol->text);
            
            return buffer;
        }

        case ASTREE_CMD_LST: {

            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);

            char *buffer = (char *)calloc(strlen(son0_source) + 1 + strlen(son1_source) + 2, sizeof(char));
            sprintf(buffer,"%s%s;\n", son0_source, son1_source);

            return buffer;
        }
        
        case ASTREE_CMD_BKTS: {
            char* son0_source = decompileTree(ast->son[0]);

            char *buffer = (char *)calloc(3 + strlen(son0_source) +2, sizeof(char));
            sprintf(buffer,"{\n%s}", son0_source);

            return buffer;
        }

        case ASTREE_KW_READ: {
            return "incomplete444";
        }
        
        case ASTREE_KW_PRINT: {
            char* son0_source = decompileTree(ast->son[0]);
            // print "texto" identifier

            char *buffer = (char *)calloc(6 + strlen(son0_source) + 1, sizeof(char));
            sprintf(buffer,"print %s", son0_source);

            return buffer;
        }
        
        case ASTREE_PRINT_LST: {
            return "incomplete666";
        }
        
        case ASTREE_KW_RETURN: {
            return "return";
        }

        case ASTREE_ATTRIB: {
            return "incomplete888";
        }
        
        case ASTREE_ATTRIB_ARR: {
            return "incomplete999";
        }

        case ASTREE_KW_BYTE: {
            return "byte";
        }
        
        case ASTREE_KW_SHORT: {
            return "short";
        }
        
        case ASTREE_KW_LONG: {
            return "long";
        }
        
        case ASTREE_KW_FLOAT: {
            return "float";
        }
        
        case ASTREE_KW_DOUBLE: {
            return "double";
        }

        case ASTREE_KW_WHEN_THEN: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char *buffer = (char *)calloc(5 +strlen(son0_source) +7 +strlen(son1_source), sizeof(char));
            sprintf(buffer, "when(%s) then %s", son0_source, son1_source);
            return buffer;
        }
        
        case ASTREE_KW_WHEN_THEN_ELSE: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char* son2_source = decompileTree(ast->son[2]);
            char *buffer = (char *)calloc(5 +strlen(son0_source) +7 +strlen(son1_source) +6 +strlen(son2_source), sizeof(char));
            sprintf(buffer, "when(%s) then %s else %s", son0_source, son1_source, son2_source);
            return buffer;
        }
        
        case ASTREE_KW_WHILE: {
            return "incomplete8888";
        }
        
        case ASTREE_KW_FOR: {
            return "incomplete9999";
        }

        case ASTREE_EXP_PARENTHESIS: {
            return "incomplete11111";
        }
        
        case ASTREE_TK_ID: {
            return "incomplete22222";
        }
        
        case ASTREE_ARRAY_CALL: {
            return "incomplete33333";
        }
        
        case ASTREE_FUNC_CALL: {
            return "incomplete44444";
        }
        
        case ASTREE_FUNC_ARGS: {
            return "incomplete55555";
        }
        
        case ASTREE_FUNC_ARGS_EXT: {
            return "incomplete66666";
        }

        case ASTREE_LEQ: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char *buffer = (char *)calloc(strlen(son0_source) +4 +strlen(son1_source), sizeof(char));
            sprintf(buffer, "%s <= %s", son0_source, son1_source);
            return buffer;
        }
        
        case ASTREE_GTE: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char *buffer = (char *)calloc(strlen(son0_source) +4 +strlen(son1_source), sizeof(char));
            sprintf(buffer, "%s >= %s", son0_source, son1_source);
            return buffer;
        }
        
        case ASTREE_EQU: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char *buffer = (char *)calloc(strlen(son0_source) +4 +strlen(son1_source), sizeof(char));
            sprintf(buffer, "%s == %s", son0_source, son1_source);
            return buffer;
        }
        
        case ASTREE_NEQ: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char *buffer = (char *)calloc(strlen(son0_source) +4 +strlen(son1_source), sizeof(char));
            sprintf(buffer, "%s != %s", son0_source, son1_source);
            return buffer;
        }
        
        case ASTREE_AND: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char *buffer = (char *)calloc(strlen(son0_source) +4 +strlen(son1_source), sizeof(char));
            sprintf(buffer, "%s && %s", son0_source, son1_source);
            return buffer;
        }
        
        case ASTREE_OR: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char *buffer = (char *)calloc(strlen(son0_source) +4 +strlen(son1_source), sizeof(char));
            sprintf(buffer, "%s || %s", son0_source, son1_source);
            return buffer;
        }
        
        case ASTREE_ADD: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char* buffer = (char*)calloc( strlen(son0_source) + 3 + strlen(son1_source) + 1,sizeof(char));
            sprintf(buffer,"%s + %s", son0_source, son1_source);
            return buffer;
        }
        
        case ASTREE_SUB: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char* buffer = (char*)calloc( strlen(son0_source) + 3 + strlen(son1_source) + 1,sizeof(char));
            sprintf(buffer,"%s - %s", son0_source, son1_source);
            return buffer;
        }
        
        case ASTREE_MUL: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char* buffer = (char*)calloc( strlen(son0_source) + 3 + strlen(son1_source) + 1,sizeof(char));
            sprintf(buffer,"%s * %s", son0_source, son1_source);
            return buffer;
        }
        
        case ASTREE_DIV: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char* buffer = (char*)calloc( strlen(son0_source) + 3 + strlen(son1_source) + 1,sizeof(char));
            sprintf(buffer,"%s / %s", son0_source, son1_source);
            return buffer;
        }
        
        case ASTREE_LES: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char* buffer = (char*)calloc( strlen(son0_source) + 3 + strlen(son1_source) + 1,sizeof(char));
            sprintf(buffer,"%s < %s", son0_source, son1_source);
            return buffer;
        }
        
        case ASTREE_GTR: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char* buffer = (char*)calloc( strlen(son0_source) + 3 + strlen(son1_source) + 1,sizeof(char));
            sprintf(buffer,"%s > %s", son0_source, son1_source);
            return buffer;
        }
        
        case ASTREE_NOT: {
            return "incomplete1111111";
        }
        }
        return "";
    } else {
        return "";
    }
}