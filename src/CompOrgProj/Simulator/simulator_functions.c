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