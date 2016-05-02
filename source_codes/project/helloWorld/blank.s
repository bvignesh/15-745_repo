	.text
	.file	"blank.bc"
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
# BB#0:                                 # %entry
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	12(%ebp), %eax
	movl	8(%ebp), %ecx
	movl	$0, -4(%ebp)
	movl	%ecx, -8(%ebp)
	movl	%eax, -12(%ebp)
	movl	$0, -16(%ebp)
	movl	-8(%ebp), %eax
	movl	%eax, -16(%ebp)
	movl	-12(%ebp), %eax
	movl	4(%eax), %eax
	movl	%eax, (%esp)
	calll	atoi
	movl	%eax, -16(%ebp)
	movl	%eax, 4(%esp)
	movl	$.L.str, (%esp)
	calll	printf
	movl	$250, %eax
	addl	$24, %esp
	popl	%ebp
	retl
.Ltmp0:
	.size	main, .Ltmp0-main

	.type	.L.str,@object          # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"Beep [%d]\n"
	.size	.L.str, 11


	.ident	"clang version 3.5.0 (branches/release_35 225468)"
	.section	".note.GNU-stack","",@progbits
