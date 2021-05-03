	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0"
	.file	"main.c"
	.globl	add                             # -- Begin function add
	.p2align	1
	.type	add,@function
add:                                    # @add
.Ladd$local:
# %bb.0:                                # %entry
	addi	sp, sp, -64
	sd	ra, 56(sp)
	sd	s0, 48(sp)
	addi	s0, sp, 64
	ld	t0, 8(s0)
	ld	t1, 0(s0)
	sw	a0, -20(s0)
	sw	a1, -24(s0)
	sw	a2, -28(s0)
	sw	a3, -32(s0)
	sw	a4, -36(s0)
	sw	a5, -40(s0)
	sw	a6, -44(s0)
	sw	a7, -48(s0)
	sw	t1, -52(s0)
	sw	t0, -56(s0)
	lw	a0, -20(s0)
	lw	a1, -24(s0)
	add	a0, a0, a1
	lw	a1, -28(s0)
	add	a0, a0, a1
	lw	a1, -32(s0)
	add	a0, a0, a1
	lw	a1, -36(s0)
	addw	a0, a0, a1
	lw	a1, -40(s0)
	add	a0, a0, a1
	lw	a1, -44(s0)
	add	a0, a0, a1
	lw	a1, -48(s0)
	add	a0, a0, a1
	lw	a1, -52(s0)
	add	a0, a0, a1
	lw	a1, -56(s0)
	addw	a0, a0, a1
	ld	s0, 48(sp)
	ld	ra, 56(sp)
	addi	sp, sp, 64
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
	sd	ra, 40(sp)
	sd	s0, 32(sp)
	addi	s0, sp, 48
	sw	zero, -20(s0)
.LBB1_1:                                # %entry
                                        # Label of block must be emitted
	auipc	a0, %pcrel_hi(.L.str)
	addi	a0, a0, %pcrel_lo(.LBB1_1)
	call	printf
	addi	a0, zero, 10
	sd	a0, 8(sp)
	addi	a0, zero, 9
	sd	a0, 0(sp)
	addi	a0, zero, 1
	addi	a1, zero, 2
	addi	a2, zero, 3
	addi	a3, zero, 4
	addi	a4, zero, 5
	addi	a5, zero, 6
	addi	a6, zero, 7
	addi	a7, zero, 8
	call	add
	ld	s0, 32(sp)
	ld	ra, 40(sp)
	addi	sp, sp, 48
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"hello world!\n"
	.size	.L.str, 14

	.ident	"clang version 11.1.0 (git@github.com:KeithPiTsui/llvm-project.git e8ab6a7603b475cd2821eb2727e2e9c72f311423)"
	.section	".note.GNU-stack","",@progbits
