# Translation of GCD from C in RV64I
#
# int gcd(int a, int b)
# {
#   int result;
#   if (b==0) result = a;
#   else if (b > a) result = gcd(b,a);
#   else result = gcd(b, a%b);
#   return result;
# }

# Frame Size = 8
# a in a2
# b in a1
# result in a0

.globl gcd
.type gcd, @function

gcd:
	addi 	sp,sp,-8	# store 
	sd 	ra,8(sp)        # ... ret address
	mv 	a2,a0		# save a in a2
	bnez 	a1,else1    	# if (b==0)
	mv 	a0,a2		# result = a
	j 	return
else1:
	ble 	a1,a0,else2	# if (b>a)
	mv 	a0,a1		# b as param1
	mv 	a1,a2		# a as param2
	jal 	ra, gcd		# jump to gcd head
	j 	return		# jump to return
else2:
	remw 	a3,a2,a1	# a3  is a'%'b
	mv 	a0,a1		# b as param1
	mv 	a1,a3		# a'%'b as param2
	jal 	ra, gcd		# jump to head
return:
	ld 	ra,8(sp)	# epilogue...
	addi 	sp,sp,8
	ret
