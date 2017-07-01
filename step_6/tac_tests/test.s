	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.globl	_main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Ltmp0:
	.cfi_def_cfa_offset 16
Ltmp1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Ltmp2:
	.cfi_def_cfa_register %rbp
	movl	$0, -4(%rbp)
	movl	_a(%rip), %eax
	addl	$222, %eax
	subl	$333, %eax              ## imm = 0x14D
	movl	%eax, _f(%rip)
	movl	_a(%rip), %eax
	popq	%rbp
	retq
	.cfi_endproc

	.section	__DATA,__data
	.globl	_a                      ## @a
	.p2align	2
_a:
	.long	111                     ## 0x6f

	.section	__TEXT,__const
	.globl	_b                      ## @b
	.p2align	2
_b:
	.long	222                     ## 0xde

	.globl	_c                      ## @c
	.p2align	2
_c:
	.long	333                     ## 0x14d

	.section	__DATA,__data
	.globl	_d                      ## @d
	.p2align	2
_d:
	.long	1                       ## 0x1

	.section	__TEXT,__const
	.globl	_e                      ## @e
	.p2align	2
_e:
	.long	0                       ## 0x0

	.globl	_f                      ## @f
.zerofill __DATA,__common,_f,4,2

.subsections_via_symbols
