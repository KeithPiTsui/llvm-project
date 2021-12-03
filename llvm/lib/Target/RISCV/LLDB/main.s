	.text
	.attribute	4, 16
	.attribute	5, "rv32i2p0"
	.file	"main.c"
	.globl	add                             # -- Begin function add
	.p2align	1
	.type	add,@function
add:                                    # @add
.Ladd$local:
# %bb.0:                                # %entry
	addi	sp, sp, -48
	sw	ra, 44(sp)
	sw	s0, 40(sp)
	addi	s0, sp, 48
	sw	a0, -12(s0)
	sw	a1, -16(s0)
	sw	a2, -20(s0)
	sw	a3, -24(s0)
	sw	a4, -28(s0)
	sw	a5, -32(s0)
	sw	a6, -36(s0)
	sw	a7, -40(s0)
	lw	a0, -12(s0)
	lw	a1, -16(s0)
	add	a0, a0, a1
	lw	a1, -20(s0)
	add	a0, a0, a1
	lw	a1, -24(s0)
	add	a0, a0, a1
	lw	a1, -28(s0)
	add	a0, a0, a1
	lw	a1, -32(s0)
	add	a0, a0, a1
	lw	a1, -36(s0)
	add	a0, a0, a1
	lw	a1, -40(s0)
	add	a0, a0, a1
	lw	a1, 0(s0)
	add	a0, a0, a1
	lw	a1, 4(s0)
	add	a0, a0, a1
	lw	s0, 40(sp)
	lw	ra, 44(sp)
	addi	sp, sp, 48
	ret
.Lfunc_end0:
	.size	add, .Lfunc_end0-add
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	1
	.type	main,@function
main:                                   # @main
.Lmain$local:
# %bb.0:                                # %entry
	addi	sp, sp, -48
	sw	ra, 44(sp)
	sw	s0, 40(sp)
	addi	s0, sp, 48
	mv	a2, zero
	sw	a2, -12(s0)
	sw	a0, -16(s0)
	sw	a1, -24(s0)
.LBB1_1:                                # %entry
                                        # Label of block must be emitted
	auipc	a0, %pcrel_hi(.L.str)
	addi	a0, a0, %pcrel_lo(.LBB1_1)
	call	printf
.LBB1_2:                                # %entry
                                        # Label of block must be emitted
	auipc	a1, %pcrel_hi(x)
	addi	a1, a1, %pcrel_lo(.LBB1_2)
	lw	a1, 0(a1)
.LBB1_3:                                # %entry
                                        # Label of block must be emitted
	auipc	a2, %pcrel_hi(y)
	addi	a2, a2, %pcrel_lo(.LBB1_3)
	lw	a2, 0(a2)
	add	a3, zero, sp
	addi	a4, zero, 10
	sw	a4, 4(a3)
	addi	a4, zero, 9
	sw	a4, 0(a3)
	addi	a3, zero, 3
	addi	a4, zero, 4
	addi	a5, zero, 5
	addi	a6, zero, 6
	addi	a7, zero, 7
	addi	t0, zero, 8
	sw	a0, -28(s0)
	add	a0, zero, a1
	add	a1, zero, a2
	add	a2, zero, a3
	add	a3, zero, a4
	add	a4, zero, a5
	add	a5, zero, a6
	add	a6, zero, a7
	add	a7, zero, t0
	call	add
	lw	s0, 40(sp)
	lw	ra, 44(sp)
	addi	sp, sp, 48
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
                                        # -- End function
	.type	x,@object                       # @x
	.section	.sdata,"aw",@progbits
	.globl	x
	.p2align	2
x:
.Lx$local:
	.word	1                               # 0x1
	.size	x, 4

	.type	y,@object                       # @y
	.globl	y
	.p2align	2
y:
.Ly$local:
	.word	2                               # 0x2
	.size	y, 4

	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"hello world!\n"
	.size	.L.str, 14

	.ident	"clang version 11.1.0 (git@github.com:KeithPiTsui/llvm-project.git 50024be1edf3ef9951f75df642f59f872d468243)"
	.section	".note.GNU-stack","",@progbits
