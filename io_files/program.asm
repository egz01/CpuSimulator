PROGRAM:
	# test
	addi $t3, $t1, $t0, $a1, 0, 0
	addi $t3, $imm1, $t0, $v1, 1, 2
	addi $t3, $imm2, $t0, $imm2, 1, 2
	.word 64 7
L1:
	slt $t3, $imm2, $t0, $t1, 0, 2