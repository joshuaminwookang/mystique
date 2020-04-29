	.file	"circle.c"
	.option pic
	.text
	.align	1
	.globl	DrawCircle
	.type	DrawCircle, @function
DrawCircle:
	addi	sp,sp,-48
	sd	s0,40(sp)
	addi	s0,sp,48
	mv	a5,a0
	mv	a4,a1
	sd	a2,-48(s0)
	sw	a5,-36(s0)
	mv	a5,a4
	sw	a5,-40(s0)
	lw	a5,-36(s0)
	subw	a5,zero,a5
	sw	a5,-20(s0)
	lw	a5,-36(s0)
	sw	a5,-24(s0)
	sw	zero,-28(s0)
	j	.L2
.L3:
	lw	a5,-40(s0)
	slli	a5,a5,3
	ld	a4,-48(s0)
	add	a5,a4,a5
	lw	a4,-24(s0)
	sw	a4,0(a5)
	lw	a5,-40(s0)
	addiw	a4,a5,1
	sw	a4,-40(s0)
	slli	a5,a5,3
	ld	a4,-48(s0)
	add	a5,a4,a5
	lw	a4,-28(s0)
	sw	a4,4(a5)
	lw	a5,-28(s0)
	slliw	a5,a5,1
	sext.w	a5,a5
	lw	a4,-20(s0)
	addw	a5,a4,a5
	sext.w	a5,a5
	addiw	a5,a5,1
	sw	a5,-20(s0)
	lw	a5,-28(s0)
	addiw	a5,a5,1
	sw	a5,-28(s0)
	lw	a5,-20(s0)
	sext.w	a5,a5
	bltz	a5,.L2
	lw	a5,-20(s0)
	addiw	a5,a5,1
	sext.w	a4,a5
	lw	a5,-24(s0)
	slliw	a5,a5,1
	sext.w	a5,a5
	subw	a5,a4,a5
	sw	a5,-20(s0)
	lw	a5,-24(s0)
	addiw	a5,a5,-1
	sw	a5,-24(s0)
.L2:
	lw	a4,-24(s0)
	lw	a5,-28(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	bge	a4,a5,.L3
	lw	a5,-40(s0)
	mv	a0,a5
	ld	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	DrawCircle, .-DrawCircle
	.ident	"GCC: (Debian 8.2.0-7) 8.2.0"
