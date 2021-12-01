import os

def generate_if(code, iselse):
    s = "{_else}if (opcode == \"{c}\")\n\treturn {C};".format(_else='else' if iselse else "", c=code.lower(), C=code.upper())
    return s
    
opcodes = [] # opcodes strings

with open('temp.txt', 'w') as f:
    iselse = False
    for code in opcodes:
    f.write(generate_if(code, iselse))
    f.write('\r\n')
    iselse = True
    
os.popen("notepad temp.txt")