import os

def generate_if(code, iselse, varname):
    s = "{_else} if (strcmp({v},\"{c}\") == 0)\n\treturn {C};".format(v=varname, _else='else' if iselse else "", c=code.lower(), C=code.upper())
    return s
    
opcodes = ["ADD",
	"SUB",
	"MAC",
	"AND",
	"OR",
	"XOR",
	"SLL",
	"SRA",
	"SRL",
	"BEQ",
	"BNE",
	"BLT",
	"BGT",
	"BLE",
	"BGE",
	"JAL",
	"LW",
	"SW",
	"RETI",
	"IN",
	"OUT",
	"HALT"] # opcodes strings
    
registers = ["$ZERO",
	"$IMM1",
	"$IMM2",
	"$V0",
	"$A0",
	"$A1",
	"$A2",
	"$T0",
	"$T1",
	"$T2",
	"$S0",
	"$S1",
	"$S2",
	"$GP",
	"$SP",
	"$RA"]

with open('temp1.txt', 'w') as f:
    iselse = False
    for code in opcodes:
        f.write(generate_if(code, iselse, 'opcode'))
        f.write('\r\n')
        iselse = True
    
os.popen("notepad temp1.txt")

with open('temp2.txt', 'w') as f:
    iselse = False
    for reg in registers:
        f.write(generate_if(reg, iselse, 'reg'))
        f.write('\r\n')
        iselse = True
        
os.popen("notepad temp2.txt")