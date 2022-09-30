	.file	"stdio.c"
	.text
.Ltext0:
	.p2align 4
	.globl	read_string
	.type	read_string, @function
read_string:
.LFB0:
	.file 1 "src/stdio.c"
	.loc 1 9 1
	.cfi_startproc
	subq	$56, %rsp
	.cfi_def_cfa_offset 64
	movq	%rdi, 24(%rsp)
	movl	%esi, 20(%rsp)
	movq	%rdx, 8(%rsp)
	.loc 1 10 18
	movq	8(%rsp), %rax
	movq	%rax, %rdi
	movabsq	$strlen, %rax
	call	*%rax
.LVL0:
	.loc 1 10 12
	cltq
	movq	%rax, 32(%rsp)
.LBB2:
	.loc 1 11 14
	movl	$0, 44(%rsp)
	.loc 1 11 5
	jmp	.L2
.L5:
	.loc 1 13 33
	movl	44(%rsp), %eax
	movslq	%eax, %rdx
	movq	8(%rsp), %rax
	leaq	(%rdx,%rax), %rcx
	.loc 1 13 24
	movl	20(%rsp), %eax
	leal	1(%rax), %edx
	movl	%edx, 20(%rsp)
	movslq	%eax, %rdx
	.loc 1 13 15
	movq	24(%rsp), %rax
	addq	%rax, %rdx
	.loc 1 13 33
	movzbl	(%rcx), %eax
	.loc 1 13 28
	movb	%al, (%rdx)
	.loc 1 14 16
	movl	44(%rsp), %eax
	movslq	%eax, %rdx
	movq	8(%rsp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	.loc 1 14 12
	cmpb	$10, %al
	je	.L3
	.loc 1 14 28 discriminator 1
	cmpl	$1023, 20(%rsp)
	jle	.L4
.L3:
	.loc 1 17 13
	movl	20(%rsp), %ecx
	movq	24(%rsp), %rax
	movl	$15, %edx
	movl	%ecx, %esi
	movq	%rax, %rdi
	movabsq	$sys_print, %rax
	call	*%rax
.LVL1:
	.loc 1 18 22
	movl	$0, 20(%rsp)
.L4:
	.loc 1 11 30 discriminator 2
	addl	$1, 44(%rsp)
.L2:
	.loc 1 11 23 discriminator 1
	movl	44(%rsp), %eax
	cltq
	.loc 1 11 5 discriminator 1
	cmpq	%rax, 32(%rsp)
	ja	.L5
.LBE2:
	.loc 1 22 12
	movl	20(%rsp), %eax
	.loc 1 23 1
	addq	$56, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE0:
	.size	read_string, .-read_string
	.p2align 4
	.globl	hex_to_string
	.type	hex_to_string, @function
hex_to_string:
.LFB1:
	.loc 1 26 1
	.cfi_startproc
	subq	$88, %rsp
	.cfi_def_cfa_offset 96
	movq	%rdi, 24(%rsp)
	movl	%esi, 20(%rsp)
	movq	%rdx, 8(%rsp)
	.loc 1 29 5
	leaq	32(%rsp), %rax
	movl	$21, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	movabsq	$memset, %rax
	call	*%rax
.LVL2:
	.loc 1 30 10
	movb	$1, 79(%rsp)
	.loc 1 31 9
	movl	$0, 72(%rsp)
	.loc 1 33 8
	cmpq	$0, 8(%rsp)
	js	.L8
	.loc 1 35 18
	movb	$0, 79(%rsp)
.L8:
	.loc 1 37 14
	movl	72(%rsp), %eax
	leal	1(%rax), %edx
	movl	%edx, 72(%rsp)
	.loc 1 37 18
	cltq
	movb	$72, 32(%rsp,%rax)
.L9:
	.loc 1 40 30 discriminator 1
	movq	8(%rsp), %rax
	cqto
	shrq	$60, %rdx
	addq	%rdx, %rax
	andl	$15, %eax
	subq	%rdx, %rax
	movq	%rax, %rcx
	.loc 1 40 18 discriminator 1
	movl	72(%rsp), %eax
	leal	1(%rax), %edx
	movl	%edx, 72(%rsp)
	.loc 1 40 27 discriminator 1
	movabsq	$hex.0, %rdx
	movzbl	(%rdx,%rcx), %edx
	.loc 1 40 22 discriminator 1
	cltq
	movb	%dl, 32(%rsp,%rax)
	.loc 1 41 11 discriminator 1
	movq	8(%rsp), %rax
	leaq	15(%rax), %rdx
	testq	%rax, %rax
	cmovs	%rdx, %rax
	sarq	$4, %rax
	movq	%rax, 8(%rsp)
	.loc 1 42 5 discriminator 1
	cmpq	$0, 8(%rsp)
	jne	.L9
	.loc 1 44 8
	cmpb	$0, 79(%rsp)
	je	.L10
	.loc 1 45 18
	movl	72(%rsp), %eax
	leal	1(%rax), %edx
	movl	%edx, 72(%rsp)
	.loc 1 45 22
	cltq
	movb	$45, 32(%rsp,%rax)
.L10:
.LBB3:
	.loc 1 47 14
	movl	72(%rsp), %eax
	subl	$1, %eax
	movl	%eax, 68(%rsp)
	.loc 1 47 5
	jmp	.L11
.L12:
	.loc 1 48 24 discriminator 3
	movl	20(%rsp), %eax
	leal	1(%rax), %edx
	movl	%edx, 20(%rsp)
	movslq	%eax, %rdx
	.loc 1 48 15 discriminator 3
	movq	24(%rsp), %rax
	addq	%rax, %rdx
	.loc 1 48 34 discriminator 3
	movl	68(%rsp), %eax
	cltq
	movzbl	32(%rsp,%rax), %eax
	.loc 1 48 28 discriminator 3
	movb	%al, (%rdx)
	.loc 1 47 37 discriminator 3
	subl	$1, 68(%rsp)
.L11:
	.loc 1 47 5 discriminator 1
	cmpl	$0, 68(%rsp)
	jns	.L12
.LBE3:
	.loc 1 50 12
	movl	72(%rsp), %eax
	.loc 1 51 1
	addq	$88, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE1:
	.size	hex_to_string, .-hex_to_string
	.p2align 4
	.globl	decimal_to_string
	.type	decimal_to_string, @function
decimal_to_string:
.LFB2:
	.loc 1 54 1
	.cfi_startproc
	subq	$72, %rsp
	.cfi_def_cfa_offset 80
	movq	%rdi, 24(%rsp)
	movl	%esi, 20(%rsp)
	movq	%rdx, 8(%rsp)
	.loc 1 56 15
	leaq	32(%rsp), %rdx
	movq	8(%rsp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	movabsq	$itoa, %rax
	call	*%rax
.LVL3:
	movq	%rax, 56(%rsp)
	.loc 1 57 9
	movl	$0, 52(%rsp)
	.loc 1 59 5
	jmp	.L15
.L16:
	.loc 1 61 24 discriminator 2
	movl	20(%rsp), %eax
	leal	1(%rax), %edx
	movl	%edx, 20(%rsp)
	movslq	%eax, %rdx
	.loc 1 61 15 discriminator 2
	movq	24(%rsp), %rax
	addq	%rax, %rdx
	.loc 1 61 30 discriminator 2
	movq	56(%rsp), %rax
	movzbl	(%rax), %eax
	.loc 1 61 28 discriminator 2
	movb	%al, (%rdx)
	.loc 1 62 13 discriminator 2
	addl	$1, 52(%rsp)
	.loc 1 59 17 discriminator 2
	addq	$1, 56(%rsp)
.L15:
	.loc 1 59 12 discriminator 1
	movq	56(%rsp), %rax
	movzbl	(%rax), %eax
	.loc 1 59 5 discriminator 1
	testb	%al, %al
	jne	.L16
	.loc 1 64 12
	movl	52(%rsp), %eax
	.loc 1 65 1
	addq	$72, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE2:
	.size	decimal_to_string, .-decimal_to_string
	.p2align 4
	.globl	printf
	.type	printf, @function
printf:
.LFB3:
	.loc 1 68 1
	.cfi_startproc
	subq	$1288, %rsp
	.cfi_def_cfa_offset 1296
	movq	%rdi, 8(%rsp)
	movq	%rsi, 1112(%rsp)
	movq	%rdx, 1120(%rsp)
	movq	%rcx, 1128(%rsp)
	movq	%r8, 1136(%rsp)
	movq	%r9, 1144(%rsp)
	testb	%al, %al
	je	.L35
	movaps	%xmm0, 1152(%rsp)
	movaps	%xmm1, 1168(%rsp)
	movaps	%xmm2, 1184(%rsp)
	movaps	%xmm3, 1200(%rsp)
	movaps	%xmm4, 1216(%rsp)
	movaps	%xmm5, 1232(%rsp)
	movaps	%xmm6, 1248(%rsp)
	movaps	%xmm7, 1264(%rsp)
.L35:
	.loc 1 71 9
	movl	$0, 1100(%rsp)
	.loc 1 72 13
	movq	$0, 1080(%rsp)
	.loc 1 73 11
	movq	$0, 1072(%rsp)
	.loc 1 74 17
	movq	$0, 1088(%rsp)
	.loc 1 75 4
	movl	$8, 1048(%rsp)
	movl	$48, 1052(%rsp)
	leaq	1296(%rsp), %rax
	movq	%rax, 1056(%rsp)
	leaq	1104(%rsp), %rax
	movq	%rax, 1064(%rsp)
	.loc 1 76 5
	leaq	16(%rsp), %rax
	movl	$1024, %edx
	movl	$0, %esi
	movq	%rax, %rdi
	movabsq	$memset, %rax
	call	*%rax
.LVL4:
	.loc 1 78 12
	movq	8(%rsp), %rax
	movq	%rax, 1088(%rsp)
	.loc 1 78 5
	jmp	.L20
.L33:
	.loc 1 80 13
	movq	1088(%rsp), %rax
	movzbl	(%rax), %eax
	.loc 1 80 12
	cmpb	$37, %al
	je	.L21
	.loc 1 82 31
	movl	1100(%rsp), %eax
	leal	1(%rax), %edx
	movl	%edx, 1100(%rsp)
	.loc 1 82 37
	movq	1088(%rsp), %rdx
	movzbl	(%rdx), %edx
	.loc 1 82 35
	cltq
	movb	%dl, 16(%rsp,%rax)
	.loc 1 84 13
	jmp	.L22
.L21:
	.loc 1 86 9
	addq	$1, 1088(%rsp)
	.loc 1 86 17
	movq	1088(%rsp), %rax
	movzbl	(%rax), %eax
	movsbl	%al, %eax
	.loc 1 86 9
	cmpl	$120, %eax
	je	.L23
	cmpl	$120, %eax
	jg	.L24
	cmpl	$100, %eax
	je	.L25
	cmpl	$115, %eax
	je	.L26
	jmp	.L24
.L25:
	.loc 1 89 22
	movl	1048(%rsp), %eax
	cmpl	$47, %eax
	ja	.L27
	movq	1064(%rsp), %rax
	movl	1048(%rsp), %edx
	movl	%edx, %edx
	addq	%rdx, %rax
	movl	1048(%rsp), %edx
	addl	$8, %edx
	movl	%edx, 1048(%rsp)
	jmp	.L28
.L27:
	movq	1056(%rsp), %rax
	leaq	8(%rax), %rdx
	movq	%rdx, 1056(%rsp)
.L28:
	movq	(%rax), %rax
	movq	%rax, 1080(%rsp)
	.loc 1 90 28
	movq	1080(%rsp), %rdx
	movl	1100(%rsp), %ecx
	leaq	16(%rsp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	movabsq	$decimal_to_string, %rax
	call	*%rax
.LVL5:
	.loc 1 90 25
	addl	%eax, 1100(%rsp)
	.loc 1 91 13
	jmp	.L22
.L23:
	.loc 1 93 22
	movl	1048(%rsp), %eax
	cmpl	$47, %eax
	ja	.L29
	movq	1064(%rsp), %rax
	movl	1048(%rsp), %edx
	movl	%edx, %edx
	addq	%rdx, %rax
	movl	1048(%rsp), %edx
	addl	$8, %edx
	movl	%edx, 1048(%rsp)
	jmp	.L30
.L29:
	movq	1056(%rsp), %rax
	leaq	8(%rax), %rdx
	movq	%rdx, 1056(%rsp)
.L30:
	movq	(%rax), %rax
	movq	%rax, 1080(%rsp)
	.loc 1 94 28
	movq	1080(%rsp), %rdx
	movl	1100(%rsp), %ecx
	leaq	16(%rsp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	movabsq	$hex_to_string, %rax
	call	*%rax
.LVL6:
	.loc 1 94 25
	addl	%eax, 1100(%rsp)
	.loc 1 95 13
	jmp	.L22
.L26:
	.loc 1 97 20
	movl	1048(%rsp), %eax
	cmpl	$47, %eax
	ja	.L31
	movq	1064(%rsp), %rax
	movl	1048(%rsp), %edx
	movl	%edx, %edx
	addq	%rdx, %rax
	movl	1048(%rsp), %edx
	addl	$8, %edx
	movl	%edx, 1048(%rsp)
	jmp	.L32
.L31:
	movq	1056(%rsp), %rax
	leaq	8(%rax), %rdx
	movq	%rdx, 1056(%rsp)
.L32:
	movq	(%rax), %rax
	movq	%rax, 1072(%rsp)
	.loc 1 98 27
	movq	1072(%rsp), %rdx
	movl	1100(%rsp), %ecx
	leaq	16(%rsp), %rax
	movl	%ecx, %esi
	movq	%rax, %rdi
	movabsq	$read_string, %rax
	call	*%rax
.LVL7:
	movl	%eax, 1100(%rsp)
	.loc 1 99 13
	jmp	.L22
.L24:
	.loc 1 101 31
	movl	1100(%rsp), %eax
	leal	1(%rax), %edx
	movl	%edx, 1100(%rsp)
	.loc 1 101 35
	cltq
	movb	$37, 16(%rsp,%rax)
	.loc 1 102 14
	subq	$1, 1088(%rsp)
	.loc 1 103 13
	nop
.L22:
	.loc 1 78 27 discriminator 2
	addq	$1, 1088(%rsp)
.L20:
	.loc 1 78 22 discriminator 1
	movq	1088(%rsp), %rax
	movzbl	(%rax), %eax
	.loc 1 78 5 discriminator 1
	testb	%al, %al
	jne	.L33
	.loc 1 107 19
	movl	1100(%rsp), %ecx
	leaq	16(%rsp), %rax
	movl	$15, %edx
	movl	%ecx, %esi
	movq	%rax, %rdi
	movabsq	$sys_print, %rax
	call	*%rax
.LVL8:
	movl	%eax, 1100(%rsp)
	.loc 1 109 12
	movl	1100(%rsp), %eax
	.loc 1 110 1
	addq	$1288, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE3:
	.size	printf, .-printf
	.data
	.align 16
	.type	hex.0, @object
	.size	hex.0, 16
hex.0:
	.byte	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43
	.byte	0x44,0x45,0x46
	.text
.Letext0:
	.file 2 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stdint-gcc.h"
	.file 3 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stdarg.h"
	.file 4 "<built-in>"
	.file 5 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stddef.h"
	.file 6 "./src/include/memory.h"
	.file 7 "./src/include/stdio.h"
	.file 8 "./src/include/string.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x42f
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF35
	.byte	0xc
	.long	.LASF36
	.long	.LASF37
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF0
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF1
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x4
	.long	.LASF7
	.byte	0x2
	.byte	0x2b
	.byte	0x12
	.long	0x4e
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF2
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF3
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF4
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF5
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF6
	.uleb128 0x4
	.long	.LASF8
	.byte	0x3
	.byte	0x28
	.byte	0x1b
	.long	0x7d
	.uleb128 0x5
	.long	.LASF38
	.long	0x86
	.uleb128 0x6
	.long	0x96
	.long	0x96
	.uleb128 0x7
	.long	0x6a
	.byte	0
	.byte	0
	.uleb128 0x8
	.long	.LASF39
	.byte	0x18
	.byte	0x4
	.byte	0
	.long	0xd3
	.uleb128 0x9
	.long	.LASF9
	.byte	0x4
	.byte	0
	.long	0x63
	.byte	0
	.uleb128 0x9
	.long	.LASF10
	.byte	0x4
	.byte	0
	.long	0x63
	.byte	0x4
	.uleb128 0x9
	.long	.LASF11
	.byte	0x4
	.byte	0
	.long	0xd3
	.byte	0x8
	.uleb128 0x9
	.long	.LASF12
	.byte	0x4
	.byte	0
	.long	0xd3
	.byte	0x10
	.byte	0
	.uleb128 0xa
	.byte	0x8
	.uleb128 0x4
	.long	.LASF13
	.byte	0x3
	.byte	0x63
	.byte	0x18
	.long	0x71
	.uleb128 0x4
	.long	.LASF14
	.byte	0x5
	.byte	0xd1
	.byte	0x1b
	.long	0x6a
	.uleb128 0xb
	.long	.LASF21
	.byte	0x1
	.byte	0x43
	.byte	0x5
	.long	0x3b
	.quad	.LFB3
	.quad	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.long	0x1c0
	.uleb128 0xc
	.long	.LASF23
	.byte	0x1
	.byte	0x43
	.byte	0x18
	.long	0x1c0
	.uleb128 0x3
	.byte	0x91
	.sleb128 -1288
	.uleb128 0xd
	.uleb128 0xe
	.long	.LASF15
	.byte	0x1
	.byte	0x45
	.byte	0xd
	.long	0xd5
	.uleb128 0x3
	.byte	0x91
	.sleb128 -248
	.uleb128 0xe
	.long	.LASF16
	.byte	0x1
	.byte	0x46
	.byte	0xa
	.long	0x1d2
	.uleb128 0x3
	.byte	0x91
	.sleb128 -1280
	.uleb128 0xe
	.long	.LASF17
	.byte	0x1
	.byte	0x47
	.byte	0x9
	.long	0x3b
	.uleb128 0x3
	.byte	0x91
	.sleb128 -196
	.uleb128 0xe
	.long	.LASF18
	.byte	0x1
	.byte	0x48
	.byte	0xd
	.long	0x42
	.uleb128 0x3
	.byte	0x91
	.sleb128 -216
	.uleb128 0xe
	.long	.LASF19
	.byte	0x1
	.byte	0x49
	.byte	0xb
	.long	0x1e3
	.uleb128 0x3
	.byte	0x91
	.sleb128 -224
	.uleb128 0xf
	.string	"p"
	.byte	0x1
	.byte	0x4a
	.byte	0x11
	.long	0x1c0
	.uleb128 0x3
	.byte	0x91
	.sleb128 -208
	.uleb128 0x10
	.quad	.LVL4
	.long	0x402
	.uleb128 0x10
	.quad	.LVL5
	.long	0x1e9
	.uleb128 0x10
	.quad	.LVL6
	.long	0x282
	.uleb128 0x10
	.quad	.LVL7
	.long	0x366
	.uleb128 0x10
	.quad	.LVL8
	.long	0x40e
	.byte	0
	.uleb128 0x11
	.byte	0x8
	.long	0x1cd
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF20
	.uleb128 0x12
	.long	0x1c6
	.uleb128 0x6
	.long	0x1c6
	.long	0x1e3
	.uleb128 0x13
	.long	0x6a
	.value	0x3ff
	.byte	0
	.uleb128 0x11
	.byte	0x8
	.long	0x1c6
	.uleb128 0xb
	.long	.LASF22
	.byte	0x1
	.byte	0x35
	.byte	0x5
	.long	0x3b
	.quad	.LFB2
	.quad	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.long	0x272
	.uleb128 0xc
	.long	.LASF16
	.byte	0x1
	.byte	0x35
	.byte	0x1d
	.long	0x1e3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0xc
	.long	.LASF24
	.byte	0x1
	.byte	0x35
	.byte	0x29
	.long	0x3b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -60
	.uleb128 0xc
	.long	.LASF25
	.byte	0x1
	.byte	0x35
	.byte	0x3b
	.long	0x42
	.uleb128 0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0xe
	.long	.LASF26
	.byte	0x1
	.byte	0x37
	.byte	0xa
	.long	0x272
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0xf
	.string	"c"
	.byte	0x1
	.byte	0x38
	.byte	0xb
	.long	0x1e3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xe
	.long	.LASF27
	.byte	0x1
	.byte	0x39
	.byte	0x9
	.long	0x3b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.uleb128 0x10
	.quad	.LVL3
	.long	0x41a
	.byte	0
	.uleb128 0x6
	.long	0x1c6
	.long	0x282
	.uleb128 0x7
	.long	0x6a
	.byte	0x13
	.byte	0
	.uleb128 0xb
	.long	.LASF28
	.byte	0x1
	.byte	0x19
	.byte	0x5
	.long	0x3b
	.quad	.LFB1
	.quad	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.long	0x346
	.uleb128 0xc
	.long	.LASF16
	.byte	0x1
	.byte	0x19
	.byte	0x19
	.long	0x1e3
	.uleb128 0x3
	.byte	0x91
	.sleb128 -72
	.uleb128 0xc
	.long	.LASF24
	.byte	0x1
	.byte	0x19
	.byte	0x25
	.long	0x3b
	.uleb128 0x3
	.byte	0x91
	.sleb128 -76
	.uleb128 0x14
	.string	"i"
	.byte	0x1
	.byte	0x19
	.byte	0x37
	.long	0x42
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.uleb128 0xf
	.string	"hex"
	.byte	0x1
	.byte	0x1b
	.byte	0x11
	.long	0x346
	.uleb128 0x9
	.byte	0x3
	.quad	hex.0
	.uleb128 0xe
	.long	.LASF26
	.byte	0x1
	.byte	0x1c
	.byte	0xa
	.long	0x356
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0xe
	.long	.LASF29
	.byte	0x1
	.byte	0x1e
	.byte	0xa
	.long	0x1c6
	.uleb128 0x2
	.byte	0x91
	.sleb128 -17
	.uleb128 0xe
	.long	.LASF27
	.byte	0x1
	.byte	0x1f
	.byte	0x9
	.long	0x3b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x15
	.quad	.LBB3
	.quad	.LBE3-.LBB3
	.long	0x338
	.uleb128 0xf
	.string	"i"
	.byte	0x1
	.byte	0x2f
	.byte	0xe
	.long	0x3b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.uleb128 0x10
	.quad	.LVL2
	.long	0x402
	.byte	0
	.uleb128 0x6
	.long	0x1c6
	.long	0x356
	.uleb128 0x7
	.long	0x6a
	.byte	0xf
	.byte	0
	.uleb128 0x6
	.long	0x1c6
	.long	0x366
	.uleb128 0x7
	.long	0x6a
	.byte	0x14
	.byte	0
	.uleb128 0xb
	.long	.LASF30
	.byte	0x1
	.byte	0x8
	.byte	0x5
	.long	0x3b
	.quad	.LFB0
	.quad	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.long	0x402
	.uleb128 0xc
	.long	.LASF16
	.byte	0x1
	.byte	0x8
	.byte	0x17
	.long	0x1e3
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xc
	.long	.LASF24
	.byte	0x1
	.byte	0x8
	.byte	0x23
	.long	0x3b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x14
	.string	"str"
	.byte	0x1
	.byte	0x8
	.byte	0x39
	.long	0x1c0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.uleb128 0xf
	.string	"len"
	.byte	0x1
	.byte	0xa
	.byte	0xc
	.long	0xe1
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x15
	.quad	.LBB2
	.quad	.LBE2-.LBB2
	.long	0x3f4
	.uleb128 0xf
	.string	"i"
	.byte	0x1
	.byte	0xb
	.byte	0xe
	.long	0x3b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.uleb128 0x10
	.quad	.LVL1
	.long	0x40e
	.byte	0
	.uleb128 0x10
	.quad	.LVL0
	.long	0x426
	.byte	0
	.uleb128 0x16
	.long	.LASF31
	.long	.LASF31
	.byte	0x6
	.byte	0x6
	.byte	0x7
	.uleb128 0x16
	.long	.LASF32
	.long	.LASF32
	.byte	0x7
	.byte	0x7
	.byte	0xc
	.uleb128 0x16
	.long	.LASF33
	.long	.LASF33
	.byte	0x8
	.byte	0x7
	.byte	0x7
	.uleb128 0x16
	.long	.LASF34
	.long	.LASF34
	.byte	0x8
	.byte	0x6
	.byte	0x5
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x5
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x6e
	.uleb128 0xe
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF7:
	.string	"int64_t"
.LASF25:
	.string	"digits"
.LASF14:
	.string	"size_t"
.LASF38:
	.string	"__builtin_va_list"
.LASF17:
	.string	"buffer_size"
.LASF10:
	.string	"fp_offset"
.LASF26:
	.string	"text"
.LASF28:
	.string	"hex_to_string"
.LASF19:
	.string	"string"
.LASF30:
	.string	"read_string"
.LASF34:
	.string	"strlen"
.LASF8:
	.string	"__gnuc_va_list"
.LASF3:
	.string	"unsigned char"
.LASF6:
	.string	"long unsigned int"
.LASF33:
	.string	"itoa"
.LASF4:
	.string	"short unsigned int"
.LASF13:
	.string	"va_list"
.LASF31:
	.string	"memset"
.LASF36:
	.string	"src/stdio.c"
.LASF32:
	.string	"sys_print"
.LASF9:
	.string	"gp_offset"
.LASF23:
	.string	"format"
.LASF5:
	.string	"unsigned int"
.LASF12:
	.string	"reg_save_area"
.LASF20:
	.string	"char"
.LASF37:
	.string	"/home/zbin/spectra/programs/stdlib"
.LASF22:
	.string	"decimal_to_string"
.LASF11:
	.string	"overflow_arg_area"
.LASF35:
	.string	"GNU C99 10.2.0 -mcmodel=large -mtune=generic -march=x86-64 -g -O0 -std=gnu99 -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fomit-frame-pointer -finline-functions -fno-builtin"
.LASF21:
	.string	"printf"
.LASF18:
	.string	"interger"
.LASF1:
	.string	"short int"
.LASF15:
	.string	"args"
.LASF16:
	.string	"buffer"
.LASF24:
	.string	"position"
.LASF2:
	.string	"long int"
.LASF0:
	.string	"signed char"
.LASF39:
	.string	"__va_list_tag"
.LASF27:
	.string	"size"
.LASF29:
	.string	"negative"
	.ident	"GCC: (GNU) 10.2.0"
