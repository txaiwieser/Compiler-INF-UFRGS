#include <stdio.h>
#include "astree.h"

astree_t* astree_create(int type, hash_node_t *symbol, astree_t *son_1, astree_t *son_2, astree_t *son_3, astree_t *son_4) {
    
    astree_t* newnode = 0;
    newnode = calloc(1, sizeof(astree_t));
    
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
    switch(node->type)
    {
        case ASTREE_PROG_START: fprintf(stderr,"ASTREE_PROG_START"); break;

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
    {
        astree_print(node->son[i], level+1);
    }
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
        return "";
    } else {
        return "Oi ";
    }
}