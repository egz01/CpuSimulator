		add $s1, $zero, $zero, $zero, 0, 0			# row = 0
		lw $s0, $zero, $imm1, $zero, 0x100, 0		# load radius from dmemory
		beq $zero, $s0, $zero, $imm2, 0, END		# if r == 0, finish
		mac $s0, $s0, $s0, $zero, 0, 0				# calc $s0 = r^2
OUTER:	blt $zero, $s1, $imm1, $imm2, 256, INNER    # iterating over lines
		beq $zero, $zero, $zero, $imm2, 0, END		# if got here, finish
INNER:	blt $zero, $s2, $imm1, $imm2, 256, LOGIC	# iterating over columns
		add $s2, $zero, $zero, $zero, 0, 0			# col = 0
		add $s1, $s1, $imm1, $zero, 1, 0			# row++
		beq $zero, $zero, $zero, $imm2, 0, OUTER	# go to outer
LOGIC:	# test
		add $a0, $zero, $zero, $zero, 0, 0			# transform (i,j) -> (x,y) where (127,127) is the origin
		sub $t0, $s1, $imm1, $zero, 127, 0			# row |-> x
		mac $t0, $t0, $t0, $zero, 0, 0				# x |-> x^2
		sub $t1, $s2, $imm1, $zero, 127, 0			# col |-> y
		mac $a0, $t1, $t1, $t0, 0, 0				# y |-> y^2 + x^2
		jal $ra, $zero, $zero, $imm2, 0, SET		# call SET(x^2+y^2)
		add $s2, $s2, $zero, $imm1, 1, 0			# col++
		beq $zero, $zero, $zero, $imm2, 0, INNER	# next inner iteration
END:	halt $zero, $zero, $zero, $zero, 0, 0		# end program
SET:	ble $zero, $a0, $s0, $imm2, 0, WHITE		# check x^2 + y^2 <= r^2
BLACK:  add $a2, $imm1, $zero, $zero, 0, 0			# set color to black (not in circle)
		beq $zero, $zero, $zero, $imm2, 0, AFTER	# skip white (yes in circle)
WHITE:  add $a2, $imm1, $zero, $zero, 255, 0		# set color to white
AFTER:	mac $t0, $s1, $imm1, $s2, 256, 0			# index = row*256 + column
		out $zero, $imm1, $zero, $t0, 20, 0			# IO[monitoradr] = index
		out $zero, $imm1, $zero, $a2, 21, 0			# IO[monitordata] =  color
		out $zero, $imm1, $zero, $imm2, 22, 1		# IO[monitorcmd] =  WRITE
		beq $zero, $zero, $zero, $ra, 0, 0			# return to caller
		.word 0x100 20