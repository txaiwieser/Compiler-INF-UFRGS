	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.globl	_main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
	movq	%rsp, %rbp

	leaq	_array(%rip), %rcx
	movl	_new(%rip), %edx
	movslq	_i(%rip), %rsi
	movl	%edx, (%rcx,%rsi,4)

	movl	(%rcx,%rsi,4), %esi # print input
	# print call (entrada %esi) {
	leaq	L_.str(%rip), %rdi
	callq	_printf
	# }


	popq	%rbp
	retq
	.cfi_endproc

	.section	__DATA,__data
	.globl	_new                    ## @new
	.p2align	2
_new:
	.long	100                     ## 0x64

	.globl	_array                  ## @array
	.p2align	4
_array:
	.long	1                       ## 0x1
	.long	2                       ## 0x2
	.long	3                       ## 0x3
	.long	4                       ## 0x4

	.globl	_i                      ## @i
	.p2align	2
_i:
	.long	2                       ## 0x2

.section	__TEXT,__cstring,cstring_literals
L_.str:	.asciz	"%d\n"
.subsections_via_symbols
