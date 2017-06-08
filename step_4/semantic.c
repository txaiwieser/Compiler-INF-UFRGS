#include "semantic.h"
#include "symcodes.h"

void semanticVariableDeclaration(astree_t* node) {
  if (node->symbol) {
    if (node->symbol->type == SYMBOL_IDENTIFIER && node->children[0]) {
      if(node->symbol->isVariableOrFuncionDeclared) {
        fprintf(stderr, "ERRO SEMANTICO\nvariavel ja declarada: %s\n", node->symbol->text);
        exit(4);
      }

      node->symbol->isVariableOrFuncionDeclared = 1;
      switch (node->children[0]->type) {
        case ASTREE_CHAR:
          node->symbol->expressionType = EXPRESSION_CHAR;
          node->symbol->nature = NATURE_VARIABLE;
          break;
        case ASTREE_INT:
          node->symbol->expressionType = EXPRESSION_INTEGER;
          node->symbol->nature = NATURE_VARIABLE;
          break;
        case ASTREE_REAL:
          node->symbol->expressionType = EXPRESSION_REAL;
          node->symbol->nature = NATURE_VARIABLE;
          break;
        case ASTREE_ARR_INT:
          node->symbol->expressionType = EXPRESSION_INTEGER;
          node->symbol->nature = NATURE_ARRAY;
          break;
        case ASTREE_ARR_CHAR:
          node->symbol->expressionType = EXPRESSION_CHAR;
          node->symbol->nature = NATURE_ARRAY;
          break;
        case ASTREE_ARR_FLOAT:
          node->symbol->expressionType = EXPRESSION_REAL;
          node->symbol->nature = NATURE_ARRAY;
          break;
        case ASTREE_ARR:
          node->symbol->expressionType = EXPRESSION_INTEGER;
          node->symbol->nature = NATURE_ARRAY;
          break;
      }

      switch (node->children[0]->children[0]->type){
        case ASTREE_KW_BYTE:
          node->symbol->dataType = DATATYPE_BYTE;
          break;
        case ASTREE_KW_SHORT:
          node->symbol->dataType = DATATYPE_SHORT;
          break;
        case ASTREE_KW_LONG:
          node->symbol->dataType = DATATYPE_LONG;
          break;
        case ASTREE_KW_FLOAT:
          node->symbol->dataType = DATATYPE_FLOAT;
          break;
        case ASTREE_KW_DOUBLE:
          node->symbol->dataType = DATATYPE_DOUBLE;
          break;
        default:
          fprintf(stderr, "ERROR 2895\n");
          break;
      }
    }
  } else{
    fprintf(stderr, "ERROR 230 \n");
  }
}

void semanticSetDeclarations(astree_t* node) {
  int i = 0;
  
  if (node == NULL) { return; }

  for (i=0; i<MAX_NUMBER_OF_CHILDREN; i++) {
    semanticSetDeclarations(node->children[i]);
  }

  switch (node->type) {
    case ASTREE_VAR_DEC:
      semanticVariableDeclaration(node);
      break;
    case ASTREE_FUNC_DEC:
      // semanticFunctionDeclaration(node);
      break;
    case ASTREE_PARAM:
      // semanticParameter(node);
      break;
    default:
      fprintf(stderr, "ERROR 345\n");
      break;
  }
}

void semanticCheck(astree_t* node) {}
