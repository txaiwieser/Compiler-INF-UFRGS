# Global Variables
.section	__DATA,__data
_a:	.long	111
_r:	.long	222

# Imediate literals / Constant Data:
.section	__TEXT,__const
_b: .long	222
_c: .long	333

# Print:
.section	__TEXT,__cstring,cstring_literals
L_.str:	.asciz	"%d\n"

# Code Section:
.section	__TEXT,__text,regular,pure_instructions
.globl	_main
_main:
	# default function beggining {
	.cfi_startproc
	pushq	%rbp
	movq	%rsp, %rbp
	## }

	movl	_b(%rip), %esi # print input

	# print call (entrada %esi) {
	leaq	L_.str(%rip), %rdi
	callq	_printf
	# }

	# default return code {		
	movl	_r(%rip), %eax			# move return value to %eax
	popq	%rbp					# popq %rbp (stack base pointer)
	retq							# call retq
	# }

	.cfi_endproc

_func:
	# default function beggining {
	.cfi_startproc
	pushq	%rbp
	movq	%rsp, %rbp
	# }

	movl	_a(%rip), %eax
	popq	%rbp
	retq
	.cfi_endproc