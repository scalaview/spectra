	.file	"string.c"
	.text
.Ltext0:
	.p2align 4
	.globl	itoa
	.type	itoa, @function
itoa:
.LFB0:
	.file 1 "src/string.c"
	.loc 1 4 1
	.cfi_startproc
	movq	%rdi, -24(%rsp)
	movq	%rsi, -32(%rsp)
	.loc 1 6 9
	movl	$19, -4(%rsp)
	.loc 1 7 9
	movl	-4(%rsp), %eax
	movslq	%eax, %rdx
	movq	-32(%rsp), %rax
	addq	%rdx, %rax
	.loc 1 7 20
	movb	$0, (%rax)
	.loc 1 8 10
	movb	$1, -5(%rsp)
	.loc 1 9 8
	cmpq	$0, -24(%rsp)
	js	.L3
	.loc 1 11 18
	movb	$0, -5(%rsp)
	.loc 1 12 11
	negq	-24(%rsp)
	.loc 1 14 11
	jmp	.L3
.L4:
	.loc 1 16 37
	movq	-24(%rsp), %rcx
	movabsq	$7378697629483820647, %rdx
	movq	%rcx, %rax
	imulq	%rdx
	movq	%rdx, %rax
	sarq	$2, %rax
	movq	%rcx, %rsi
	sarq	$63, %rsi
	subq	%rsi, %rax
	movq	%rax, %rdx
	movq	%rdx, %rax
	salq	$2, %rax
	addq	%rdx, %rax
	addq	%rax, %rax
	subq	%rax, %rcx
	movq	%rcx, %rdx
	.loc 1 16 32
	movl	$48, %eax
	subl	%edx, %eax
	movl	%eax, %ecx
	.loc 1 16 26
	subl	$1, -4(%rsp)
	.loc 1 16 14
	movl	-4(%rsp), %eax
	movslq	%eax, %rdx
	.loc 1 16 13
	movq	-32(%rsp), %rax
	addq	%rdx, %rax
	.loc 1 16 32
	movl	%ecx, %edx
	.loc 1 16 26
	movb	%dl, (%rax)
	.loc 1 17 11
	movq	-24(%rsp), %rcx
	movabsq	$7378697629483820647, %rdx
	movq	%rcx, %rax
	imulq	%rdx
	movq	%rdx, %rax
	sarq	$2, %rax
	sarq	$63, %rcx
	movq	%rcx, %rdx
	subq	%rdx, %rax
	movq	%rax, -24(%rsp)
.L3:
	.loc 1 14 11
	cmpq	$0, -24(%rsp)
	jne	.L4
	.loc 1 20 8
	cmpl	$19, -4(%rsp)
	jne	.L5
	.loc 1 21 26
	subl	$1, -4(%rsp)
	.loc 1 21 14
	movl	-4(%rsp), %eax
	movslq	%eax, %rdx
	.loc 1 21 13
	movq	-32(%rsp), %rax
	addq	%rdx, %rax
	.loc 1 21 26
	movb	$48, (%rax)
.L5:
	.loc 1 23 8
	cmpb	$0, -5(%rsp)
	je	.L6
	.loc 1 24 26
	subl	$1, -4(%rsp)
	.loc 1 24 14
	movl	-4(%rsp), %eax
	movslq	%eax, %rdx
	.loc 1 24 13
	movq	-32(%rsp), %rax
	addq	%rdx, %rax
	.loc 1 24 26
	movb	$45, (%rax)
.L6:
	.loc 1 26 12
	movl	-4(%rsp), %eax
	movslq	%eax, %rdx
	movq	-32(%rsp), %rax
	addq	%rdx, %rax
	.loc 1 27 1
	ret
	.cfi_endproc
.LFE0:
	.size	itoa, .-itoa
	.p2align 4
	.globl	strlen
	.type	strlen, @function
strlen:
.LFB1:
	.loc 1 30 1
	.cfi_startproc
	movq	%rdi, -24(%rsp)
	.loc 1 31 9
	movl	$0, -4(%rsp)
	.loc 1 32 11
	jmp	.L9
.L10:
	.loc 1 34 12
	addl	$1, -4(%rsp)
	.loc 1 35 13
	addq	$1, -24(%rsp)
.L9:
	.loc 1 32 12
	movq	-24(%rsp), %rax
	movzbl	(%rax), %eax
	.loc 1 32 11
	testb	%al, %al
	jne	.L10
	.loc 1 38 12
	movl	-4(%rsp), %eax
	.loc 1 39 1
	ret
	.cfi_endproc
.LFE1:
	.size	strlen, .-strlen
.Letext0:
	.file 2 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stdint-gcc.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x124
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF13
	.byte	0xc
	.long	.LASF14
	.long	.LASF15
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
	.long	.LASF16
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
	.uleb128 0x5
	.long	.LASF8
	.byte	0x1
	.byte	0x1d
	.byte	0x5
	.long	0x3b
	.quad	.LFB1
	.quad	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.long	0xb2
	.uleb128 0x6
	.string	"ptr"
	.byte	0x1
	.byte	0x1d
	.byte	0x18
	.long	0xb2
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x7
	.string	"len"
	.byte	0x1
	.byte	0x1f
	.byte	0x9
	.long	0x3b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.long	0xbf
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF7
	.uleb128 0x9
	.long	0xb8
	.uleb128 0x5
	.long	.LASF9
	.byte	0x1
	.byte	0x3
	.byte	0x7
	.long	0x121
	.quad	.LFB0
	.quad	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.long	0x121
	.uleb128 0x6
	.string	"i"
	.byte	0x1
	.byte	0x3
	.byte	0x14
	.long	0x42
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0xa
	.long	.LASF10
	.byte	0x1
	.byte	0x3
	.byte	0x1d
	.long	0x121
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0xb
	.long	.LASF11
	.byte	0x1
	.byte	0x6
	.byte	0x9
	.long	0x3b
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.uleb128 0xb
	.long	.LASF12
	.byte	0x1
	.byte	0x8
	.byte	0xa
	.long	0xb8
	.uleb128 0x2
	.byte	0x91
	.sleb128 -13
	.byte	0
	.uleb128 0x8
	.byte	0x8
	.long	0xb8
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
	.uleb128 0x6
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
	.uleb128 0x7
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
	.uleb128 0x8
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
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
	.uleb128 0xb
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
.LASF16:
	.string	"int64_t"
.LASF10:
	.string	"text"
.LASF8:
	.string	"strlen"
.LASF9:
	.string	"itoa"
.LASF3:
	.string	"unsigned char"
.LASF6:
	.string	"long unsigned int"
.LASF4:
	.string	"short unsigned int"
.LASF11:
	.string	"position"
.LASF5:
	.string	"unsigned int"
.LASF7:
	.string	"char"
.LASF15:
	.string	"/home/zbin/spectra/programs/stdlib"
.LASF13:
	.string	"GNU C99 10.2.0 -mcmodel=large -mtune=generic -march=x86-64 -g -O0 -std=gnu99 -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fomit-frame-pointer -finline-functions -fno-builtin"
.LASF1:
	.string	"short int"
.LASF14:
	.string	"src/string.c"
.LASF2:
	.string	"long int"
.LASF0:
	.string	"signed char"
.LASF12:
	.string	"negative"
	.ident	"GCC: (GNU) 10.2.0"
