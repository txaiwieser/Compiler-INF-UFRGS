	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.globl	_main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
	movq	%rsp, %rbp

# SCANF 
	leaq	L_.str(%rip), %rdi
	leaq	_a(%rip), %rsi
	callq	_scanf

# PRINT
	leaq	L_.str.1(%rip), %rdi
	movl	_a(%rip), %esi
	callq	_printf

# RETURN
	movl	%esi, %eax
	popq	%rbp
	retq
	.cfi_endproc

	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%d"

	.comm	_a,4,2                  ## @a
L_.str.1:                               ## @.str.1
	.asciz	"%d\n"


# .subsections_via_symbols

# section .text
#   global main
#   extern printf
#   extern scanf

# section .data
#   message: db "The result is = %d", 10, 0
#   request: db "Enter the number: ", 0
#   integer1: times 4 db 0 # 32-bits integer = 4 bytes
#   formatin: db "%d", 0

# main:
#   #  Ask for an integer
#   push request
#   call printf
#   add esp, 4    # remove the parameter

#   push integer1 # address of integer1, where the input is going to be stored (second parameter)
#   push formatin # arguments are right to left (first  parameter)
#   call scanf
#   add esp, 8    # remove the parameters

#   # Move the value under the address integer1 to EAX
#   mov eax, [integer1]

#   # Print out the content of eax register
#   push eax
#   push message
#   call printf
#   add esp, 8

#   #  Linux terminate the app
#   MOV AL, 1
#   MOV EBX, 0 
#   INT 80h 