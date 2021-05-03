	.text
	.attribute	4, 16
	.attribute	5, "rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0"
	.file	"main.c"
	.globl	add                             # -- Begin function add
	.p2align	1
	.type	add,@function
add:                                    # @add
# %bb.0:                                # %entry
	addi	sp, sp, -80
	sd	ra, 72(sp)
	sd	s0, 64(sp)
	sd	s1, 56(sp)
	sd	s2, 48(sp)
	sd	s3, 40(sp)
	addi	s0, sp, 80
	ld	t0, 8(s0)
	ld	t1, 0(s0)
	add	t2, zero, a7
	add	t3, zero, a6
	add	t4, zero, a5
	add	t5, zero, a4
	add	t6, zero, a3
	add	s1, zero, a2
	add	s2, zero, a1
	add	s3, zero, a0
	sw	a0, -44(s0)
	sw	a1, -48(s0)
	sw	a2, -52(s0)
	sw	a3, -56(s0)
	sw	a4, -60(s0)
	sw	a5, -64(s0)
	sw	a6, -68(s0)
	sw	a7, -72(s0)
	sw	t1, -76(s0)
	sw	t0, -80(s0)
	lw	a0, -44(s0)
	lw	a1, -48(s0)
	add	a0, a0, a1
	lw	a1, -52(s0)
	add	a0, a0, a1
	lw	a1, -56(s0)
	add	a0, a0, a1
	lw	a1, -60(s0)
	addw	a0, a0, a1
	lw	a1, -64(s0)
	add	a0, a0, a1
	lw	a1, -68(s0)
	add	a0, a0, a1
	lw	a1, -72(s0)
	add	a0, a0, a1
	lw	a1, -76(s0)
	add	a0, a0, a1
	lw	a1, -80(s0)
	addw	a0, a0, a1
	ld	s3, 40(sp)
	ld	s2, 48(sp)
	ld	s1, 56(sp)
	ld	s0, 64(sp)
	ld	ra, 72(sp)
	addi	sp, sp, 80
	ret
.Lfunc_end0:
	.size	add, .Lfunc_end0-add
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	1
	.type	main,@function
main:                                   # @main
# %bb.0:                                # %entry
	addi	sp, sp, -48
	sd	ra, 40(sp)
	sd	s0, 32(sp)
	addi	s0, sp, 48
	mv	a0, zero
	sw	a0, -20(s0)
	lui	a0, %hi(.L.str)
	addi	a0, a0, %lo(.L.str)
	call	printf
	add	a1, zero, sp
	addi	a2, zero, 10
	sd	a2, 8(a1)
	addi	a2, zero, 9
	sd	a2, 0(a1)
	addi	a1, zero, 1
	addi	a2, zero, 2
	addi	a3, zero, 3
	addi	a4, zero, 4
	addi	a5, zero, 5
	addi	a6, zero, 6
	addi	a7, zero, 7
	addi	t0, zero, 8
	sd	a0, -32(s0)
	add	a0, zero, a1
	add	a1, zero, a2
	add	a2, zero, a3
	add	a3, zero, a4
	add	a4, zero, a5
	add	a5, zero, a6
	add	a6, zero, a7
	add	a7, zero, t0
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
