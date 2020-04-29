# A method to perform circle drawing
# in RV64M	
# (c) 2019 Josh Kang
	.text
	.globl	DrawCircle
	.type	DrawCircle,@function

	# call frame size 32+16=48
	.equ	pairSize,8
	.equ	fx,0	#field y
	.equ	fy,4	#field x

	.equ	y,-28
	.equ	x,-24
	.equ	e,-20
	.equ	display,-16
	.equ	i,-8
	.equ	r,-4
	# oldfp:	+0
	# retaddr:	+8

	.equ	oldfp,32
	.equ	return,40
	.equ	frame,48
	
DrawCircle:
	addi	sp,sp,-frame	#set call frame
	sd	ra,return(sp) 	#store return address
	sd	s0,oldfp(sp)	#store old fp
	addi	s0,sp,oldfp	#set fp to +0
		
	# save parameters
	sw      a0,r(s0)	#save r (int)
	sw	a1,i(s0)	#save i (int)
	sd	a2,display(s0)	#save display (ptr)

	# sign extend all int params r and i
	sext.w	a0,a0
	sext.w	a1,a1
	
	# e = -r
	mv	a3,a0		# get r
	negw	a3,a3		# get -r
	sw	a3,e(s0)	# e = -r
	# x = r
	sw	a0,x(s0)    # store x
	# y = 0
	sw	zero,y(s0)  # store y

_check:
	# check x >=y
	lw      a3,x(s0)    	# x in a3
	lw      a4,y(s0)    	# y in a4
	sext.w  a3,a3
	sext.w  a4,a4
	
	bge	a3,a4,_loop 	# go to while loop

	# return value
	lw	a0,i(s0)	
	sext.w	a0,a0

	# unwind call frame
	ld	ra,return(sp)
	ld	s0,oldfp(sp)
	addi	sp,sp,frame
	ret

_loop:
	# while(x>=y)
	# load i and *display
	lw      a1,i(s0)    # i in a1
	sext.w  a1,a1       # ... sign extend
	ld      a2,display(s0)  # display in a2
	    
	# display[i].x = x
	# display[i++].y = y
	slli    a1,a1,3     # i << 3
	add     a2,a2,a1    # disp[i] = *disp + (i<<3)
	sw      a3,fx(a2)   # disp[i].x = x
	sw      a4,fy(a2)   # disp[i].y = y
	lw      a1,i(s0)    # i in a1
	addiw   a1,a1,1     # i++
	sw      a1,i(s0)    #   ... update stack

	# e = e + 2*y + 1
	lw      a5,e(s0)    # e in a5
	slli    a6,a4,1     # 2*y in a6
	addw    a5,a5,a6    # e +=  2*y
	addiw   a5,a5,1     #         + 1
	sw      a5,e(s0)    # store e
	    
	# y++
	addiw    a4,a4,1
	sw      a4,y(s0)    # store y

	# if (e >= 0)
	bltz    a5,_check

	# e = e+1-2*x
	addi    a5,a5,1	    # e = e+1
	slli    a6,a3,1     # 2*x in a6
	subw	a5,a5,a6    # e += 2*y +1
	sw      a5,e(s0)    # store e

	# x--
	addiw   a3,a3,-1
	sw      a3,x(s0)    # store x-=1
	j       _check      # back to loop head
    
	.size	DrawCircle,.-DrawCircle
