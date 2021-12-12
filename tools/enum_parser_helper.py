import os

def generate_if(code, iselse, varname):
    s = "{_else} if (strcmp({v},\"{c}\") == 0)\n\treturn {C};".format(v=varname, _else='else' if iselse else "", c=code.lower(), C=code.upper())
    return s
    
def generate_case(code):
    s = "case ({c}):".format(c=code)
    return s
    
def generate_memcpy(code, index, iselse, varname, out_varname):
    s = "{_else}if ({v}=={c})\n\t memcpy({o}, \"{C}\", strlen(\"{C}\"));".format(v=varname, _else='else ' if iselse else "", c=index, C=code.upper(), o=out_varname)
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

hwregisters = [ "IRQ0ENABLE",
	"IRQ1ENABLE",
	"IRQ2ENABLE",
	"IRQ0STATUS",
	"IRQ1STATUS",
	"IRQ2STATUS",
	"IRQHANDLER",
	"IRQRETURN",
	"CLKS",
	"LEDS",
	"DISPLAY7SEG",
	"TIMERENABLE",
	"TIMERCURRENT",
	"TIMERMAX",
	"DISKCMD",
	"DISKSECTOR",
	"DISKBUFFER",
	"DISKSTATUS",
	"RESERVED1",
	"RESERVED2",
	"MONITORADDR",
	"MONITORDATA",
	"MONITORCMD"
]

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
        
with open('temp3.txt', 'w') as f:
    for code in opcodes:
        f.write(generate_case(code))
        f.write('\r\n')    
                
os.popen("notepad temp3.txt")

with open('temp4.txt', 'w') as f:
    iselse = False
    for i, code in zip(range(len(opcodes)), hwregisters):
        f.write(generate_memcpy(code, i, iselse, 'code', 'str'))
        f.write('\r\n')
        iselse = True
        
os.popen("notepad temp4.txt")