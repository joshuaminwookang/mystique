# ASM code to manipulate empty CSR register in Rocket (RV64)
# (c) 2019 Josh Kang
	.text
	.globl	setwfi
	.type	setwfi,@function

	# call frame size 16
	# oldfp:	+0
	# retaddr:	+8

	.equ	oldfp,0
	.equ	return,8
	.equ	frame,16

setwfi:
	addi	sp,sp,-frame	#set call frame
	sd	ra,return(sp) 	#store return address
	sd	s0,oldfp(sp)	#store old fp
	addi	s0,sp,oldfp
	
	mv	a2,a0		#save parameter (int of what to store in CSR)
	csrrc	a0,0x803,x0 	#read CSR, don't write
	csrrw	x0,0x803,a2	#write a2 (param) to CSR
				#read value in CSR to a0

	# unwind call frame
	ld	ra,return(sp)
	ld	s0,oldfp(sp)
	addi	sp,sp,frame
	ret

	.size	setwfi,.-setwfi
