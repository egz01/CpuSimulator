		add $t0, $imm1, $zero, $zero, -128, 0		# set x=-128
		add $t1, $imm1, $zero, $zero, -128, 0		# set y=-128
		sll $sp, $imm1, $imm2, $zero, 1, 7			# set stack to 128 words
outer:	beq $t0, $imm1, $zero, $imm2, 0, end		# at the end of the outer loop, jump to end
		beq $t1, $imm1, $zero, $imm2, 0, 			# at the end of the inner loop, advance counter by 1 and countinue to outer loop
inner:	
# calling function to set pixels, storing values in a0-a1
		add $sp, $sp, $imm1, 0, -3, 0				# push back stackpointer by 2
		sw <some reg1>, $sp, $imm1, $zero, 0, 0				# store <some reg1>
		sw <some reg2>, $sp, $imm1, $zero, 1, 0				# store <some reg2>
		jal $ra, $zero, $zero, $imm1, SET			# call function SET


end:	halt $zero, $zero, $zero, $zero, 0, 0		# if reached here, we're done
SET:	
		out $
		beq $zero, $zero, $zero, $ra, 0, 0			#return to caller

