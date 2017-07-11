//@TODO: booleans should be solved here, not in tac.c;
//@TODO: function should return in a register. How to make it work in expressions and attribuitions?
//@TODO: it should work with different data types. Now it only works with integers (as longs);
//@TODO: final tests.

#include "compiler.h"
#include "hash.h"
#include "symcodes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_PARAM_REGS 6

const char *param_regs_64[] = { "%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9" };
const char *param_regs_32[] = { "%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d" };

unsigned int con_str_count = 0;
unsigned int lit_str_count = 0;

char *comp_literal_str_sec_str() {

	char* new = malloc(44);
	sprintf(new, ".section\t__TEXT,__cstring,cstring_literals\n");
	return new;
}

char *comp_append_str(char* string, char* addition) {
	char* new = malloc(strlen(string)+strlen(addition)+1);
	new = strcpy(new, string);
	strcat(new, addition);
	return new;
}


char *com_generate_print(tac_t *print, char **imediate_decs, char **lit_str_section) {

	char *assemby = malloc(sizeof(""));
	assemby[0] = '\0';

	char lit_srt_sec_dec[30];
	sprintf(lit_srt_sec_dec, "L_.str.%d: .asciz\t\"", lit_str_count);
	char *lit_srt_sec_dec_p = &lit_srt_sec_dec[0];
	char s[] = "%s";
	char d[] = "%d";

	char *addition = NULL;
	int regc = 1;
	tac_t *arg;
	for(arg = print->next; arg->type == TAC_PRARG && regc < NUM_PARAM_REGS; arg = arg->next) {

		hash_node_t* symbol = arg->res;
		switch(symbol->type) {
			case SYMBOL_LIT_STRING:
				addition = realloc(addition, +strlen(symbol->text) +30);
				sprintf(addition, "_string_%d: .asciz\t%s\n", con_str_count, symbol->text);
				*imediate_decs = comp_append_str(*imediate_decs, addition);
				
				addition = realloc(addition, +strlen(param_regs_64[regc]) +34);
				sprintf(addition, "\tleaq\t_string_%d(%%rip), %s\n", con_str_count, param_regs_64[regc]);
				assemby = comp_append_str(assemby, addition);

				lit_srt_sec_dec_p = comp_append_str(lit_srt_sec_dec_p, s);
				
				con_str_count++;
				break;
			default:
				addition = realloc(addition, +17 +strlen(symbol->text) +strlen(param_regs_32[regc]));
				sprintf(addition, "\tmovl\t_%s(%%rip), %s\n", symbol->text, param_regs_32[regc]);
				assemby = comp_append_str(assemby, addition);
				lit_srt_sec_dec_p = comp_append_str(lit_srt_sec_dec_p, d);
		}
		regc++;
	}

	addition = realloc(addition, 54);
	sprintf(addition,	"\tleaq\tL_.str.%d(%%rip), %%rdi\n"
						"\tcallq\t_printf\n", lit_str_count);
	assemby = comp_append_str(assemby, addition);

	lit_srt_sec_dec_p = comp_append_str(lit_srt_sec_dec_p, "\\n\"\n");
	*lit_str_section = comp_append_str(*lit_str_section, lit_srt_sec_dec_p);

	lit_str_count++;
	free(addition);
	return assemby;
}

char *comp_imediate_sec_str() {

	char *imediate_decs = malloc(25);
	sprintf(imediate_decs, ".section\t__TEXT,__const\n");

	int i;
	for(i = 0; i<HASH_SIZE; i++) {

		if(!hash_table[i]) continue;

		hash_node_t *node;
		for(node = hash_table[i]; node; node = node->next) {
			if(node->isVariableOrFuncionDeclared == 0 && node->nature != NATURE_TEMPORARY && node->type != SYMBOL_LIT_STRING) {
				char *addition = (char *)malloc(+1 +2*strlen(node->text) +10);
				sprintf(addition, "_%s: .long\t%s\n", node->text, node->text);
				imediate_decs = comp_append_str(imediate_decs, addition);
				free(addition);
				break;
			}
		}
	}
	
	return imediate_decs;
}

char *comp_variable_sec_str() {

	char *variable_decs = malloc(24);
	sprintf(variable_decs, ".section\t__DATA,__data\n");

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
	for(func_dec = arg; func_dec && !(func_dec->type == TAC_BEGINFUN && strcmp(func_call->op1->text, func_dec->res->text) == 0); func_dec = func_dec->next);
	
	if(!func_dec)
		for(func_dec = arg; func_dec && !(func_dec->type == TAC_BEGINFUN && strcmp(func_call->op1->text, func_dec->res->text) == 0); func_dec = func_dec->prev);

	tac_t *param = func_dec;
	int j;
	for(j = 0; j<=i; j++) {
		param = param->prev;
	}
	return param;
}

char *comp_build_array_asm(tac_t *arr) {

	char* str = malloc(+1 +strlen(arr->res->text) +3);
	sprintf(str, "_%s:\n", arr->res->text);

	int i;
	tac_t *tac = arr->next;
	for(i = 0; i < atoi(arr->op1->text); i++) {
		char* tail = malloc(+1 +strlen(tac->res->text) +9);
		sprintf(tail, "\t.long\t%s\n", tac->res->text);
		str = comp_append_str(str, tail);
		free(tail);
		tac = tac->next;
	}

	return str;
}

int comp_asm_generate(tac_t *head, char *output) {

	FILE *fout = fopen(output, "w");
	int r = 0;

	char *variable_decs		= comp_variable_sec_str();
	char *imediate_decs		= comp_imediate_sec_str();
	char *lit_str_section	= comp_literal_str_sec_str();
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
			case TAC_ARR:
				addition = comp_build_array_asm(tac);
				variable_decs = comp_append_str(variable_decs, addition);
				free(addition);
				break;
			case TAC_MOVE: fprintf(fout,	"\t# TAC_MOVE\n"
											"\tmovl\t_%s(%%rip), %%eax\n"
											"\tmovl\t%%eax, _%s(%%rip)\n",
											tac->op1->text, tac->res->text); break;
			case TAC_INC: fprintf(fout,	"\t# TAC_INC\n"
										"\tincl\t_%s(%%rip)\n", tac->res->text); break;
			case TAC_ADD: fprintf(fout, "\t# TAC_ADD\n"
										"\tmovl\t_%s(%%rip), %%eax\n"
										"\taddl\t_%s(%%rip), %%eax\n"
										"\tmovl\t%%eax, _%s(%%rip)\n",
										tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_SUB: fprintf(fout, "\t# TAC_SUB\n"
										"\tmovl\t_%s(%%rip), %%eax\n"
										"\tsubl\t_%s(%%rip), %%eax\n"
										"\tmovl\t%%eax, _%s(%%rip)\n",
										tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_MUL: fprintf(fout, "\t# TAC_MUL\n"
										"\tmovl\t_%s(%%rip), %%eax\n"
										"\timull\t_%s(%%rip), %%eax\n"
										"\tmovl\t%%eax, _%s(%%rip)\n",
										tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_DIV: fprintf(fout, "\t# TAC_DIV\n"
										"\tmovl\t_%s(%%rip), %%eax\n"
										"\tcltd\n"
										"\tidivl\t_%s(%%rip)\n"
										"\tmovl\t%%eax, _%s(%%rip)\n",
										tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_BLE: fprintf(fout, "\t# TAC_BLE\n"
										"\tmovl\t_%s(%%rip), %%edx\n"
										"\tcmpl\t_%s(%%rip), %%edx\n"
										"\tjle\t\t_%s\n", tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_BGE: fprintf(fout, "\t# TAC_BGE\n"
										"\tmovl\t_%s(%%rip), %%edx\n"
										"\tcmpl\t_%s(%%rip), %%edx\n"
										"\tjge\t\t_%s\n", tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_BEQ: fprintf(fout, "\t# TAC_BEQ\n"
										"\tmovl\t_%s(%%rip), %%edx\n"
										"\tcmpl\t_%s(%%rip), %%edx\n"
										"\tje\t\t_%s\n", tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_BNE: fprintf(fout, "\t# TAC_BNE\n"
										"\tmovl\t_%s(%%rip), %%edx\n"
										"\tcmpl\t_%s(%%rip), %%edx\n"
										"\tjne\t\t_%s\n", tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_AND: fprintf(fout,	"\t# TAC_AND\n"
										"\tmovl\t_%s(%%rip), %%eax\n"
										"\tmovl\t_%s(%%rip), %%edx\n"
										"\tandl\t%%edx, %%eax\n"
										"\tmovl\t%%eax, _%s(%%rip)", tac->op1->text,
										tac->op2->text, tac->res->text); break;
			case TAC_OR : fprintf(fout,	"\t# TAC_OR\n"
										"\tmovl\t_%s(%%rip), %%eax\n"
										"\tmovl\t_%s(%%rip), %%edx\n"
										"\torl\t\t %%edx, %%eax\n"
										"\tmovl\t%%eax, _%s(%%rip)\n", tac->op1->text,
										tac->op2->text, tac->res->text); break;
			case TAC_BLT: fprintf(fout, "\t# TAC_BLT\n"
										"\tmovl\t_%s(%%rip), %%edx\n"
										"\tcmpl\t_%s(%%rip), %%edx\n"
										"\tjl\t\t_%s\n", tac->op1->text, tac->op2->text,
										tac->res->text); break;
			case TAC_BGT: fprintf(fout, "\t# TAC_BGT\n"
										"\tmovl\t_%s(%%rip), %%edx\n"
										"\tcmpl\t_%s(%%rip), %%edx\n"
										"\tjg\t\t_%s\n", tac->op1->text, tac->op2->text,
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
			case TAC_ACALL: fprintf(fout,	"\t# TAC_ACALL\n"
											"\tleaq\t_%s(%%rip), %%rcx\n"
											"\tmovslq\t_%s(%%rip), %%rdx\n"
											"\tmovl\t(%%rcx,%%rdx,4), %%esi\n"
											"\tmovl\t%%esi, _%s(%%rip)\n", tac->op1->text,
											tac->op2->text, tac->res->text); break;
			case TAC_AATTRIB: fprintf(fout,	"\t# TAC_AATTRIB\n"
											"\tleaq\t_%s(%%rip), %%rcx\n"
											"\tmovl\t_%s(%%rip), %%edx\n"
											"\tmovslq\t_%s(%%rip), %%rsi\n"
											"\tmovl\t%%edx, (%%rcx,%%rsi,4)\n", tac->res->text,
											tac->op2->text, tac->op1->text); break;
			case TAC_IFZ: fprintf(fout,	"\t# TAC_IFZ\n"
										"\tcmpl\t$0, _%s(%%rip)\n"
										"\tje\t\t_%s\n", tac->op1->text, tac->res->text); break;
			case TAC_JUMP: fprintf(fout,	"\t# TAC_JUMP\n"
											"\tjmp\t\t_%s\n",
											tac->res->text); break;
			case TAC_ARG: fprintf(fout,	"\t# TAC_ARG\n"
										"\tmovl\t_%s(%%rip), %%eax\n"
										"\tmovl\t%%eax, _%s(%%rip)\n", tac->res->text,
										comp_fill_parameter(tac)->res->text); break;
			case TAC_RET: fprintf(fout,	"\t# TAC_RET\n"
										"\tmovl\t_%s(%%rip), %%eax\n"
										"\tpopq\t%%rbp\n"
										"\tretq\n", tac->res->text); break;
			case TAC_PRINT: fprintf(fout, 	"\t# TAC_PRINT\n"
											"%s", com_generate_print(tac, &imediate_decs, &lit_str_section)); break;
			case TAC_PRARG: break;
			case TAC_READ: fprintf(fout,	"\t# TAC_READ\n"
											"\tleaq\tL_.str.1(%%rip), %%rdi\n"
											"\tleaq\t_%s(%%rip), %%rsi\n"
											"\tcallq\t_scanf\n", tac->res->text); break;
			default: r++; fprintf(stderr, "Compiler error: unknown intermediary code.\n"); break;
		}
	}
	
	fprintf(fout, "\n%s\n%s\n%s\n", variable_decs, imediate_decs, lit_str_section);
	fclose(fout);
	return r;
}