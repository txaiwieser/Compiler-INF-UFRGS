#include "semantic.h"
#include "symcodes.h"

int semantic_get_expression_t(astree_t* node) {
  switch(node->type) {
    case ASTREE_LIT_INT:
      return EXPRESSION_INTEGER;
      break;
    case ASTREE_LIT_CHAR:
      return EXPRESSION_CHAR;
      break;
    case ASTREE_LIT_REAL:
      return EXPRESSION_REAL;
      break;
    case ASTREE_LIT_STRING:
      return EXPRESSION_STRING;
      break;
    case ASTREE_TK_ID:
    case ASTREE_ARRAY_CALL:
    case ASTREE_FUNC_CALL:
      return node->symbol->expressionType;
      break;
    case ASTREE_LEQ:
    case ASTREE_GTE:
    case ASTREE_EQU:
    case ASTREE_NEQ:
    case ASTREE_AND:
    case ASTREE_LES:
    case ASTREE_GTR:
    case ASTREE_OR:
      return EXPRESSION_BOOLEAN;
      break;
    case ASTREE_ADD:
    case ASTREE_SUB:
    case ASTREE_MUL:
    case ASTREE_DIV:
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_REAL || semantic_get_expression_t(node->children[1]) == EXPRESSION_REAL) {
        return EXPRESSION_REAL;
      } else {
        return EXPRESSION_INTEGER;
      }
      break;
    case ASTREE_EXP_PARENTHESIS:
      return semantic_get_expression_t(node->children[0]);
      break;
  }
  fprintf(stderr, "Semantic error at semantic_get_expression_t(): node without type.\n");
  return -1;
}

void semantic_check_arguments_t(astree_t* node) {
  if(!node) return;
  if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN) {
    fprintf(stderr, "Semantic error: argument can't be boolean type.\n");
    exit(4);  
  } else if(semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING) {
    fprintf(stderr, "Semantic error: argument can't be string type.\n");
    exit(4);
  } else if(node->type == ASTREE_FUNC_ARGS) {
    semantic_check_arguments_t(node->children[1]);
    return;
  } else if(node->type == ASTREE_FUNC_ARGS_EXT) {
    semantic_check_arguments_t(node->children[1]);
    return;
  }
  fprintf(stderr, "Semantic error at semantic_check_arguments_t().\n");
  return;
}

int semantic_get_num_arguments(astree_t* node) {
    if(!node) return 0;
    if(node->type == ASTREE_FUNC_ARGS) {
      return 1 + semantic_get_num_arguments(node->children[1]);
    } else if(node->type == ASTREE_FUNC_ARGS_EXT) {
      return 1 + semantic_get_num_arguments(node->children[1]);
    } else {
      fprintf(stderr, "Semantic error: number of arguments can't be resolved.\n");
      return 0;
  }
}

int semantic_get_num_parameters(astree_t* node) {
  if(node->type==ASTREE_PARAM) {
    return 1;
  } else if(node->children[1]->type==ASTREE_PARAM) {
    return 2;
  } else if(node->children[1]->type==ASTREE_PARAM_LST) {
    return 1 + semantic_get_num_parameters(node->children[1]);
  } else {
    fprintf(stderr, "Semantic error: number of parameters can't be resolved.\n" );
    return -1;
  }
}

void semantic_variable_declaration(astree_t* node) {
  if(node->symbol) {
    if(node->symbol->type == SYMBOL_IDENTIFIER && node->children[0]) {
      if(node->symbol->isVariableOrFuncionDeclared) {
        fprintf(stderr, "Semantic error: identifier %s used to the valriable already declared.\n", node->symbol->text);
        exit(4);
      } 

      node->symbol->isVariableOrFuncionDeclared = 1;
      
      // Bad code added to identify with imediate values must be transformed into a variable:
      node->children[0]->symbol->isVariableOrFuncionDeclared = 1;
      switch(node->children[0]->type) {
        case ASTREE_ARR_INT:
        case ASTREE_ARR_CHAR:
        case ASTREE_ARR_FLOAT:
          for(astree_t *aux = node->children[0]->children[1]; aux; aux = aux->children[0])
            aux->symbol->isVariableOrFuncionDeclared = 1;
      }
      // Outra estrategia: criar TAC_IMEDIATE pra declarar. Varre TAC_IMEDIATE's e TAC_VARDEC's pra criar a seção de dados.

      switch(node->children[0]->type) {
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

      switch(node->children[0]->children[0]->type){
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
          fprintf(stderr, "Semantic error: semantic_set_declarations() can't resolve node dataType.\n");
          break;
      }
    }
  } else {
    fprintf(stderr, "Semantic error at semantic_variable_declaration(): ASTREE node hasn't a symbol.\n");
  }
}

void semantic_check_return_t(astree_t* node) {
    int i;
    if(!node) return;
    if(node->type == ASTREE_KW_RETURN) {
	      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING) {     //Dúvida: os DataTypes são intercambiáveis? Exemplo: long a (byte x, long y) return x+y
          fprintf(stderr, "Semantic error: retorno invalido \n");
          exit(4);
        }
    }
    for (i=0; i < MAX_NUMBER_OF_CHILDREN; i++) {
        semantic_check_return_t(node->children[i]);
    }
}

void semantic_function_declaration(astree_t* node) {
  if(node->symbol) {
    if(node->symbol->isVariableOrFuncionDeclared) {
      fprintf(stderr, "Semantic error: identifier %s used to the function already declared.\n", node->symbol->text);
      exit(4);
    }

    node->symbol->isVariableOrFuncionDeclared = 1;

    if(node->symbol->type == SYMBOL_IDENTIFIER && node->children[0]){
      node->symbol->nature = NATURE_FUNCTION;
    }

    switch(node->children[0]->type){
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
        fprintf(stderr, "Semantic error: semantic_function_declaration() can't resolve node dataType.\n");
        break;
    }

    if(node->children[1] == 0) {
      node->symbol->parametersNumber = 0;
    } else{
      node->symbol->parametersNumber = semantic_get_num_parameters(node->children[1]);
    }
    // printf(stderr, "Debug info: function delcaration setted: %s, number of parameters: %d\n", node->symbol->text, node->symbol->parametersNumber);
  } else{
    fprintf(stderr, "Semantic error at semantic_function_declaration(): ASTREE node hasn't a symbol.\n");
  }
}

void semantic_parameters(astree_t* node) {
  if(node->symbol) {
    if(node->symbol->isVariableOrFuncionDeclared) {
      fprintf(stderr, "Semantic error: identifier %s used to the prameter already declared.\n", node->symbol->text);
      exit(4);
    }

    node->symbol->isVariableOrFuncionDeclared = 1;

    if(node->symbol->type == SYMBOL_IDENTIFIER && node->children[0]) {
      node->symbol->nature = NATURE_VARIABLE;
    }

    switch(node->children[0]->type) {
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
        fprintf(stderr, "Semantic error: semantic_parameters() can't resolve node dataType.\n");
        break;
    }
  } else {
    fprintf(stderr, "Semantic error at semantic_parameters(): ASTREE node hasn't a symbol.\n");
  }
}

void semantic_set_declarations(astree_t* node) {
  int i = 0;
  
  if(node == NULL) { return; }

  for (i=0; i<MAX_NUMBER_OF_CHILDREN; i++) {
    semantic_set_declarations(node->children[i]);
  }

  switch(node->type) {
    case ASTREE_VAR_DEC:
      semantic_variable_declaration(node);
      break;
    case ASTREE_FUNC_DEC:
      semantic_function_declaration(node);
      break;
    case ASTREE_PARAM:
      semantic_parameters(node);
      break;
    default:
      // fprintf(stderr, "Debug info: semantic_set_declarations() trying to parse a non-declartion node.\n");
      break;
  }
}

void semantic_check(astree_t* node) {
  int i=0;
  if(!node) return;

  for(i=0; i<MAX_NUMBER_OF_CHILDREN; i++) {
    semantic_check(node->children[i]);
  }

  switch(node->type) {
    case ASTREE_VAR_DEC: {
      // Checks for undeclared variables:
      if(!node->symbol->isVariableOrFuncionDeclared) {
        fprintf(stderr, "Semantic error: variable %s isn't declared.\n", node->symbol->text);
        exit(4);
      }
      break;
    }

    case ASTREE_FUNC_DEC: {
      // Checks for undeclared variables:
      if(!node->symbol->isVariableOrFuncionDeclared) {
        fprintf(stderr, "Semantic error: variable %s isn't declared.\n", node->symbol->text);
        exit(4);
      }
      // Checks for nature:
      if(node->symbol->nature != NATURE_FUNCTION) {
        fprintf(stderr, "Semantic error: %s got a wrong nature (%d), it should be %d.\n", node->symbol->text, node->symbol->nature, NATURE_FUNCTION);
        exit(4);
      }

      // Checks for return type:
      semantic_check_return_t(node->children[2]);
      break;
    }

    case ASTREE_PARAM: {
      // Checks for undeclared variables:
      if(!node->symbol->isVariableOrFuncionDeclared) {
        fprintf(stderr, "Semantic error: variable %s isn't declared.\n", node->symbol->text);
        exit(4);
      }
        break;
    }

    case ASTREE_KW_READ: {
      // Checks for undeclared variables:
      if(!node->symbol->isVariableOrFuncionDeclared) {
        fprintf(stderr, "Semantic error: variable %s isn't declared.\n", node->symbol->text);
        exit(4);
      }
        break;
    }

    case ASTREE_KW_PRINT: {
      // Checks for expressions types:
      if(node->children[0]->type != ASTREE_LIT_STRING && node->children[0]->type != ASTREE_PRINT_LST){
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[0])== EXPRESSION_STRING) {
            fprintf(stderr, "Semantic error: print command with invalid types.\n");
            exit(4);
         }
      }
      break;
    }

    case ASTREE_KW_FOR: {
      // Checks for undeclared variables:
      if(!node->symbol->isVariableOrFuncionDeclared) {
        fprintf(stderr, "Semantic error: variable %s isn't declared.\n", node->symbol->text);
        exit(4);
      }
      // Checks for expressions types:
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[0])== EXPRESSION_STRING) {
        fprintf(stderr, "Semantic error: for command with invalid types.\n");
        exit(4);
      }
      if(semantic_get_expression_t(node->children[1]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1])== EXPRESSION_STRING) {
        fprintf(stderr, "Semantic error: for command with invalid types.\n");
        exit(4);
      }
      break;
    }

    case ASTREE_KW_RETURN: {
      // Checks for expressions types:
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[0])== EXPRESSION_STRING) {
        fprintf(stderr, "Semantic error: invalid return type.\n");
        exit(4);
      }
      break;
    }

    case ASTREE_PRINT_LST: {
      // Checks for expressions types:
      if(node->children[0]->type != ASTREE_LIT_STRING) {
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[0])== EXPRESSION_STRING) {
          fprintf(stderr, "Semantic error: print command with invalid types.\n");
          exit(4);
        }
      }
      break;
    }

    case ASTREE_KW_WHEN_THEN: {
      // Checks for expressions types:
      if(semantic_get_expression_t(node->children[0]) != EXPRESSION_BOOLEAN) {
        fprintf(stderr, "Semantic error: when command with invalid types.\n");
        exit(4);
        
      }
      break;
    }

    case ASTREE_KW_WHEN_THEN_ELSE: {
      // Checks for expressions types:
      if(semantic_get_expression_t(node->children[0]) != EXPRESSION_BOOLEAN) {
        fprintf(stderr, "Semantic error: when command with invalid types.\n");
        exit(4);
      }
      break;
    }
    
    case ASTREE_KW_WHILE: {
      // Checks for expressions types:
      if(semantic_get_expression_t(node->children[0]) != EXPRESSION_BOOLEAN) {
        fprintf(stderr, "Semantic error: while command with invalid types.\n");
        exit(4);
      }
      break;
    }

    case ASTREE_ATTRIB: {
      // Checks for undeclared variables:
      if(!node->symbol->isVariableOrFuncionDeclared) {
        fprintf(stderr, "Semantic error: variable %s isn't declared.\n", node->symbol->text);
        exit(4);
      }
      // Checks for nature:
      if(node->symbol->nature != NATURE_VARIABLE && node->symbol->nature != 0) {
        fprintf(stderr, "Semantic error: %s got a wrong nature (%d), it should be %d.\n", node->symbol->text, node->symbol->nature, NATURE_VARIABLE);
        exit(4);
      }
      // Checks for expressions types:
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[0])== EXPRESSION_STRING) {
        fprintf(stderr, "Semantic error: attribuition with invalid types.\n");
        exit(4);
      }
      break;
    }

    case ASTREE_ATTRIB_ARR: {
      // Checks for undeclared variables:
      if(!node->symbol->isVariableOrFuncionDeclared) {
        fprintf(stderr, "Semantic error: variable %s isn't declared.\n", node->symbol->text);
        exit(4);
      }
      // Checks for nature:
      if(node->symbol->nature != NATURE_ARRAY && node->symbol->nature != 0) {
        fprintf(stderr, "Semantic error: %s got a wrong nature (%d), it should be %d.\n", node->symbol->text, node->symbol->nature, NATURE_ARRAY);
        exit(4);
      }
      // Checks for vector index:
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[0])== EXPRESSION_REAL) {
        fprintf(stderr, "Semantic error: vector %s with invalid index.\n", node->symbol->text);
        exit(4);
      }
      // Checks for expressions types:
      if(semantic_get_expression_t(node->children[1]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1])== EXPRESSION_STRING) {
        fprintf(stderr, "Semantic error: attribuition with invalid types.\n");
        exit(4);
      }
      break;
    }
    case ASTREE_TK_ID: {
      // Checks for undeclared variables:
      if(!node->symbol->isVariableOrFuncionDeclared) {
        fprintf(stderr, "Semantic error: variable %s isn't declared.\n", node->symbol->text);
        exit(4);
      }
      // Checks for nature:
      if(node->symbol->nature != NATURE_VARIABLE) {
        fprintf(stderr, "Semantic error: %s got a wrong nature (%d), it should be %d.\n", node->symbol->text, node->symbol->nature, NATURE_VARIABLE);
        exit(4);
      }
      break;
    }

    case ASTREE_ARRAY_CALL: {
      // Checks for undeclared variables:
      if(!node->symbol->isVariableOrFuncionDeclared) {
        fprintf(stderr, "Semantic error: variable %s isn't declared.\n", node->symbol->text);
        exit(4);
      }
      // Checks for nature:
      if(node->symbol->nature != NATURE_ARRAY) {
        fprintf(stderr, "Semantic error: %s got a wrong nature (%d), it should be %d.\n", node->symbol->text, node->symbol->nature, NATURE_ARRAY);
        exit(4);
      }
      // Checks for vector index:
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[0])== EXPRESSION_REAL) {
          fprintf(stderr, "Semantic error: vector %s with invalid index.\n", node->symbol->text);
          exit(4);
	     }
      break;
    }

    case ASTREE_FUNC_CALL: {
      // Checks for undeclared variables:
      if(!node->symbol->isVariableOrFuncionDeclared) {
        fprintf(stderr, "Semantic error: variable %s wasn't declared.\n", node->symbol->text);
        exit(4);
      }
      // Checks for nature:
      if(node->symbol->nature != NATURE_FUNCTION) {
        fprintf(stderr, "Semantic error: %s got a wrong nature (%d), it should be %d.\n", node->symbol->text, node->symbol->nature, NATURE_FUNCTION);
        exit(4);
      }

      // Checks for arguments types:
      semantic_check_arguments_t(node->children[0]);

      // Checks number of arguments
      int parametersNumber = node->symbol->parametersNumber;
      int numArgs = semantic_get_num_arguments(node->children[0]);
      if(numArgs != parametersNumber) {
        fprintf(stderr, "Semantic error: wrong number of arguments: %s() should receive %d arguments.\natureza", node->symbol->text, parametersNumber);
        exit(4);
      }
      break;
    }

    case ASTREE_LEQ: {
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING || semantic_get_expression_t(node->children[1]) == EXPRESSION_STRING){
          fprintf(stderr, "Semantic error: comparing strings. (LEQ)");
          exit(4);
        }
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1]) == EXPRESSION_BOOLEAN) {
          fprintf(stderr, "Semantic error: comparing boolean sizes (LEQ).\n");
          exit(4);
        }
        break;
    }

    case ASTREE_GTE: {
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING || semantic_get_expression_t(node->children[1]) == EXPRESSION_STRING){
          fprintf(stderr, "Semantic error: comparing strings (GTE).\n");
          exit(4);
        }
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1]) == EXPRESSION_BOOLEAN) {
          fprintf(stderr, "Semantic error: comparing boolean sizes (GTE).\n");
          exit(4);
        }
        break;
    }

    case ASTREE_EQU: {
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING || semantic_get_expression_t(node->children[1]) == EXPRESSION_STRING){
          fprintf(stderr, "Semantic error: comparing strings (EQU).\n");
          exit(4);
        }
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1]) == EXPRESSION_BOOLEAN) {
          fprintf(stderr, "Semantic error: comparing boolean sizes (EQU).\n");
          exit(4);
        }
        break;
    }

    case ASTREE_NEQ: {
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING || semantic_get_expression_t(node->children[1]) == EXPRESSION_STRING){
          fprintf(stderr, "Semantic error: comparing strings (NEQ).\n");
          exit(4);
        }
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1]) == EXPRESSION_BOOLEAN) {
          fprintf(stderr, "Semantic error: comparing boolean sizes (NEQ).\n");
          exit(4);
        }
        break;
    }

    case ASTREE_AND: {
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING || semantic_get_expression_t(node->children[1]) == EXPRESSION_STRING){
        fprintf(stderr, "Semantic error: using strings instead booleans (AND).\n");
        exit(4);
      }
      if(semantic_get_expression_t(node->children[0]) != EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1]) != EXPRESSION_BOOLEAN) {
        fprintf(stderr, "Semantic error: using booleans instead numbers (AND).\n");
        exit(4);
      }
      break;
    }

    case ASTREE_OR: {
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING || semantic_get_expression_t(node->children[1]) == EXPRESSION_STRING){
        fprintf(stderr, "Semantic error: using strings instead booleans (OR).\n");
        exit(4);
      }
      if(semantic_get_expression_t(node->children[0]) != EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1]) != EXPRESSION_BOOLEAN) {
        fprintf(stderr, "Semantic error: using booleans instead numbers (OR).\n");
        exit(4);
      }
      break;
    }

    case ASTREE_MUL: {
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING || semantic_get_expression_t(node->children[1]) == EXPRESSION_STRING){
        fprintf(stderr, "Semantic error: multiplying strings.\n");
        exit(4);
      }
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1]) == EXPRESSION_BOOLEAN) {
        fprintf(stderr, "Semantic error: multiplying booleans.\n");
        exit(4);
      }
      break;
    }

    case ASTREE_ADD: {
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING || semantic_get_expression_t(node->children[1]) == EXPRESSION_STRING){
        fprintf(stderr, "Semantic error: adding strings.\n");
        exit(4);
      }
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1]) == EXPRESSION_BOOLEAN) {
        fprintf(stderr, "Semantic error: adding booleans.\n");
        exit(4);
      }
      break;
    }

    case ASTREE_SUB: {
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING || semantic_get_expression_t(node->children[1]) == EXPRESSION_STRING){
        fprintf(stderr, "Semantic error: subtracting strings.\n");
        exit(4);
      }
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1]) == EXPRESSION_BOOLEAN) {
        fprintf(stderr, "Semantic error: subtracting booleans.\n");
        exit(4);
      }
      break;
    }

    case ASTREE_DIV: {
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING || semantic_get_expression_t(node->children[1]) == EXPRESSION_STRING){
        fprintf(stderr, "Semantic error: dividing strings.\n");
        exit(4);
      }
      if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1]) == EXPRESSION_BOOLEAN) {
        fprintf(stderr, "Semantic error: dividing booleans.\n");
        exit(4);
      }
      break;
    }

    case ASTREE_LES: {
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING || semantic_get_expression_t(node->children[1]) == EXPRESSION_STRING){
          fprintf(stderr, "Semantic error: comparing strings (LES).\n");
          exit(4);
        }
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1]) == EXPRESSION_BOOLEAN) {
          fprintf(stderr, "Semantic error: comparing boolean sizes (LES).\n");
          exit(4);
        }
        break;
    }

    case ASTREE_GTR: {
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_STRING || semantic_get_expression_t(node->children[1]) == EXPRESSION_STRING){
          fprintf(stderr, "Semantic error: comparing strings (GTR).\n");
          exit(4);
        }
        if(semantic_get_expression_t(node->children[0]) == EXPRESSION_BOOLEAN || semantic_get_expression_t(node->children[1]) == EXPRESSION_BOOLEAN) {
          fprintf(stderr, "Semantic error: comparing boolean sizes (GTR).\n");
          exit(4);
        }
        break;
    }

    default:
      // fprintf(stderr, "Debug info: ASTREE node of type %d wasn't checked.\n", node->type);
      break;
  }
  // fprintf(stderr, "Debug info: ASTREE node of type %d was semanticaly verified with success!\n", node->type);
  return;
}
