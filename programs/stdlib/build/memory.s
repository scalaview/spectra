	.file	"memory.c"
	.text
.Ltext0:
	.p2align 4
	.globl	memset
	.type	memset, @function
memset:
.LFB0:
	.file 1 "src/memory.c"
	.loc 1 4 1
	.cfi_startproc
	movq	%rdi, -24(%rsp)
	movl	%esi, -28(%rsp)
	movq	%rdx, -40(%rsp)
	.loc 1 5 11
	movq	-24(%rsp), %rax
	movq	%rax, -16(%rsp)
.LBB2:
	.loc 1 6 14
	movl	$0, -4(%rsp)
	.loc 1 6 5
	jmp	.L2
.L3:
	.loc 1 8 15 discriminator 3
	movl	-4(%rsp), %eax
	movslq	%eax, %rdx
	movq	-16(%rsp), %rax
	addq	%rdx, %rax
	.loc 1 8 21 discriminator 3
	movl	-28(%rsp), %edx
	.loc 1 8 19 discriminator 3
	movb	%dl, (%rax)
	.loc 1 6 31 discriminator 3
	addl	$1, -4(%rsp)
.L2:
	.loc 1 6 22 discriminator 1
	movl	-4(%rsp), %eax
	cltq
	.loc 1 6 5 discriminator 1
	cmpq	%rax, -40(%rsp)
	ja	.L3
.LBE2:
	.loc 1 10 12
	movq	-24(%rsp), %rax
	.loc 1 11 1
	ret
	.cfi_endproc
.LFE0:
	.size	memset, .-memset
	.p2align 4
	.globl	memcpy
	.type	memcpy, @function
memcpy:
.LFB1:
	.loc 1 14 1
	.cfi_startproc
	movq	%rdi, -24(%rsp)
	movq	%rsi, -32(%rsp)
	movl	%edx, -36(%rsp)
	.loc 1 15 11
	movq	-24(%rsp), %rax
	movq	%rax, -8(%rsp)
	.loc 1 16 11
	movq	-32(%rsp), %rax
	movq	%rax, -16(%rsp)
	.loc 1 18 11
	jmp	.L6
.L7:
	.loc 1 20 18
	movq	-16(%rsp), %rdx
	leaq	1(%rdx), %rax
	movq	%rax, -16(%rsp)
	.loc 1 20 11
	movq	-8(%rsp), %rax
	leaq	1(%rax), %rcx
	movq	%rcx, -8(%rsp)
	.loc 1 20 16
	movzbl	(%rdx), %edx
	.loc 1 20 14
	movb	%dl, (%rax)
.L6:
	.loc 1 18 15
	movl	-36(%rsp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -36(%rsp)
	.loc 1 18 11
	testl	%eax, %eax
	jne	.L7
	.loc 1 22 12
	movq	-24(%rsp), %rax
	.loc 1 23 1
	ret
	.cfi_endproc
.LFE1:
	.size	memcpy, .-memcpy
	.p2align 4
	.globl	memcmp
	.type	memcmp, @function
memcmp:
.LFB2:
	.loc 1 26 1
	.cfi_startproc
	movq	%rdi, -24(%rsp)
	movq	%rsi, -32(%rsp)
	movl	%edx, -36(%rsp)
	.loc 1 27 11
	movq	-24(%rsp), %rax
	movq	%rax, -8(%rsp)
	.loc 1 28 11
	movq	-32(%rsp), %rax
	movq	%rax, -16(%rsp)
	.loc 1 29 11
	jmp	.L10
.L14:
	.loc 1 31 16
	movq	-8(%rsp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -8(%rsp)
	.loc 1 31 13
	movzbl	(%rax), %ecx
	.loc 1 31 25
	movq	-16(%rsp), %rax
	leaq	1(%rax), %rdx
	movq	%rdx, -16(%rsp)
	.loc 1 31 22
	movzbl	(%rax), %eax
	.loc 1 31 12
	cmpb	%al, %cl
	je	.L10
	.loc 1 33 22
	movq	-8(%rsp), %rax
	subq	$1, %rax
	movzbl	(%rax), %edx
	.loc 1 33 31
	movq	-16(%rsp), %rax
	subq	$1, %rax
	movzbl	(%rax), %eax
	.loc 1 33 41
	cmpb	%al, %dl
	jge	.L11
	.loc 1 33 41 is_stmt 0 discriminator 1
	movl	$-1, %eax
	ret
.L11:
	.loc 1 33 41 discriminator 2
	movl	$1, %eax
	ret
.L10:
	.loc 1 29 17 is_stmt 1
	movl	-36(%rsp), %eax
	leal	-1(%rax), %edx
	movl	%edx, -36(%rsp)
	.loc 1 29 11
	testl	%eax, %eax
	jg	.L14
	.loc 1 37 12
	movl	$0, %eax
	.loc 1 38 1
	ret
	.cfi_endproc
.LFE2:
	.size	memcmp, .-memcmp
.Letext0:
	.file 2 "/home/zbin/opt/cross_64/lib/gcc/x86_64-elf/10.2.0/include/stddef.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x1a6
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.long	.LASF9
	.byte	0xc
	.long	.LASF10
	.long	.LASF11
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF0
	.uleb128 0x3
	.long	.LASF12
	.byte	0x2
	.byte	0xd1
	.byte	0x1b
	.long	0x40
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF1
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x5
	.long	.LASF4
	.byte	0x1
	.byte	0x19
	.byte	0x5
	.long	0x47
	.quad	.LFB2
	.quad	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.long	0xb8
	.uleb128 0x6
	.string	"s1"
	.byte	0x1
	.byte	0x19
	.byte	0x12
	.long	0xb8
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x6
	.string	"s2"
	.byte	0x1
	.byte	0x19
	.byte	0x1c
	.long	0xb8
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x7
	.long	.LASF2
	.byte	0x1
	.byte	0x19
	.byte	0x24
	.long	0x47
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x8
	.string	"c1"
	.byte	0x1
	.byte	0x1b
	.byte	0xb
	.long	0xba
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x8
	.string	"c2"
	.byte	0x1
	.byte	0x1c
	.byte	0xb
	.long	0xba
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x9
	.byte	0x8
	.uleb128 0xa
	.byte	0x8
	.long	0xc0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF3
	.uleb128 0x5
	.long	.LASF5
	.byte	0x1
	.byte	0xd
	.byte	0x7
	.long	0xb8
	.quad	.LFB1
	.quad	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.long	0x131
	.uleb128 0x7
	.long	.LASF6
	.byte	0x1
	.byte	0xd
	.byte	0x14
	.long	0xb8
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x6
	.string	"src"
	.byte	0x1
	.byte	0xd
	.byte	0x20
	.long	0xb8
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x6
	.string	"len"
	.byte	0x1
	.byte	0xd
	.byte	0x29
	.long	0x47
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x8
	.string	"d"
	.byte	0x1
	.byte	0xf
	.byte	0xb
	.long	0xba
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x8
	.string	"s"
	.byte	0x1
	.byte	0x10
	.byte	0xb
	.long	0xba
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0xb
	.long	.LASF13
	.byte	0x1
	.byte	0x3
	.byte	0x7
	.long	0xb8
	.quad	.LFB0
	.quad	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x6
	.string	"ptr"
	.byte	0x1
	.byte	0x3
	.byte	0x14
	.long	0xb8
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x6
	.string	"c"
	.byte	0x1
	.byte	0x3
	.byte	0x1d
	.long	0x47
	.uleb128 0x2
	.byte	0x91
	.sleb128 -36
	.uleb128 0x7
	.long	.LASF7
	.byte	0x1
	.byte	0x3
	.byte	0x27
	.long	0x34
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0xc
	.long	.LASF8
	.byte	0x1
	.byte	0x5
	.byte	0xb
	.long	0xba
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0xd
	.quad	.LBB2
	.quad	.LBE2-.LBB2
	.uleb128 0x8
	.string	"i"
	.byte	0x1
	.byte	0x6
	.byte	0xe
	.long	0x47
	.uleb128 0x2
	.byte	0x91
	.sleb128 -12
	.byte	0
	.byte	0
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
	.uleb128 0x4
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
	.uleb128 0x8
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
	.uleb128 0x9
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
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
	.byte	0
	.byte	0
	.uleb128 0xc
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
	.uleb128 0xd
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
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
.LASF11:
	.string	"/home/zbin/spectra/programs/stdlib"
.LASF6:
	.string	"dest"
.LASF9:
	.string	"GNU C99 10.2.0 -mcmodel=large -mtune=generic -march=x86-64 -g -O0 -std=gnu99 -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fomit-frame-pointer -finline-functions -fno-builtin"
.LASF12:
	.string	"size_t"
.LASF4:
	.string	"memcmp"
.LASF1:
	.string	"long unsigned int"
.LASF3:
	.string	"char"
.LASF2:
	.string	"count"
.LASF0:
	.string	"long int"
.LASF5:
	.string	"memcpy"
.LASF7:
	.string	"size"
.LASF8:
	.string	"ch_ptr"
.LASF13:
	.string	"memset"
.LASF10:
	.string	"src/memory.c"
	.ident	"GCC: (GNU) 10.2.0"
