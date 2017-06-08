#include "semantic.h"
#include "symcodes.h"

void checkArgsTypes(astree_t* node) {
  if (!node) return;
  if (node->children[0]->symbol->expressionType == EXPRESSION_BOOLEAN) {
    fprintf(stderr, "ERRO SEMANTICO\nargumento %s nao pode ser booleano\n", node->children[0]->symbol->text);
    exit(4);
  } else if (node->children[0]->symbol->expressionType == EXPRESSION_STRING) {
    fprintf(stderr, "ERRO SEMANTICO\nargumento %s nao pode ser string\n", node->children[0]->symbol->text);
    exit(4);
  }else if(node->type == ASTREE_FUNC_ARGS) {
    checkArgsTypes(node->children[1]);
    return;
  } else if(node->type == ASTREE_FUNC_ARGS_EXT) {
    checkArgsTypes(node->children[1]);
    return;
  }
  //nao é pra chegar ate aqui
  fprintf(stderr, "ERRO Q N ERA PRA ACONTECER\ngetNumArgs");
  return;
}

int getExprType(astree_t* node) {
  switch (node->type) {
    case ASTREE_EXP_PARENTHESIS:
      return getExprType(node->children[0]);
      break;
    case ASTREE_TK_ID:
      return node->symbol->expressionType;
      break;
    case ASTREE_ARRAY_CALL:
      return node->symbol->expressionType;
      break;
    case ASTREE_FUNC_CALL:
      return node->symbol->expressionType;
      break;
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
    case ASTREE_LEQ:
      return EXPRESSION_BOOLEAN;
      break;
    case ASTREE_GTE:
      return EXPRESSION_BOOLEAN;
      break;
    case ASTREE_EQU:
      return EXPRESSION_BOOLEAN;
      break;
    case ASTREE_NEQ:
      return EXPRESSION_BOOLEAN;
      break;
    case ASTREE_AND:
      return EXPRESSION_BOOLEAN;
      break;
    case ASTREE_OR:
      return EXPRESSION_BOOLEAN;
      break;
    case ASTREE_MUL:
      if (getExprType(node->children[0]) == EXPRESSION_REAL || getExprType(node->children[1]) == EXPRESSION_REAL) {
        return EXPRESSION_REAL;
      } else {
        return EXPRESSION_INTEGER;
      }
      break;
    case ASTREE_ADD:
      if (getExprType(node->children[0]) == EXPRESSION_REAL || getExprType(node->children[1]) == EXPRESSION_REAL) {
        return EXPRESSION_REAL;
      } else {
        return EXPRESSION_INTEGER;
      }
      break;
    case ASTREE_SUB:
      if (getExprType(node->children[0]) == EXPRESSION_REAL || getExprType(node->children[1]) == EXPRESSION_REAL) {
        return EXPRESSION_REAL;
      } else {
        return EXPRESSION_INTEGER;
      }
      break;
    case ASTREE_DIV:
      return EXPRESSION_REAL;
      break;
    case ASTREE_LES:
      return EXPRESSION_BOOLEAN;
      break;
    case ASTREE_GTR:
      return EXPRESSION_BOOLEAN;
      break;
  }
  printf("ERROR 29845\n");
  return -1;
}

int getNumArgs(astree_t* node) {
    if (!node) return 0;
    if (node->type == ASTREE_FUNC_ARGS) {
      return 1 + getNumArgs(node->children[1]);
    } else if (node->type == ASTREE_FUNC_ARGS_EXT) {
      return 1 + getNumArgs(node->children[1]);
    } else {
      fprintf(stderr, "ERRO 439807");
      return 0;
  }
}

int getParametersNumber(astree_t* node) {
  if (node->type==ASTREE_PARAM) {
    return 1;
  } else if (node->children[1]->type==ASTREE_PARAM) {
    return 2;
  } else if (node->children[1]->type==ASTREE_PARAM_LST) {
    return 1 + getParametersNumber(node->children[1]);
  } else {
    fprintf(stderr, "ERROR 9543\n" );
    return -1;
  }
}

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

void findReturn(astree_t* node) {
    int i;
    if (!node) return;
    if (node->type == ASTREE_KW_RETURN) {
	      if (getExprType(node->children[0]) == EXPRESSION_BOOLEAN || getExprType(node->children[0]) == EXPRESSION_STRING) {     //Dúvida: os DataTypes são intercambiáveis? Exemplo: long a (byte x, long y) return x+y
          fprintf(stderr, "ERRO SEMANTICO\nretorno invalido \n");
          exit(4);
        }
    }
    for (i=0; i < MAX_NUMBER_OF_CHILDREN; i++) {
        findReturn(node->children[i]);
    }
}

void semanticFunctionDeclaration(astree_t* node) {
  if (node->symbol) {
    if (node->symbol->isVariableOrFuncionDeclared) {
      fprintf(stderr, "ERRO SEMANTICO\nfuncao ja declarada: %s\n", node->symbol->text);
      exit(4);
    }

    node->symbol->isVariableOrFuncionDeclared = 1;

    if (node->symbol->type == SYMBOL_IDENTIFIER && node->children[0]){
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
        fprintf(stderr, "ERROR 2437\n");
        break;
    }

    if (node->children[1] == 0) {
      node->symbol->parametersNumber = 0;
    } else{
      node->symbol->parametersNumber = getParametersNumber(node->children[1]);
    }
  } else{
    fprintf(stderr, "ERROR 2345\n");
  }
}

void semanticParameters(astree_t* node) {
  if (node->symbol) {
    if (node->symbol->isVariableOrFuncionDeclared) {
      fprintf(stderr, "ERRO SEMANTICO\nvariavel de parametro ja declarada: %s\n", node->symbol->text);
      exit(4);
    }

    node->symbol->isVariableOrFuncionDeclared = 1;

    if (node->symbol->type == SYMBOL_IDENTIFIER && node->children[0]) {
      node->symbol->nature = NATURE_VARIABLE;
    }

    switch (node->children[0]->type) {
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
        fprintf(stderr, "ERROR 24566\n");
        break;
    }
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
      semanticFunctionDeclaration(node);
      break;
    case ASTREE_PARAM:
      semanticParameters(node);
      break;
    default:
      fprintf(stderr, "ERROR 345\n");
      break;
  }
}

void semanticCheck(astree_t* node) {
  int i=0;
  if (!node) return;

  for(i=0; i<MAX_NUMBER_OF_CHILDREN; i++) {
    semanticCheck(node->children[i]);
  }

  switch (node->type) {
    case ASTREE_VAR_DEC:
    {
      //verif vars nao declaradas
      if(!node->symbol->isVariableOrFuncionDeclared)
      {
        fprintf(stderr, "ERRO SEMANTICO\nvariavel nao declarada: %s\n", node->symbol->text);
        exit(4);
      }
      break;
    }

    case ASTREE_FUNC_DEC:
    {
      //verif vars nao declaradas
      if(!node->symbol->isVariableOrFuncionDeclared)
      {
        fprintf(stderr, "ERRO SEMANTICO\nvariavel nao declarada: %s\n", node->symbol->text);
        exit(4);
      }
      //verifica natureza
      if(node->symbol->nature != NATURE_FUNCTION)
      {
        fprintf(stderr, "ERRO SEMANTICO\n Natureza incompativel: %s eh da natureza: %i ,  esperado: %i\n",
                node->symbol->text, node->symbol->nature, NATURE_FUNCTION);
        exit(4);
      }

      //verifica se o tipo do return esta correto
      findReturn(node->children[2]);

      break;
    }

    case ASTREE_PARAM:
    {
      //verif vars nao declaradas
      if(!node->symbol->isVariableOrFuncionDeclared)
      {
        fprintf(stderr, "ERRO SEMANTICO\nvariavel nao declarada: %s\n", node->symbol->text);
        exit(4);
      }
        break;
    }

    case ASTREE_KW_READ:
    {
      //verif vars nao declaradas
      if(!node->symbol->isVariableOrFuncionDeclared)
      {
        fprintf(stderr, "ERRO SEMANTICO\nvariavel nao declarada: %s\n", node->symbol->text);
        exit(4);
      }
        break;
    }

    case ASTREE_KW_FOR:
    {
      //verif vars nao declaradas
      if(!node->symbol->isVariableOrFuncionDeclared)
      {
        fprintf(stderr, "ERRO SEMANTICO\nvariavel nao declarada: %s\n", node->symbol->text);
        exit(4);
      }
        break;
    }

    case ASTREE_ATTRIB:
    {
        //verif vars nao declaradas
        if(!node->symbol->isVariableOrFuncionDeclared)
        {
          fprintf(stderr, "ERRO SEMANTICO\nvariavel nao declarada: %s\n", node->symbol->text);
          exit(4);
        }
        //verifica natureza
        if(node->symbol->nature != NATURE_VARIABLE)
        {
          fprintf(stderr, "ERRO SEMANTICO\n Natureza incompativel: %s eh da natureza: %i ,  esperado: %i\n",
                  node->symbol->text, node->symbol->nature, NATURE_VARIABLE);
          exit(4);
        }
        break;
    }

    case ASTREE_ATTRIB_ARR:
    {
      //verif vars nao declaradas
      if(!node->symbol->isVariableOrFuncionDeclared)
      {
        fprintf(stderr, "ERRO SEMANTICO\nvariavel nao declarada: %s\n", node->symbol->text);
        exit(4);
      }
      //verifica natureza
      if(node->symbol->nature != NATURE_ARRAY)
      {
        fprintf(stderr, "ERRO SEMANTICO\n Natureza incompativel: %s eh da natureza: %i ,  esperado: %i\n",
                node->symbol->text, node->symbol->nature, NATURE_ARRAY);
        exit(4);
      }
     //verifica indice do vetor
      if(getExprType(node->children[0]) == EXPRESSION_BOOLEAN || getExprType(node->children[0])== EXPRESSION_REAL)
	     {
	        fprintf(stderr, "ERRO SEMANTICO\nIndice do vetor %s invalido.\n", node->symbol->text);
          exit(4);
	     }
        break;
    }

    case ASTREE_TK_ID:
    {
      //verif vars nao declaradas
      if(!node->symbol->isVariableOrFuncionDeclared)
      {
        fprintf(stderr, "ERRO SEMANTICO\nvariavel nao declarada: %s\n", node->symbol->text);
        exit(4);
      }

      //verifica natureza
      if(node->symbol->nature != NATURE_VARIABLE)
      {
        fprintf(stderr, "ERRO SEMANTICO\n Natureza incompativel: %s eh da natureza: %i ,  esperado: %i\n",
                node->symbol->text, node->symbol->nature, NATURE_VARIABLE);
        exit(4);
      }
      break;
    }

    case ASTREE_ARRAY_CALL:
    {
      //verif vars nao declaradas
      if(!node->symbol->isVariableOrFuncionDeclared)
      {
        fprintf(stderr, "ERRO SEMANTICO\nvariavel nao declarada: %s\n", node->symbol->text);
        exit(4);
      }
      //verifica natureza
      if(node->symbol->nature != NATURE_ARRAY)
      {
        fprintf(stderr, "ERRO SEMANTICO\n Natureza incompativel: %s é da natureza %i ,  esperado %i\n",
                node->symbol->text, node->symbol->nature, NATURE_ARRAY);
        exit(4);
      }
      //verifica indice do vetor
      if(getExprType(node->children[0]) == EXPRESSION_BOOLEAN || getExprType(node->children[0])== EXPRESSION_REAL)
	     {
	        fprintf(stderr, "ERRO SEMANTICO\nIndice do vetor %s invalido.\n", node->symbol->text);
          exit(4);
	     }
      break;
    }

    case ASTREE_FUNC_CALL:
    {
      //verif vars nao declaradas
      if(!node->symbol->isVariableOrFuncionDeclared)
      {
        fprintf(stderr, "ERRO SEMANTICO\nvariavel nao declarada: %s\n", node->symbol->text);
        exit(4);
      }
      //verifica natureza
      if(node->symbol->nature != NATURE_FUNCTION)
      {
        fprintf(stderr, "ERRO SEMANTICO\n Natureza incompativel: %s é da natureza %i ,  esperado %i\n",
                node->symbol->text, node->symbol->nature, NATURE_FUNCTION);
        exit(4);
      }

      //verifica tipo dos argumentos
      checkArgsTypes(node->children[0]);

      //verifica num de argumentos
      int parametersNumber = node->symbol->parametersNumber;
      int numArgs = getNumArgs(node->children[0]);
      if(numArgs != parametersNumber)
      {
        fprintf(stderr, "ERRO SEMANTICO\n numero de errado argumentos: %s é deveria receber %i argumentos,  mas recebeu %i\n",
                node->symbol->text, parametersNumber, numArgs);
        exit(4);
      }
      break;
    }

    case ASTREE_LEQ:
    {
        if(getExprType(node->children[0]) == EXPRESSION_STRING || getExprType(node->children[1]) == EXPRESSION_STRING){
          fprintf(stderr, "ERRO SEMANTICO\ncomparando strings");
          exit(4);
        }
        if(getExprType(node->children[0]) == EXPRESSION_BOOLEAN || getExprType(node->children[1]) == EXPRESSION_BOOLEAN)
        {
          fprintf(stderr, "ERRO SEMANTICO\ncomparando tamanhos de booleanos");
          exit(4);
        }
        break;
    }

    case ASTREE_GTE:
    {
        if(getExprType(node->children[0]) == EXPRESSION_STRING || getExprType(node->children[1]) == EXPRESSION_STRING){
          fprintf(stderr, "ERRO SEMANTICO\ncomparando strings\n");
          exit(4);
        }
        if(getExprType(node->children[0]) == EXPRESSION_BOOLEAN || getExprType(node->children[1]) == EXPRESSION_BOOLEAN)
        {
          fprintf(stderr, "ERRO SEMANTICO\ncomparando tamanhos de booleanos\n");
          exit(4);
        }
        break;
    }

    case ASTREE_EQU:
    {
        if(getExprType(node->children[0]) == EXPRESSION_STRING || getExprType(node->children[1]) == EXPRESSION_STRING){
          fprintf(stderr, "ERRO SEMANTICO\ncomparando strings\n");
          exit(4);
        }
        if(getExprType(node->children[0]) == EXPRESSION_BOOLEAN || getExprType(node->children[1]) == EXPRESSION_BOOLEAN)
        {
          fprintf(stderr, "ERRO SEMANTICO\ncomparando tamanhos de booleanos\n");
          exit(4);
        }
        break;
    }

    case ASTREE_NEQ:
    {
        if(getExprType(node->children[0]) == EXPRESSION_STRING || getExprType(node->children[1]) == EXPRESSION_STRING){
          fprintf(stderr, "ERRO SEMANTICO\ncomparando strings\n");
          exit(4);
        }
        if(getExprType(node->children[0]) == EXPRESSION_BOOLEAN || getExprType(node->children[1]) == EXPRESSION_BOOLEAN)
        {
          fprintf(stderr, "ERRO SEMANTICO\ncomparando tamanhos de booleanos\n");
          exit(4);
        }
        break;
    }

    case ASTREE_AND:
    {
      if(getExprType(node->children[0]) == EXPRESSION_STRING || getExprType(node->children[1]) == EXPRESSION_STRING){
        fprintf(stderr, "ERRO SEMANTICO\nusando strings no lugar de booleanos\n");
        exit(4);
      }
      if(getExprType(node->children[0]) != EXPRESSION_BOOLEAN || getExprType(node->children[1]) != EXPRESSION_BOOLEAN)
      {
        fprintf(stderr, "ERRO SEMANTICO\nusando numeros no lugar de booleanos\n");
        exit(4);
      }
      break;
    }

    case ASTREE_OR:
    {
      if(getExprType(node->children[0]) == EXPRESSION_STRING || getExprType(node->children[1]) == EXPRESSION_STRING){
        fprintf(stderr, "ERRO SEMANTICO\nusando strings no lugar de booleanos\n");
        exit(4);
      }
      if(getExprType(node->children[0]) != EXPRESSION_BOOLEAN || getExprType(node->children[1]) != EXPRESSION_BOOLEAN)
      {
        fprintf(stderr, "ERRO SEMANTICO\nusando numeros no lugar de booleanos\n");
        exit(4);
      }
      break;
    }

    case ASTREE_MUL:
    {
      if(getExprType(node->children[0]) == EXPRESSION_STRING || getExprType(node->children[1]) == EXPRESSION_STRING){
        fprintf(stderr, "ERRO SEMANTICO\nmultiplicando strings\n");
        exit(4);
      }
      if(getExprType(node->children[0]) == EXPRESSION_BOOLEAN || getExprType(node->children[1]) == EXPRESSION_BOOLEAN)
      {
        fprintf(stderr, "ERRO SEMANTICO\n multiplicando booleanos\n");
        exit(4);
      }
      break;
    }

    case ASTREE_ADD:
    {
      if(getExprType(node->children[0]) == EXPRESSION_STRING || getExprType(node->children[1]) == EXPRESSION_STRING){
        fprintf(stderr, "ERRO SEMANTICO\nsomando strings\n");
        exit(4);
      }
      if(getExprType(node->children[0]) == EXPRESSION_BOOLEAN || getExprType(node->children[1]) == EXPRESSION_BOOLEAN)
      {
        fprintf(stderr, "ERRO SEMANTICO\n somando booleanos\n");
        exit(4);
      }
      break;
    }

    case ASTREE_SUB:
    {
      if(getExprType(node->children[0]) == EXPRESSION_STRING || getExprType(node->children[1]) == EXPRESSION_STRING){
        fprintf(stderr, "ERRO SEMANTICO\n subtraindo strings\n");
        exit(4);
      }
      if(getExprType(node->children[0]) == EXPRESSION_BOOLEAN || getExprType(node->children[1]) == EXPRESSION_BOOLEAN)
      {
        fprintf(stderr, "ERRO SEMANTICO\n subtraindo booleanos\n");
        exit(4);
      }
      break;
    }

    case ASTREE_DIV:
    {
      if(getExprType(node->children[0]) == EXPRESSION_STRING || getExprType(node->children[1]) == EXPRESSION_STRING){
        fprintf(stderr, "ERRO SEMANTICO\n dividindo strings\n");
        exit(4);
      }
      if(getExprType(node->children[0]) == EXPRESSION_BOOLEAN || getExprType(node->children[1]) == EXPRESSION_BOOLEAN)
      {
        fprintf(stderr, "ERRO SEMANTICO\n dividindo booleanos\n");
        exit(4);
      }
      break;
    }

    case ASTREE_LES:
    {
        if(getExprType(node->children[0]) == EXPRESSION_STRING || getExprType(node->children[1]) == EXPRESSION_STRING){
          fprintf(stderr, "ERRO SEMANTICO\ncomparando strings\n");
          exit(4);
        }
        if(getExprType(node->children[0]) == EXPRESSION_BOOLEAN || getExprType(node->children[1]) == EXPRESSION_BOOLEAN)
        {
          fprintf(stderr, "ERRO SEMANTICO\ncomparando tamanhos de booleanos\n");
          exit(4);
        }
        break;
    }

    case ASTREE_GTR:
    {
        if(getExprType(node->children[0]) == EXPRESSION_STRING || getExprType(node->children[1]) == EXPRESSION_STRING){
          fprintf(stderr, "ERRO SEMANTICO\ncomparando strings\n");
          exit(4);
        }
        if(getExprType(node->children[0]) == EXPRESSION_BOOLEAN || getExprType(node->children[1]) == EXPRESSION_BOOLEAN)
        {
          fprintf(stderr, "ERRO SEMANTICO\ncomparando tamanhos de booleanos\n");
          exit(4);
        }
        break;
    }

    default:
      //fprintf(stderr, "ERRO QUE NAO DEVIA ACONTECER\n");
      //fprintf(stderr, "tipo da astree fora do switch case\n");
      //fprintf(stderr, "(semanticSetDeclarations)\n");
      break;
  }

  //nenhum erro semantico encontrado
  return;
}
