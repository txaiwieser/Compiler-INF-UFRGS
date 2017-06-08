#ifndef SEMANTIC_H
#define SEMANTIC_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "astree.h"

void semanticSetDeclarations(astree_t* node);
void semanticCheck(astree_t* node);

#endif
