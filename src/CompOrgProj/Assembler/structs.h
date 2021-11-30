#ifndef _STRUCTS
#define _STRUCTS

typedef struct _Instruction
{
	char opcode;
	char rd;
	char rs;
	char rt;
	char rm;
	short immediate1;
	short immediate2;

} Instruction;

typedef enum _OpCode {
	ADD,
	SUB,
	MAC,
	AND,
	OR,
	XOR,
	SLL,
	SRA,
	SRL,
	BEQ,
	BNE,
	BLT,
	BGT,
	BLE,
	BGE,
	JAL,
	LW,
	SW,
	RETI,
	IN,
	OUT,
	HALT,
} OpCode;

typedef enum _LineType {
	REGULAR,
	PSEUDO,
	LABEL
} LineType;

#endif