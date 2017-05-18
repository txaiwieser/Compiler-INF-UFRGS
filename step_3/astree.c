#include <stdio.h>
#include "astree.h"
#include "strings.h"

astree_t* astree_create(int type, hash_node_t *symbol, astree_t *child_1, astree_t *child_2, astree_t *child_3, astree_t *child_4) {

    astree_t* newnode = 0;
    newnode = (astree_t *) calloc(1, sizeof(astree_t));
    
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->children[0] = child_1;
    newnode->children[1] = child_2;
    newnode->children[2] = child_3;
    newnode->children[3] = child_4;

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
        case ASTREE_DECL_LIST: fprintf(stderr, "ASTREE_DECL_LIST"); break;

        case ASTREE_VAR_DEC: fprintf(stderr, "ASTREE_VAR_DEC"); break;
        case ASTREE_BYTE_CHAR: fprintf(stderr, "ASTREE_BYTE_CHAR"); break;
        case ASTREE_BYTE_INT: fprintf(stderr, "ASTREE_BYTE_INT"); break;
        case ASTREE_SHORT_INT: fprintf(stderr, "ASTREE_SHORT_INT"); break;
        case ASTREE_LONG_INT: fprintf(stderr, "ASTREE_LONG_INT"); break;
        case ASTREE_FLOAT_INT: fprintf(stderr, "ASTREE_FLOAT_INT"); break;
        case ASTREE_FLOAT_REAL: fprintf(stderr, "ASTREE_FLOAT_REAL"); break;
        case ASTREE_DOUBLE_INT: fprintf(stderr, "ASTREE_DOUBLE_INT"); break;
        case ASTREE_DOUBLE_REAL: fprintf(stderr, "ASTREE_DOUBLE_REAL"); break;
        
        case ASTREE_BYTE_ARR_INT: fprintf(stderr, "ASTREE_BYTE_ARR_INT"); break;
        case ASTREE_BYTE_ARR_CHAR: fprintf(stderr, "ASTREE_BYTE_ARR_CHAR"); break;

        case ASTREE_SHORT_ARR_INT: fprintf(stderr, "ASTREE_SHORT_ARR_INT"); break;
        case ASTREE_LONG_ARR_INT: fprintf(stderr, "ASTREE_LONG_ARR_INT"); break;
        case ASTREE_FLOAT_ARR_RI: fprintf(stderr, "ASTREE_FLOAT_ARR_RI"); break;
        case ASTREE_DOUBLE_ARR_RI: fprintf(stderr, "ASTREE_DOUBLE_ARR_RI"); break;

        case ASTREE_BYTE_ARR: fprintf(stderr, "ASTREE_BYTE_ARR"); break;
        case ASTREE_SHORT_ARR: fprintf(stderr, "ASTREE_SHORT_ARR"); break;
        case ASTREE_LONG_ARR: fprintf(stderr, "ASTREE_LONG_ARR"); break;
        case ASTREE_FLOAT_ARR: fprintf(stderr, "ASTREE_FLOAT_ARR"); break;
        case ASTREE_DOUBLE_ARR: fprintf(stderr, "ASTREE_DOUBLE_ARR"); break;

        case ASTREE_INT_LST: fprintf(stderr, "ASTREE_INT_LST"); break;
        case ASTREE_CHAR_LST: fprintf(stderr, "ASTREE_CHAR_LST"); break;
        case ASTREE_FLOAT_LST: fprintf(stderr, "ASTREE_FLOAT_LST"); break;

        case ASTREE_FUNC_DEC: fprintf(stderr, "ASTREE_FUNC_DEC"); break;
        case ASTREE_PARAM_LST: fprintf(stderr, "ASTREE_PARAM_LST"); break;
        case ASTREE_PARAM: fprintf(stderr, "ASTREE_PARAM"); break;

        case ASTREE_LIT_INT: fprintf(stderr, "ASTREE_LIT_INT"); break;
        case ASTREE_LIT_REAL: fprintf(stderr, "ASTREE_LIT_REAL"); break;
        case ASTREE_LIT_CHAR: fprintf(stderr, "ASTREE_LIT_CHAR"); break;
        case ASTREE_LIT_STRING: fprintf(stderr, "ASTREE_LIT_STRING"); break;

        case ASTREE_CMD_LST: fprintf(stderr, "ASTREE_CMD_LST"); break;
        case ASTREE_CMD_BKTS: fprintf(stderr, "ASTREE_CMD_BKTS"); break;

        case ASTREE_KW_READ: fprintf(stderr, "ASTREE_KW_READ"); break;
        case ASTREE_KW_PRINT: fprintf(stderr, "ASTREE_KW_PRINT"); break;
        case ASTREE_PRINT_LST: fprintf(stderr, "ASTREE_PRINT_LST"); break;
        case ASTREE_KW_RETURN: fprintf(stderr, "ASTREE_KW_RETURN"); break;

        case ASTREE_ATTRIB: fprintf(stderr, "ASTREE_ATTRIB"); break;
        case ASTREE_ATTRIB_ARR: fprintf(stderr, "ASTREE_ATTRIB_ARR"); break;

        case ASTREE_KW_BYTE: fprintf(stderr, "ASTREE_KW_BYTE"); break;
        case ASTREE_KW_SHORT: fprintf(stderr, "ASTREE_KW_SHORT"); break;
        case ASTREE_KW_LONG: fprintf(stderr, "ASTREE_KW_LONG"); break;
        case ASTREE_KW_FLOAT: fprintf(stderr, "ASTREE_KW_FLOAT"); break;
        case ASTREE_KW_DOUBLE: fprintf(stderr, "ASTREE_KW_DOUBLE"); break;

        case ASTREE_KW_WHEN_THEN: fprintf(stderr, "ASTREE_KW_WHEN_THEN"); break;
        case ASTREE_KW_WHEN_THEN_ELSE: fprintf(stderr, "ASTREE_KW_WHEN_THEN_ELSE"); break;
        case ASTREE_KW_WHILE: fprintf(stderr, "ASTREE_KW_WHILE"); break;
        case ASTREE_KW_FOR: fprintf(stderr, "ASTREE_KW_FOR"); break;

        case ASTREE_EXP_PARENTHESIS: fprintf(stderr, "ASTREE_EXP_PARENTHESIS"); break;
        case ASTREE_TK_ID: fprintf(stderr, "ASTREE_TK_ID"); break;
        case ASTREE_ARRAY_CALL: fprintf(stderr, "ASTREE_ARRAY_CALL"); break;
        case ASTREE_FUNC_CALL: fprintf(stderr, "ASTREE_FUNC_CALL"); break;
        case ASTREE_FUNC_ARGS: fprintf(stderr, "ASTREE_FUNC_ARGS"); break;
        case ASTREE_FUNC_ARGS_EXT: fprintf(stderr, "ASTREE_FUNC_ARGS_EXT"); break;

        case ASTREE_LEQ: fprintf(stderr, "ASTREE_LEQ"); break;
        case ASTREE_GTE: fprintf(stderr, "ASTREE_GTE"); break;
        case ASTREE_EQU: fprintf(stderr, "ASTREE_EQU"); break;
        case ASTREE_NEQ: fprintf(stderr, "ASTREE_NEQ"); break;
        case ASTREE_AND: fprintf(stderr, "ASTREE_AND"); break;
        case ASTREE_OR: fprintf(stderr, "ASTREE_OR"); break;
        case ASTREE_ADD: fprintf(stderr, "ASTREE_ADD"); break;
        case ASTREE_SUB: fprintf(stderr, "ASTREE_SUB"); break;
        case ASTREE_MUL: fprintf(stderr, "ASTREE_MUL"); break;
        case ASTREE_DIV: fprintf(stderr, "ASTREE_DIV"); break;
        case ASTREE_LES: fprintf(stderr, "ASTREE_LES"); break;
        case ASTREE_GTR: fprintf(stderr, "ASTREE_GTR"); break; 
    }

    if(node->symbol)
        fprintf(stderr, ",%s)\n", node->symbol->text);
    else
        fprintf(stderr, ",)\n");

    for(i=0; i < MAX_NUMBER_OF_CHILDREN; i++)
        astree_print(node->children[i], level+1);
};

char* decompileTree(astree_t* ast) {
    if (ast) {
        switch (ast->type) {

          case ASTREE_DECL_LIST: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char *buffer = (char *) calloc(1 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s%s", child_0_text, child_1_text);
            return buffer;
        }

        case ASTREE_VAR_DEC: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* buffer = (char *) calloc(6 + strlen(ast->symbol->text) + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "%s : %s;\n",ast->symbol->text,child_0_text);
            return buffer;
        }

        case ASTREE_BYTE_CHAR: {
            char* buffer = (char *) calloc(7 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "byte %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_BYTE_INT: {
            char* buffer = (char *) calloc(6 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "byte %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_SHORT_INT: {
            char* buffer = (char *) calloc(7 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "short %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_LONG_INT: {
            char* buffer = (char *) calloc(6 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "long %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_FLOAT_INT: {
            char* buffer = (char *) calloc(7 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "float %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_FLOAT_REAL: {
            char* buffer = (char *) calloc(7 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "float %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_DOUBLE_INT: {
            char* buffer = (char *) calloc(8 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "double %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_DOUBLE_REAL: {
            char *buffer = (char *) calloc(8 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "double %s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_BYTE_ARR_INT: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* buffer = (char *) calloc(9 + strlen(ast->symbol->text) + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "byte [%s] %s", ast->symbol->text, child_0_text);
            return buffer;
        }
        
        case ASTREE_BYTE_ARR_CHAR: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* buffer = (char *) calloc(9 + strlen(ast->symbol->text) + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "byte [%s] %s", ast->symbol->text, child_0_text);
            return buffer;
        }

        case ASTREE_SHORT_ARR_INT: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* buffer = (char *) calloc(8 + strlen(ast->symbol->text) + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "int [%s] %s", ast->symbol->text, child_0_text);
            return buffer;
        }
        
        case ASTREE_LONG_ARR_INT: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* buffer = (char *) calloc(9 + strlen(ast->symbol->text) + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "long [%s] %s", ast->symbol->text, child_0_text);
            return buffer;
        }
        
        case ASTREE_FLOAT_ARR_RI: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* buffer = (char *) calloc(10 + strlen(ast->symbol->text) + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "float [%s] %s", ast->symbol->text, child_0_text);
            return buffer;
        }
        
        case ASTREE_DOUBLE_ARR_RI: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* buffer = (char *) calloc(11 + strlen(ast->symbol->text) + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "double [%s] %s", ast->symbol->text, child_0_text);
            return buffer;
        }

        case ASTREE_BYTE_ARR: {
            char *buffer = (char *) calloc(7 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "byte [%s]", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_SHORT_ARR: {
            char *buffer = (char *) calloc(8 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "short [%s]", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_LONG_ARR: {
            char *buffer = (char *) calloc(8 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "long [%s]", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_FLOAT_ARR: {
            char *buffer = (char *) calloc(8 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "float [%s]", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_DOUBLE_ARR: {
            char *buffer = (char *) calloc(9 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "double [%s]", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_INT_LST: {
            char* child_0_text = decompileTree(ast->children[0]);

            char *buffer = (char *) calloc(3 + strlen(ast->symbol->text) + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "%s %s", ast->symbol->text, child_0_text);

            return buffer;
        }
        
        case ASTREE_CHAR_LST: {
            char* child_0_text = decompileTree(ast->children[0]);

            char *buffer = (char *) calloc(3 + strlen(ast->symbol->text) + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "%s %s", ast->symbol->text, child_0_text);

            return buffer;
        }
        
        case ASTREE_FLOAT_LST: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);

            char *buffer = (char *) calloc(1 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s %s", child_0_text, child_1_text);

            return buffer;
        }

        case ASTREE_FUNC_DEC: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char* child_2_text = decompileTree(ast->children[2]);

            // short main(parameters) commando;

            char *buffer = (char *) calloc(5 + strlen(child_0_text) + strlen(ast->symbol->text) + strlen(child_1_text) + strlen(child_2_text), sizeof(char));
            sprintf(buffer, "%s %s(%s)%s;\n", child_0_text, ast->symbol->text, child_1_text, child_2_text);

            return buffer;
        }
        
        case ASTREE_PARAM_LST: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char* buffer = (char *) calloc(2 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s,%s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_PARAM: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* buffer = (char *) calloc(2 + strlen(child_0_text) + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "%s %s", child_0_text, ast->symbol->text);
            return buffer;
        }

        case ASTREE_LIT_INT: {
            char* child_0_text = decompileTree(ast->children[0]);
            char *buffer = (char *) calloc(strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "%s", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_LIT_REAL: {
            char* child_0_text = decompileTree(ast->children[0]);
            char *buffer = (char *) calloc(strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "%s", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_LIT_CHAR: {
            char *buffer = (char *) calloc(2 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "%s", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_LIT_STRING: {
            char *buffer = (char *) calloc(strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "%s", ast->symbol->text);
            return buffer;
        }

        case ASTREE_CMD_LST: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char *buffer = (char *) calloc(3 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s%s;\n", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_CMD_BKTS: {
            char* child_0_text = decompileTree(ast->children[0]);
            char *buffer = (char *) calloc(5 + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "{\n%s}", child_0_text);

            return buffer;
        }

        case ASTREE_KW_READ: {
            char *buffer = (char *) calloc(5 + strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "read %s", ast->symbol->text);
            return buffer;        
        }
        
        case ASTREE_KW_PRINT: {
            char* child_0_text = decompileTree(ast->children[0]); // print "texto" identifier
            char *buffer = (char *) calloc(7 + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "print %s", child_0_text);

            return buffer;
        }
        
        case ASTREE_PRINT_LST: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char *buffer = (char *) calloc(1 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_KW_RETURN: {
            char* child_0_text = decompileTree(ast->children[0]);
            char *buffer = (char *) calloc(7 + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "return %s", child_0_text);
            return buffer;   
        }

        case ASTREE_ATTRIB: {
            char* child_0_text = decompileTree(ast->children[0]);
            char *buffer = (char *) calloc(3 + strlen(ast->symbol->text) + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "%s = %s", ast->symbol->text, child_0_text);
            return buffer;        
        }
        
        case ASTREE_ATTRIB_ARR: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char *buffer = (char *) calloc(6 + strlen(ast->symbol->text) + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s # %s = %s", ast->symbol->text, child_0_text, child_1_text);

            return buffer;
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
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char *buffer = (char *) calloc(12 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "when(%s) then %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_KW_WHEN_THEN_ELSE: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char* child_2_text = decompileTree(ast->children[2]);
            char *buffer = (char *) calloc(18 + strlen(child_0_text) + strlen(child_1_text) + strlen(child_2_text), sizeof(char));
            sprintf(buffer, "when(%s) then %s else %s", child_0_text, child_1_text, child_2_text);
            return buffer;
        }
        
        case ASTREE_KW_WHILE: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char* buffer = (char *) calloc(9 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "while(%s)\n%s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_KW_FOR: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char* child_2_text = decompileTree(ast->children[2]);
            char* buffer = (char *) calloc(14 + strlen(ast->symbol->text) + strlen(child_0_text) + strlen(child_1_text) + strlen(child_2_text), sizeof(char));
            sprintf(buffer, "for(%s = %s to %s)\n%s", ast->symbol->text, child_0_text, child_1_text, child_2_text);
            return buffer;
        }

        case ASTREE_EXP_PARENTHESIS: {
            char* child_0_text = decompileTree(ast->children[0]);
            char *buffer = (char *) calloc(2 + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "(%s)", child_0_text);
            return buffer;
        }
        
        case ASTREE_TK_ID: {
            char *buffer = (char *) calloc(strlen(ast->symbol->text), sizeof(char));
            sprintf(buffer, "%s", ast->symbol->text);
            return buffer;
        }
        
        case ASTREE_ARRAY_CALL: {
            char* child_0_text = decompileTree(ast->children[0]);
            char *buffer = (char *) calloc(2 + strlen(ast->symbol->text) + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "%s[%s]", ast->symbol->text, child_0_text);
            return buffer;
        }
        
        case ASTREE_FUNC_CALL: {
            char* child_0_text = decompileTree(ast->children[0]);

            char *buffer = (char *) calloc(2 + strlen(ast->symbol->text) + strlen(child_0_text), sizeof(char));
            sprintf(buffer, "%s(%s)", ast->symbol->text, child_0_text);
            return buffer;
        }
        
        case ASTREE_FUNC_ARGS: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char *buffer = (char *) calloc(1 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_FUNC_ARGS_EXT: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char *buffer = (char *) calloc(3 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, ", %s%s", child_0_text, child_1_text);
            return buffer;
        }

        case ASTREE_LEQ: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char *buffer = (char *) calloc(4 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s <= %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_GTE: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char *buffer = (char *) calloc(4 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s >= %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_EQU: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char *buffer = (char *) calloc(4 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s == %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_NEQ: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char *buffer = (char *) calloc(4 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s != %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_AND: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char *buffer = (char *) calloc(4 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s && %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_OR: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char *buffer = (char *) calloc(4 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s || %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_ADD: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char* buffer = (char *) calloc(4 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s + %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_SUB: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char* buffer = (char *) calloc(4 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s - %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_MUL: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char* buffer = (char *) calloc(4 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s * %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_DIV: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char* buffer = (char *) calloc(4 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s / %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_LES: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char* buffer = (char *) calloc(4 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s < %s", child_0_text, child_1_text);
            return buffer;
        }
        
        case ASTREE_GTR: {
            char* child_0_text = decompileTree(ast->children[0]);
            char* child_1_text = decompileTree(ast->children[1]);
            char* buffer = (char *) calloc(4 + strlen(child_0_text) + strlen(child_1_text), sizeof(char));
            sprintf(buffer, "%s > %s", child_0_text, child_1_text);
            return buffer;
        }
        }
        return "";
    } else {
        return "";
    }
}