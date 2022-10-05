	.file	"simple2.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC6:
	.string	"Time in seconds = %f\n"
.LC7:
	.string	"c = %f\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB39:
	.cfi_startproc
	endbr64
	pushq	%r12
	.cfi_def_cfa_offset 16
	.cfi_offset 12, -16
	movdqa	.LC0(%rip), %xmm2
	pushq	%rbp
	.cfi_def_cfa_offset 24
	.cfi_offset 6, -24
	movdqa	.LC2(%rip), %xmm4
	leaq	b(%rip), %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset 3, -32
	movdqa	.LC3(%rip), %xmm3
	leaq	a(%rip), %rbx
	movq	%rbp, %rax
	movq	%rbx, %rdx
	leaq	16384(%rbp), %rcx
.L2:
	movdqa	%xmm2, %xmm0
	addq	$32, %rax
	paddd	%xmm4, %xmm2
	addq	$32, %rdx
	cvtdq2pd	%xmm0, %xmm1
	movaps	%xmm1, -32(%rax)
	pshufd	$238, %xmm0, %xmm1
	paddd	%xmm3, %xmm0
	cvtdq2pd	%xmm1, %xmm1
	movaps	%xmm1, -16(%rax)
	cvtdq2pd	%xmm0, %xmm1
	pshufd	$238, %xmm0, %xmm0
	cvtdq2pd	%xmm0, %xmm0
	movaps	%xmm1, -32(%rdx)
	movaps	%xmm0, -16(%rdx)
	cmpq	%rcx, %rax
	jne	.L2
	call	clock@PLT
	movl	$100000, %edx
	pxor	%xmm1, %xmm1
	movapd	.LC4(%rip), %xmm3
	movq	%rax, %r12
.L3:
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L4:
	movapd	(%rbx,%rax), %xmm0
	mulpd	%xmm3, %xmm0
	addpd	0(%rbp,%rax), %xmm0
	addq	$16, %rax
	addsd	%xmm0, %xmm1
	unpckhpd	%xmm0, %xmm0
	addsd	%xmm0, %xmm1
	cmpq	$16384, %rax
	jne	.L4
	subl	$1, %edx
	jne	.L3
	movsd	%xmm1, c(%rip)
	call	clock@PLT
	pxor	%xmm0, %xmm0
	movl	$1, %edi
	leaq	.LC6(%rip), %rsi
	subq	%r12, %rax
	cvtsi2sdq	%rax, %xmm0
	movl	$1, %eax
	divsd	.LC5(%rip), %xmm0
	call	__printf_chk@PLT
	movl	$1, %edi
	movl	$1, %eax
	movsd	c(%rip), %xmm0
	leaq	.LC7(%rip), %rsi
	call	__printf_chk@PLT
	popq	%rbx
	.cfi_def_cfa_offset 24
	xorl	%eax, %eax
	popq	%rbp
	.cfi_def_cfa_offset 16
	popq	%r12
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE39:
	.size	main, .-main
	.local	c
	.comm	c,8,8
	.local	b
	.comm	b,16384,32
	.local	a
	.comm	a,16384,32
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.long	0
	.long	1
	.long	2
	.long	3
	.align 16
.LC2:
	.long	4
	.long	4
	.long	4
	.long	4
	.align 16
.LC3:
	.long	1
	.long	1
	.long	1
	.long	1
	.align 16
.LC4:
	.long	-611603343
	.long	1072693352
	.long	-611603343
	.long	1072693352
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC5:
	.long	0
	.long	1093567616
	.ident	"GCC: (Ubuntu 11.2.0-19ubuntu1) 11.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
