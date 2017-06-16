#ifndef SEMANTIC_H
#define SEMANTIC_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hash.h"
#include "astree.h"

void semantic_set_declarations(astree_t* node);
void semantic_check(astree_t* node);

#endif
