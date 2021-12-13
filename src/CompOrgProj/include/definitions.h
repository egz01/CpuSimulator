#ifndef _DEFINITIONS
#define _DEFINITIONS

// type definitions for readable code
#define BOOL int
#define TRUE 1
#define FALSE 0
#define INSTRUCTION_TYPE unsigned long long int
#define DATA_TYPE unsigned int
#define NOOP 0
#define READ 1
#define WRITE 2
#define FREE 0
#define BUSY 1
#define ENABLED 1
#define DISABLED 0

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
#define NUM_IOREGISTERS 23

#define SCREEN_X 256
#define SCREEN_Y 256

#define DISK_SECTORS 128
#define SECTOR_SIZE_IN_BYTES 512
#define DISK_OPERATION_CYCLE_LENGTH 1024

// instruction structure
#define OPCODE_BITS 8
#define	RD_BITS 4
#define RS_BITS 4
#define RT_BITS 4
#define RM_BITS 4
#define IMM1_BITS 12
#define IMM2_BITS 12

#define OPCODE_MASK 0xff0000000000
#define RD_MASK		0x00f000000000
#define RS_MASK		0x000f00000000
#define RT_MASK		0x0000f0000000
#define RM_MASK		0x00000f000000
#define IMM1_MASK	0x000000fff000
#define IMM2_MASK	0x000000000fff
#endif