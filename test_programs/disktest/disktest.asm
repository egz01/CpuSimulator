		out $zero, $zero, $imm1, $imm1, 1, 0		# enable irq1
		out $zero, $imm1, $zero, $imm2, 6, irq1		# set irqhandler as L1
		sll $sp, $imm1, $imm2, $zero, 1, 11			# set $sp = 1 << 11 = 2048 to prepare for stack operations
		add $t0, $imm1, $zero, $zero, 7, 0			# set i = 7
		add $t1, $zero, $zero, $zero, 0, 0			# set $t1 to 0, used as addr in memory
for:   	
		beq $zero, $t0, $imm1, $imm2, 0, end		# found a bug in assembler!!
		out $zero, $imm1, $zero, $t1, 16, 0			# set diskbuffer to $t1 which is 0	
		out $zero, $imm1, $zero, $t0, 15, 0			# set disksector to $t0 which is i
		out $zero, $imm1, $zero, $imm2, 14, 1		# set diskcmd to read # iterate until i == 0
		add $t2, $imm1, $zero, $zero, 1, 0			# set $t2 <- 1, prepare to wait until freed
while1: and $zero, $zero, $zero, $zero, 0, 0		# noop
		beq $zero, $t2, $imm1, $imm2, 1, while1		# return to while1
		add $t0, $imm1, $t0, $zero, -1, 0			# calculate $t0 <- (i-1)
		out $zero, $imm1, $zero, $t1, 16, 0			# set diskbuffer to $t1 which is 0
		out $zero, $imm1, $zero, $t0, 15, 0			# set disksector to $t0 which is i-1
		out $zero, $imm1, $zero, $imm2, 14, 2		# set diskcmd to write
		add $t2, $imm1, $zero, $zero, 1, 0			# set $t2 <- 1, prepare to wait until freed again
while2:	and $zero, $zero, $zero, $zero, 0, 0		# noop
		beq $zero, $t2, $imm1, $imm2, 1, while2		# return to while2
		bge $zero, $t0, $zero, $imm1, for, 0		# go back to for
end:	halt $zero, $zero, $zero, $zero, 0, 0		# if reached here, we're done
irq1:	add $t2, $zero, $zero, $zero, 0, 0			# free the waiters
		out $zero, $imm1, $zero, $imm2, 4, 0		# toggle irq1status
		reti $zero, $zero, $zero, $zero, 0, 0		# return from interrupt
