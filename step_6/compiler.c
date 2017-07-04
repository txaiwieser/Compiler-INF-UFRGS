#include "compiler.h"
#include "hash.h"
#include "symcodes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *comp_print_section_str() {

	char* new = malloc(66);
	sprintf(new, 	".section	__TEXT,__cstring,cstring_literals\n"
					"L_.str:	.asciz	\"%%d\\n\"\n");
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

	for(int i = 0; i<HASH_SIZE; i++) {
		if(hash_table[i] && !hash_table[i]->isVariableOrFuncionDeclared && hash_table[i]->nature != NATURE_TEMPORARY) {
			char *addition = (char *)malloc(+1 +2*strlen(hash_table[i]->text) +10);
			sprintf(addition, "_%s: .long\t%s\n", hash_table[i]->text, hash_table[i]->text);
			imediate_decs = comp_append_str(imediate_decs, addition);
			free(addition);
		}
	}

	return imediate_decs;
}

char *comp_variable_sec_str() {

	char *variable_decs = malloc(24);
	sprintf(variable_decs, ".section	__DATA,__data\n");

	for(int i = 0; i<HASH_SIZE; i++) {
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
	for(int j = 0; j<=i; j++) {
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

	for(tac_t *tac = head; tac; tac = tac->prev) {

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
			case TAC_INC: fprintf(fout, "\t# TAC_INC\n"); break;
			case TAC_ADD: fprintf(fout, "\t# TAC_ADD\n"
										"\tmovl _%s(%%rip), %%eax\n"
										"\taddl _%s(%%rip), %%eax\n"
										"\tmovl %%eax, _%s(%%rip)\n",
										tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_SUB: fprintf(fout, "\t# TAC_SUB\n"); break;
			case TAC_MUL: fprintf(fout, "\t# TAC_MUL\n"); break;
			case TAC_DIV: fprintf(fout, "\t# TAC_DIV\n"); break;
			case TAC_BLE: fprintf(fout, "\t# TAC_BLE\n"); break;
			case TAC_BGE: fprintf(fout, "\t# TAC_BGE\n"); break;
			case TAC_BEQ: fprintf(fout, "\t# TAC_BEQ\n"); break;
			case TAC_BNE: fprintf(fout, "\t# TAC_BNE\n"); break;
			case TAC_AND: fprintf(fout, "\t# TAC_AND\n"); break;
			case TAC_OR : fprintf(fout, "\t# TAC_OR\n"); break;
			case TAC_BLT: fprintf(fout, "\t# TAC_BLT\n"); break;
			case TAC_BGT: fprintf(fout, "\t# TAC_BGT\n"); break;
			case TAC_LABEL: fprintf(fout, "\t# TAC_LABEL\n"); break;
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
			case TAC_IFZ: fprintf(fout, "\t# TAC_IFZ\n"); break;
			case TAC_JUMP: fprintf(fout, "\t# TAC_JUMP\n"); break;
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
			case TAC_READ: fprintf(fout, "\t# TAC_READ\n"); break;
			default: r++; fprintf(stderr, "Compiler error: unknown intermediary code.\n"); break;
		}
	}
	
	fprintf(fout, "\n%s\n%s\n%s\n", variable_decs, imediate_decs, comp_print_section_str());
	fclose(fout);
	return r;
}