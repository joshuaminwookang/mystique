# Translation of GCD from C in RV64I
# New Version that Recursively calls itself
	
# int gcd(int a, int b)
# {
#   int result;
#   if (b==0) result = a;
#   else if (b > a) result = gcd(b,a);
#   else result = gcd(b, a%b);
#   return result	     ;
#}

	.equ	frame,24
# size of call frame: 24
	
	.equ	retAddr,16	# ptr,8
	.equ	oldfp,8 	# ptr,8
	.equ	a,-4 		# int,4	
	.equ	b,-8 		# int,4


# directives
	.globl	gcd
	.type	gcd,@function
	
gcd:
	addi 	sp,sp,-frame	# reserve call frame (size 48)
	sd 	ra,retAddr(sp)  # store ret address
	sd	s0,oldfp(sp)    # store fp 
	addi	s0,sp,oldfp	# move fp to +0 location (mov rsp,rbp in x86)
	# sign extend a, b
	sext.w	a0,a0
	sext.w	a1,a1
	
	sw 	a0,a(s0)	# save a
	sw	a1,b(s0)	# save b	

	bnez 	a1,else1	# if (b==0)
	# result = a, already stored in a0
	j 	return		
else1:
	ble 	a1,a0,else2	# if (b>a)
	# result = gcd(b,a)
	mv	a2,a0		# swap a<->b
	mv	a0,a1
	mv	a1,a2
	call	gcd
	j 	return		# jump to return
else2:
#	remw 	a2,a0,a1	# a2  is a'%'b
	subw	a2,a0,a1
	mv 	a0,a1		# b as param1
	mv 	a1,a2		# a'%'b as param2
	call	gcd
	j	return
return:
	mv	sp,s0
	ld 	s0,0(sp)
	ld	ra,8(sp)
	addi 	sp,sp,16
	ret
