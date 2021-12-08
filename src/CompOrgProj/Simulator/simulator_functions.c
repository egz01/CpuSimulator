#include "simulator_functions.h"

void parse_instruction(INSTRUCTION instruction_bytes, Instruction* out)
{
	int bit_index = INSTRUCTION_SIZE_IN_BITS;
	
	bit_index -= OPCODE_BITS;
	out->opcode = (OpCode)((instruction_bytes & OPCODE_MASK) >> bit_index);
	
	bit_index -= RD_BITS;
	out->rd = (instruction_bytes & RD_MASK) >> bit_index;

	bit_index -= RS_BITS;
	out->rs = (instruction_bytes & RS_MASK) >> bit_index;

	bit_index -= RT_BITS;
	out->rt = (instruction_bytes & RT_MASK) >> bit_index;

	bit_index -= RM_BITS;
	out->rm = (instruction_bytes & RM_MASK) >> bit_index;
	
	bit_index -= IMM1_BITS;
	out->immediate1 = (instruction_bytes & IMM1_MASK) >> bit_index;
	if (out->immediate1 & 0x000000000800) // sign extend
		out->immediate1 ^= 0xfffffffff000;

	bit_index -= IMM2_BITS;
	out->immediate2 = (instruction_bytes & IMM2_MASK) >> bit_index;
	if (out->immediate2 & 0x000000000800)
		out->immediate2 ^= 0xfffffffff000;
}