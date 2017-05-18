#include <stdio.h>
#include "astree.h"
#include "strings.h"

astree_t* astree_create(int type, hash_node_t *symbol, astree_t *son_1, astree_t *son_2, astree_t *son_3, astree_t *son_4) {
    
    astree_t* newnode = calloc(1, sizeof(astree_t));
    
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
    if (ast == NULL) {
        return "incomplete";
    } else {
        switch (ast->type) {

          case ASTREE_DECL_LIST: {
            char* son0_source = decompileTree(ast->son[0]);
            char* son1_source = decompileTree(ast->son[1]);
            char *buffer = (char *) calloc(strlen(son0_source) + strlen(son1_source), sizeof(char));
            sprintf(buffer, "%s%s", son0_source, son1_source);
            return buffer;
        }

        case ASTREE_VAR_DEC: {
            char* son0_source = decompileTree(ast->son[0]);
            char *buffer = (char *) calloc(30 + 1 + strlen(son0_source) + 1, sizeof(char));
            sprintf(buffer, "%s:%s;", ast->symbol->text, son0_source);
            return buffer;
        }

        case ASTREE_BYTE_CHAR: {
            char *buffer = (char *) calloc(4 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer,"BYTE%s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_BYTE_INT: {
            char *buffer = (char *) calloc(4 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "BYTE%s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_SHORT_INT: {
            char *buffer = (char *) calloc(5 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "SHORT%s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_LONG_INT: {
            char *buffer = (char *) calloc(4 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "LONG%s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_FLOAT_INT: {
            char *buffer = (char *) calloc(5 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "FLOAT%s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_FLOAT_REAL: {
            char *buffer = (char *) calloc(5 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "FLOAT%s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_DOUBLE_INT: {
            char *buffer = (char *) calloc(6 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "DOUBLE%s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_DOUBLE_REAL: {
            char *buffer = (char *) calloc(6 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "DOUBLE%s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_BYTE_ARR_INT: {
            char* son0_source = decompileTree(ast->son[0]);
            char *buffer = (char *)calloc(5 + strlen(ast->symbol->text) + 1 + strlen(son0_source) ,sizeof(char));
            sprintf(buffer,"BYTE[%s]%s", ast->symbol->text, son0_source);
            return buffer;
        }
        
        case ASTREE_BYTE_ARR_CHAR: {
            return "incomplete";
        }

        case ASTREE_SHORT_ARR_INT: {
            return "incomplete";
        }
        
        case ASTREE_LONG_ARR_INT: {
            return "incomplete";
        }
        
        case ASTREE_FLOAT_ARR_RI: {
            return "incomplete";
        }
        
        case ASTREE_DOUBLE_ARR_RI: {
            return "incomplete";
        }

        case ASTREE_BYTE_ARR: {
            return "incomplete";
        }
        
        case ASTREE_SHORT_ARR: {
            return "incomplete";
        }
        
        case ASTREE_LONG_ARR: {
            return "incomplete";
        }
        
        case ASTREE_FLOAT_ARR: {
            return "incomplete";
        }
        
        case ASTREE_DOUBLE_ARR: {
            return "incomplete";
        }
        
        case ASTREE_INT_LST: {
            return "incomplete";
        }
        
        case ASTREE_CHAR_LST: {
            return "incomplete";
        }
        
        case ASTREE_FLOAT_LST: {
            return "incomplete";
        }

        case ASTREE_FUNC_DEC: {
            return "incomplete";
        }
        
        case ASTREE_PARAM_LST: {
            return "incomplete";
        }
        
        case ASTREE_PARAM: {
            return "incomplete";
        }

        case ASTREE_LIT_INT: {
            return "incomplete";
        }
        
        case ASTREE_LIT_REAL: {
            return "incomplete";
        }
        
        case ASTREE_LIT_CHAR: {
            return "incomplete";
        }
        
        case ASTREE_LIT_STRING: {
            return "incomplete";
        }

        case ASTREE_CMD_LST: {
            return "incomplete";
        }
        
        case ASTREE_CMD_BKTS: {
            return "incomplete";
        }

        case ASTREE_KW_READ: {
            return "incomplete";
        }
        
        case ASTREE_KW_PRINT: {
            return "incomplete";
        }
        
        case ASTREE_PRINT_LST: {
            return "incomplete";
        }
        
        case ASTREE_KW_RETURN: {
            return "incomplete";
        }

        case ASTREE_ATTRIB: {
            return "incomplete";
        }
        
        case ASTREE_ATTRIB_ARR: {
            return "incomplete";
        }

        case ASTREE_KW_BYTE: {
            return "incomplete";
        }
        
        case ASTREE_KW_SHORT: {
            return "incomplete";
        }
        
        case ASTREE_KW_LONG: {
            return "incomplete";
        }
        
        case ASTREE_KW_FLOAT: {
            return "incomplete";
        }
        
        case ASTREE_KW_DOUBLE: {
            return "incomplete";
        }

        case ASTREE_KW_WHEN_THEN: {
            return "incomplete";
        }
        
        case ASTREE_KW_WHEN_THEN_ELSE: {
            return "incomplete";
        }
        
        case ASTREE_KW_WHILE: {
            return "incomplete";
        }
        
        case ASTREE_KW_FOR: {
            return "incomplete";
        }

        case ASTREE_EXP_PARENTHESIS: {
            return "incomplete";
        }
        
        case ASTREE_TK_ID: {
            return "incomplete";
        }
        
        case ASTREE_ARRAY_CALL: {
            return "incomplete";
        }
        
        case ASTREE_FUNC_CALL: {
            return "incomplete";
        }
        
        case ASTREE_FUNC_ARGS: {
            return "incomplete";
        }
        
        case ASTREE_FUNC_ARGS_EXT: {
            return "incomplete";
        }

        case ASTREE_LEQ: {
            return "incomplete";
        }
        
        case ASTREE_GTE: {
            return "incomplete";
        }
        
        case ASTREE_EQU: {
            return "incomplete";
        }
        
        case ASTREE_NEQ: {
            return "incomplete";
        }
        
        case ASTREE_AND: {
            return "incomplete";
        }
        
        case ASTREE_OR: {
            return "incomplete";
        }
        
        case ASTREE_ADD: {
            return "incomplete";
        }
        
        case ASTREE_SUB: {
            return "incomplete";
        }
        
        case ASTREE_MUL: {
            return "incomplete";
        }
        
        case ASTREE_DIV: {
            return "incomplete";
        }
        
        case ASTREE_LES: {
            return "incomplete";
        }
        
        case ASTREE_GTR: {
            return "incomplete";
        }
        
        case ASTREE_NOT: {
            return "incomplete";
        }
        }
    }
}