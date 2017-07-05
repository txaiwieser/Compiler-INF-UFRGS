//@TODO: booleans should be solved here, not in tac.c;
//@TODO: function should return in a register. How to make it work in expressions and attribuitions?
//@TODO: it should work with different data types. Now it only works with integers (as longs);
//@TODO: array support.
//@TODO: final tests.

#include "compiler.h"
#include "hash.h"
#include "symcodes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *comp_print_section_str() {

	char* new = malloc(66);
	sprintf(new, 	".section	__TEXT,__cstring,cstring_literals\n"
					"L_.str:	.asciz	\"%%d\\n\"\n"
					"L_.str.1:	.asciz	\"%%d\"\n");
	return new;
}

char *comp_append_str(char* string, char* addition) {
	char* new = malloc(strlen(string)+strlen(addition)+1);
	new = strcpy(new, string);
	strcat(new, addition);
	return new;
}

char *comp_imediate_sec_str() {

	char *imediate_decs = malloc(25);
	sprintf(imediate_decs, ".section	__TEXT,__const\n");

	printf("\nDebug: comp_imediate_sec_str() (' = added as \"global immediate\")\n"); // @TODO: comment/remove this.
	int i;
	for(i = 0; i<HASH_SIZE; i++) {

		if(!hash_table[i]) continue;

		hash_node_t *node;
		for(node = hash_table[i]; node; node = node->next) {
			printf("%s", node->text); // @TODO: comment/remove this.
			if(node->isVariableOrFuncionDeclared == 0 && node->nature != NATURE_TEMPORARY) {
				printf("\'"); // @TODO: comment/remove this.
				char *addition = (char *)malloc(+1 +2*strlen(node->text) +10);
				sprintf(addition, "_%s: .long\t%s\n", node->text, node->text);
				imediate_decs = comp_append_str(imediate_decs, addition);
				free(addition);
				break;
			}
			printf(" "); // @TODO: comment/remove this.
		}
		printf("\n"); // @TODO: comment/remove this.
	}

	printf("\n"); // @TODO: comment/remove this.
	return imediate_decs;
}

char *comp_variable_sec_str() {

	char *variable_decs = malloc(24);
	sprintf(variable_decs, ".section	__DATA,__data\n");

	int i;
	for(i = 0; i<HASH_SIZE; i++) {
		if(hash_table[i] && hash_table[i]->nature == NATURE_TEMPORARY) {
			char *addition = (char *)malloc(+1 +2*strlen(hash_table[i]->text) +10);
			sprintf(addition, "_%s: .long\t%s\n", hash_table[i]->text, "0");
			variable_decs = comp_append_str(variable_decs, addition);
			free(addition);
		}
	}

	return variable_decs;
}

tac_t *comp_fill_parameter(tac_t *arg) {

	int i = 0;
	tac_t *func_call = arg;
	while(func_call->type != TAC_FCALL) {
		func_call = func_call->prev;
		if(func_call->type == TAC_ARG) i++;
	}

	tac_t *func_dec;
	for(func_dec = arg; !(func_dec->type == TAC_BEGINFUN && strcmp(func_call->op1->text, func_dec->res->text) == 0); func_dec = func_dec->next);
	tac_t *param = func_dec;
	int j;
	for(j = 0; j<=i; j++) {
		param = param->prev;
	}

	return param;
}

int comp_asm_generate(tac_t *head, char *output) {

	FILE *fout = fopen(output, "w");
	int r = 0;

	char *variable_decs;
	char *imediate_decs;
	variable_decs = comp_variable_sec_str();
	imediate_decs = comp_imediate_sec_str();
	char *addition;

	fprintf(fout, ".section	__TEXT,__text,regular,pure_instructions\n.globl _main\n");

	tac_t *tac;
	for(tac = head; tac; tac = tac->prev) {

		switch(tac->type) {

			case TAC_SYMBOL: break;
			case TAC_VAR:
			case TAC_PARAM:
				addition = (char *)malloc(+1 +2*strlen(tac->res->text) +10);
				sprintf(addition, "_%s: .long\t%s\n", tac->res->text, tac->op1 ? tac->op1->text : "0");
				variable_decs = comp_append_str(variable_decs, addition);
				free(addition);
				break;
			case TAC_ARR: fprintf(fout, "\t# TAC_ARR\n"); break;
			case TAC_MOVE: fprintf(fout,	"\t# TAC_MOVE\n"
											"\tmovl\t_%s(%%rip), %%eax\n"
											"\tmovl\t%%eax, _%s(%%rip)\n",
											tac->op1->text, tac->res->text); break;
			case TAC_INC: fprintf(fout, 	"\t# TAC_INC\n"
											"\tincl _%s(%%rip)\n", tac->res->text); break;
			case TAC_ADD: fprintf(fout, "\t# TAC_ADD\n"
										"\tmovl _%s(%%rip), %%eax\n"
										"\taddl _%s(%%rip), %%eax\n"
										"\tmovl %%eax, _%s(%%rip)\n",
										tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_SUB: fprintf(fout, "\t# TAC_SUB\n"
										"\tmovl _%s(%%rip), %%eax\n"
										"\tsubl _%s(%%rip), %%eax\n"
										"\tmovl %%eax, _%s(%%rip)\n",
										tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_MUL: fprintf(fout, "\t# TAC_MUL\n"
										"\tmovl _%s(%%rip), %%eax\n"
										"\timull _%s(%%rip), %%eax\n"
										"\tmovl %%eax, _%s(%%rip)\n",
										tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_DIV: fprintf(fout, "\t# TAC_DIV\n"
										"\tmovl _%s(%%rip), %%eax\n"
										"\tidivl _%s(%%rip), %%eax\n"
										"\tmovl %%eax, _%s(%%rip)\n",
										tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_BLE: fprintf(fout, "\t# TAC_BLE\n"
										"\tmovl _%s(%%rip), %%edx\n"
										"\tcmpl _%s(%%rip), %%edx\n"
										"\tjle _%s\n", tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_BGE: fprintf(fout, "\t# TAC_BGE\n"
										"\tmovl _%s(%%rip), %%edx\n"
										"\tcmpl _%s(%%rip), %%edx\n"
										"\tjge _%s\n", tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_BEQ: fprintf(fout, "\t# TAC_BEQ\n"
										"\tmovl _%s(%%rip), %%edx\n"
										"\tcmpl _%s(%%rip), %%edx\n"
										"\tje _%s\n", tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_BNE: fprintf(fout, "\t# TAC_BNE\n"
										"\tmovl _%s(%%rip), %%edx\n"
										"\tcmpl _%s(%%rip), %%edx\n"
										"\tjne _%s\n", tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_AND: fprintf(fout,	"\t# TAC_AND\n"
										"\tmovl _%s(%%rip), %%eax\n"
										"\tmovl _%s(%%rip), %%edx\n"
										"\tandl %%edx, %%eax\n"
										"\tmovl %%eax, _%s(%%rip)", tac->op1->text,
										tac->op2->text, tac->res->text); break;

			case TAC_OR : fprintf(fout,	"\t# TAC_OR\n"
										"\tmovl _%s(%%rip), %%eax\n"
										"\tmovl _%s(%%rip), %%edx\n"
										"\torl  %%edx, %%eax\n"
										"\tmovl %%eax, _%s(%%rip)\n", tac->op1->text,
										tac->op2->text, tac->res->text); break;
			case TAC_BLT: fprintf(fout, "\t# TAC_BLT\n"
										"\tmovl _%s(%%rip), %%edx\n"
										"\tcmpl _%s(%%rip), %%edx\n"
										"\tjl _%s\n", tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_BGT: fprintf(fout, "\t# TAC_BGT\n"
										"\tmovl _%s(%%rip), %%edx\n"
										"\tcmpl _%s(%%rip), %%edx\n"
										"\tjg _%s\n", tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_LABEL: fprintf(fout, 	"\t# TAC_LABEL\n"
											"_%s:\n", tac->res->text); break;
			case TAC_BEGINFUN: fprintf(fout, 	"\t# TAC_BEGINFUN\n"
												"_%s:\n"
												"\t.cfi_startproc\n"
												"\tpushq\t%%rbp\n"
												"\tmovq\t%%rsp, %%rbp\n",
												tac->res->text); break;
			case TAC_ENDFUN: fprintf(fout,	"\t# TAC_ENDFUN\n"
											"\t.cfi_endproc\n"); break;
			case TAC_FCALL: fprintf(fout,	"\t# TAC_FCALL\n"
											"\tcallq\t_%s\n", tac->op1->text); break;
			case TAC_ACALL: fprintf(fout, "\t# TAC_ACALL\n"); break;
			case TAC_AATTRIB: fprintf(fout, "\t# TAC_AATTRIB\n"); break;
			case TAC_IFZ: fprintf(fout,	"\t# TAC_IFZ\n"
										"\tcmpl $0, _%s(%%rip)\n"
										"\tje _%s\n", tac->op1->text, tac->res->text); break;
			case TAC_JUMP: fprintf(fout,	"\t# TAC_JUMP\n"
											"\tjmp _%s\n",
											tac->res->text); break;
			case TAC_CALL: fprintf(fout, "\t# TAC_CALL\n"); break;
			case TAC_ARG: fprintf(fout,	"\t# TAC_ARG\n"
										"\tmovl _%s(%%rip), %%eax\n"
										"\tmovl %%eax, _%s(%%rip)\n", tac->res->text,
										comp_fill_parameter(tac)->res->text); break;
			case TAC_RET: fprintf(fout,	"\t# TAC_RET\n"
										"\tmovl\t_%s(%%rip), %%eax\n"
										"\tpopq\t%%rbp\n"
										"\tretq\n", tac->res->text); break;
			case TAC_PRINT: fprintf(fout, 	"\t# TAC_PRINT\n"
											"\tmovl\t_%s(%%rip), %%esi\n"
											"\tleaq\tL_.str(%%rip), %%rdi\n"
											"\tcallq\t_printf\n", tac->res->text); break;
			case TAC_READ: fprintf(fout, "\t# TAC_READ\n"
											"\tleaq\tL_.str.1(%%rip), %%rdi\n"
											"\tleaq\t_%s(%%rip), %%rsi\n"
											"\tcallq\t_scanf\n", tac->res->text); break;
			default: r++; fprintf(stderr, "Compiler error: unknown intermediary code.\n"); break;
		}
	}
	
	fprintf(fout, "\n%s\n%s\n%s\n", variable_decs, imediate_decs, comp_print_section_str());
	fclose(fout);
	return r;
}