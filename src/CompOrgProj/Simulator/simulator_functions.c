#include "simulator_functions.h"

void parse_instruction(INSTRUCTION_TYPE instruction_bytes, Instruction* out)
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

void load_instruction_bytes(FILE* input, INSTRUCTION_TYPE* output) {
	char line[LINE_MAX_LENGTH_IN_BYTES];
	int i = 0;
	while (read_line(input, line)) {
		output[i++] = get_numeric_value_from_hex_instruction(line);
	}
}

void load_data_bytes(FILE* input, DATA_TYPE* output) {
	char line[LINE_MAX_LENGTH_IN_BYTES];
	int i = 0;
	while (read_line(input, line)) {
		output[i++] = get_numeric_value_from_hex_data(line);
	}
}

/// <summary>
/// returns a numeric value inside field, using labels as a map to labels indices
/// </summary>
/// <param name="field"> - a stripped field containing either a hexadecimal number, a decimal number or a label</param>
/// <param name="labels"> - labels map</param>
/// <returns>a numeric value hidden in field</returns>
INSTRUCTION_TYPE get_numeric_value_from_hex_instruction(const char* field) {
	INSTRUCTION_TYPE retval;
	sscanf(field, "%llx", &retval);
	return retval;
}

DATA_TYPE get_numeric_value_from_hex_data(const char* field) {
	DATA_TYPE retval;
	sscanf(field, "%x", &retval);
	return retval;
}

/// <summary>
/// reads a single line from a given input file
/// </summary>
/// <param name="input"> - file to read lines from</param>
/// <param name="line"> - line will be written here</param>
/// <returns>number of bytes in output line</returns>
int read_line(FILE* input, char* line)
{
	if (input == NULL) {
		line[0] = '\0';
		return 0;
	}

	char c;
	int stop = 0;
	int i = 0;
	while (!stop && (c = fgetc(input)) != EOF)
	{
		if (c == '\n' || c == '\0')
			stop = 1;
		else
		{
			line[i] = c;
			i++;
		}
	}
	line[i] = '\0';
	return i;
}

/// <summary>
/// preforms execution of given instruction
/// </summary>
/// <param name="inst">pointer to instruction to execute</param>
/// <param name="PC">(out) indicates index of next instruction</param>
/// <param name="registers">registers access</param>
/// <param name="IOregisters">IO access</param>
/// <param name="memory">ram access</param>
/// <param name="halt">(out) indicates whether instructed to halt program</param>
void execute(Instruction* inst, unsigned short* PC, int* registers, int* IOregisters, DATA_TYPE* memory, BOOL* halt)
{
	unsigned int temp = 0;
	registers[ZERO] = 0;
	registers[IMM1] = inst->immediate1;
	registers[IMM2] = inst->immediate2;

	switch (inst->opcode)
	{
		case (ADD):
			registers[inst->rd] = registers[inst->rs] + registers[inst->rt] + registers[inst->rm];
			break;

		case (SUB):
			registers[inst->rd] = registers[inst->rs] - registers[inst->rt] - registers[inst->rm];
			break;

		case (MAC):
			registers[inst->rd] = registers[inst->rs] * registers[inst->rt] + registers[inst->rm];
			break;

		case (AND):
			registers[inst->rd] = registers[inst->rs] & registers[inst->rt] & registers[inst->rm];
			break;

		case (OR):
			registers[inst->rd] = registers[inst->rs] | registers[inst->rt] | registers[inst->rm];
			break;

		case (XOR):
			registers[inst->rd] = registers[inst->rs] ^ registers[inst->rt] ^ registers[inst->rm];
			break;

		case (SLL):
			registers[inst->rd] = registers[inst->rs] >> registers[inst->rt];
			break;

		case (SRA):
			registers[inst->rd] = registers[inst->rs] >> registers[inst->rt];
			break;

		case (SRL):
			temp = (unsigned)registers[inst->rs];
			temp >>= registers[inst->rt];
			registers[inst->rd] = (signed)temp;
			break;

		case (BEQ):
			if (registers[inst->rs] == registers[inst->rt])
				*PC = (registers[inst->rm] & 0xfff);
			break;

		case (BNE):
			if (registers[inst->rs] != registers[inst->rt])
				*PC = (registers[inst->rm] & 0xfff);
			break;

		case (BLT):
			if (registers[inst->rs] < registers[inst->rt])
				*PC = (registers[inst->rm] & 0xfff);
			break;

		case (BGT):
			if (registers[inst->rs] > registers[inst->rt])
				*PC = (registers[inst->rm] & 0xfff);
			break;

		case (BLE):
			if (registers[inst->rs] <= registers[inst->rt])
				*PC = (registers[inst->rm] & 0xfff);
			break;

		case (BGE):
			if (registers[inst->rs] >= registers[inst->rt])
				*PC = (registers[inst->rm] & 0xfff);
			break;

		case (JAL):
			registers[inst->rd] = *PC;
			*PC = registers[inst->rm] & 0xfff;
			break;

		case (LW):
			registers[inst->rd] = memory[registers[inst->rs] + registers[inst->rt]] + registers[inst->rm];
			break;

		case (SW):
			memory[registers[inst->rs] + registers[inst->rt]] = registers[inst->rm] + registers[inst->rd];
			break;

		case (RETI):
			*PC = IOregisters[IRQRETURN];
			break;

		case (IN):
			registers[inst->rd] = IOregisters[registers[inst->rs] + registers[inst->rt]];
			break;

		case (OUT):
			IOregisters[registers[inst->rs] + registers[inst->rt]] = registers[inst->rm];
			break;

		case (HALT):
			*halt = 1;
			break;
	}
}