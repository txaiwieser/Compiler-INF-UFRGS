#include <stdio.h>
#include "astree.h"

astree_t* astree_create(int type, hash_node_t *symbol, astree_t *son_1, astree_t *son_2, astree_t *son_3, astree_t *son_4) {
    
    astree_t* newnode = 0;
    newnode = calloc(1, sizeof(astree_t));
    
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->son[0] = son1;
    newnode->son[1] = son2;
    newnode->son[2] = son3;
    newnode->son[3] = son4;

    return newnode;
}

void astree_print_tree(astree_t *node, int level) {
    
    int i;
    
    if(!node) return;
    for(i=0; i < level; i++) fprintf(stderr, "  ");
    
    fprintf(stderr, "ASTREE(");

    switch(node->type) {
        case ASTREE_SYMBOL:
            fprintf(stderr, "ASTREE_SYMBOL");
            break;
        case ASTREE_ADD:
            fprintf(stderr,"ASTREE_ADD");
            break;
    }
    
    if(node->symbol)
        fprintf(stderr, ",%s", node->symbol->text); // CHECK print (?)
    else
        fprintf(stderr, ",");
    
    for(i=0; i < MAX_SONS; i++) astree_print_tree(node->son[i], level+1);

}

void astree_print_node(astree_t *node) {
    if(!node) return;

    fprintf(stderr, "ASTREE NODE(");
    
    switch(node->type) {
        case ASTREE_SYMBOL:
            fprintf(stderr, "ASTREE_SYMBOL");
            break;
        case ASTREE_ADD:
            fprintf(stderr,"ASTREE_ADD");
            break;
    }

    if(node->symbol)
        fprintf(stderr, ",%s", node->symbol->text); // CHECK print (?)
    else
        fprintf(stderr, ",");
}