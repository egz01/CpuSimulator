# calcs A*B=C				
				add $s0,$zero,$zero,$zero,0,0				# set i = 0
Newrow:			add $s1,$zero,$zero,$zero,0,0				# set j = 0
Newcolumn:		
				add $s2,$zero,$zero,$zero,0,0				# set k = 0
				add $t1,$zero,$zero,$zero,0,0				# set $t1 = 0 holds the value to be set at C[i][j]
				sll $t2,$s0,$imm1,$zero,2,0					# $t2 = i*4
				add $t2,$t2,$s1,$zero,0,0					# $t2 += j
				add $t2,$t2,$imm1,$zero,0x120,0				# $t2 += result address offset - holds address of C[i][j]
Calcmult:		sll $t0,$s2,$imm1,$zero,2,0					# $t0 = k*4
				add $t0,$t0,$s1,$zero,0,0					# $t0 += j
				add $t0,$t0,$imm1,$zero,0x110,0				# $t0 = the address of B[k][j]
				lw $t0,$t0,$zero,$zero,0,0					# $t0 = B[k][j]
				sll $a0,$s0,$imm1,$zero,2,0					# $a0 = i*4
				add $a0,$a0,$s2,$zero,0,0					# $a0 += k
				add $a0,$a0,$imm1,$zero,0x100,0				# $a0 = the address of A[i][k]
				lw $a0,$a0,$zero,$zero,0,0					# $a0 = A[i][k]
				mac $t1,$t0, $a0, $t1, 0, 0					# $t1= A[i][k]*B[k][j]

				add $s2 ,$s2,$zero,$imm1, 1,0				# k = k+1

				bne $zero, $imm1, $s2, $imm2, 4,Calcmult	# if k not equal to 4  jump to Calcmult
				sw $t1 , $t2, $zero, $zero,0 ,0				# C[i][j] stored in the  result address

				add $s1 , $s1,$zero,$imm1, 1,0				# j = j+1

				bne $zero, $imm1, $s1, $imm2,4,Newcolumn	# if j not equal to 4 jump to Newcolumn
				add $s0 , $s0,$zero,$imm1, 1,0				# i = i+1
				bne $zero, $imm1, $s0, $imm2, 4,Newrow  	# if i not equal to 4 jump to Newrow
				halt $zero,$zero,$zero,$zero,0,0			# halt, we have reached the end of the matrix
				.word 0x100 0x0
				.word 0x101 0x1
				.word 0x102 0x2
				.word 0x103 0x3
				.word 0x104 0x4
				.word 0x105 0x5
				.word 0x106 0x6
				.word 0x107 0x7
				.word 0x108 0x8
				.word 0x109 0x9
				.word 0x10A 0xA
				.word 0x10B 0xB
				.word 0x10C 0xC
				.word 0x10D 0xD
				.word 0x10E 0xE
				.word 0x10F 0xF
				.word 0x110 0x0
				.word 0x111	0x1
				.word 0x112	0x2
				.word 0x113	0x3
				.word 0x114	0x4
				.word 0x115	0x5
				.word 0x116	0x6
				.word 0x117	0x7
				.word 0x118	0x8
				.word 0x119	0x9
				.word 0x11A	0xA
				.word 0x11B	0xB
				.word 0x11C	0xC
				.word 0x11D	0xD
				.word 0x11E	0xE
				.word 0x11F 0xF