	sll $sp, $imm1, $imm2, $zero, 1, 10			 # set $sp = 1 << 10 = 1024
	lw $a0, $zero, $imm2, $zero, 0, 0x100		 # load n from address 0x100
	lw $a1, $zero, $imm2, $zero, 0, 0x101		 # load k from address 0x101
	jal $ra, $zero, $zero, $imm2, 0, Start		 # Start the recursion $v0 = binom(n,k)
	sw $zero, $zero, $imm2, $v0, 0, 0x102		 # Store result binom(n,k) in 0x102
	halt $zero, $zero, $zero, $zero, 0, 0		 # halt

						
				
Start:                                           
	add $sp, $sp, $imm1, $zero, -4, 0	     	 # allocate space in the stack
	sw $zero, $sp, $imm1, $s0, 3, 0		     	 # save $s0
	sw $zero, $sp, $imm1, $ra, 2, 0		     	 # save return address
	sw $zero, $sp, $imm1, $a0, 1, 0		     	 # save argument 1 
	sw $zero, $sp, $imm1, $a1, 0, 0		     	 # save argument 2
	beq $zero, $a0, $a1, $imm1, Stop, 0	         # if k==n stop
	beq $zero, $a1, $zero, $imm1, Stop, 0        # if k==0 stop
	beq $zero, $zero, $zero, $imm1, Recursion, 0 # else go to recursion step
Stop:

			
	add $v0, $imm1, $zero, $zero, 1, 0		     # otherwise, binom(n,k) = 1
	beq $zero, $zero, $zero, $imm2, 0, Finish	 # jump to Finish
Recursion:
	sub $a0, $a0, $imm1, $zero, 1, 0		     # calculate n - 1
	jal $ra, $zero, $zero, $imm1, Start, 0 		 # recursion for (n-1,k) $v0=binom(n-1,k)
	add $s0, $v0, $imm1, $zero, 0, 0		     # save result $s0 = binom(n-1,k)
	lw $a0, $sp, $imm1, $zero, 1, 0			     # load $a0 = n
	sub $a0, $a0, $imm1, $zero, 1, 0		     # calculate n - 1
	sub $a1, $a1, $imm1, $zero, 1, 0		     # calculate k-1
	jal $ra, $zero, $zero, $imm1, Start, 0		 # recursion for (n-1,k-1) binom(n-1,k-1)
	add $v0, $v0, $s0, $zero, 0, 0		       	 # combine results $v0 = binom(n-1,k-1) + binom(n-1,k)
Finish:
	lw $a1, $sp, $imm1, $zero, 0, 0			     # load $a0
	lw $a0, $sp, $imm1, $zero, 1, 0			     # load $a1
	lw $ra, $sp, $imm1, $zero, 2, 0			     # load $ra
	lw $s0, $sp, $imm1, $zero, 3, 0			     # load $s0
	add $sp, $sp, $imm1, $zero, 4, 0		     # pop items from stack
	beq $zero, $zero, $zero, $ra, 0, 0		     # return
	.word 0x100 7
	.word 0x101 5