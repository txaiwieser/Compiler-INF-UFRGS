#include "compiler.h"
#include <stdio.h>

int comp_asm_generate(tac_t *head, char *output) {

	FILE *fout = fopen(output, "w");
	int r = 0;

	for(tac_t *tac = head; tac; tac = tac->next) {
		
		if(tac->type == TAC_SYMBOL) continue;

		switch(tac->type) {
			//@TODO: insert cases.
			default: r++; fprintf(stderr, "Compiler error: unknown intermediary code.\n"); break;
		}

	}

	fclose(fout);
	return r;
}