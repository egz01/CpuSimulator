#ifndef _STRUCTS
#define _STRUCTS
#include "definitions.h"

typedef struct _Word {
	int address;
	int value;
} Word;

typedef enum _OpCode {
	ADD = 0,
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

typedef struct _Instruction {
	enum OpCode opcode;
	int rd;
	int rs;
	int rt;
	int rm;
	int immediate1;
	int immediate2;
} Instruction;

typedef enum _LineType {
	REGULAR,
	PSEUDO,
	LABEL,
	COMMENT
} LineType;

typedef enum _Register {
	ZERO = 0,
	IMM1,
	IMM2,
	V0,
	A0,
	A1,
	A2,
	T0,
	T1,
	T2,
	S0,
	S1,
	S2,
	GP,
	SP,
	RA,
} Register;

typedef enum _IOHWRegister { 
	IRQ0ENABLE = 0,
	IRQ1ENABLE,
	IRQ2ENABLE,
	IRQ0STATUS,
	IRQ1STATUS,
	IRQ2STATUS,
	IRQHANDLER,
	IRQRETURN,
	CLKS,
	LEDS,
	DISPLAY7SEG,
	TIMERENABLE,
	TIMERCURRENT,
	TIMERMAX,
	DISKCMD,
	DISKSECTOR,
	DISKBUFFER,
	DISKSTATUS,
	RESERVED1,
	RESERVED2,
	MONITORADDR,
	MONITORDATA,
	MONITORCMD
} IOHWRegister;

#endif