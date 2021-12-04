#ifndef _DEFINITIONS
#define _DEFINITIONS

// type definitions for readable code
#define BOOL char
#define TRUE 1
#define FALSE 0
#define INSTRUCTION unsigned long long

// physical limitations constants
#define INSTRUCTIONS_DEPTH 4096
#define MEMORY_DEPTH 4096

#define LINE_MAX_LENGTH_IN_BYTES 500
#define LABEL_MAX_LENGTH 50

#define INSTRUCTION_SIZE_IN_BITS 48
#define INSTRUCTION_SIZE_IN_CHARS 12

#define MEMORY_SIZE_IN_BITS 32
#define MEMORY_SIZE_IN_CHARS 8

#define NUM_REGISTERS 16

// instruction structure
#define OPCODE_BITS 8
#define	RD_BITS 4
#define RS_BITS 4
#define RT_BITS 4
#define RM_BITS 4
#define IMM1_BITS 12
#define IMM2_BITS 12

#endif