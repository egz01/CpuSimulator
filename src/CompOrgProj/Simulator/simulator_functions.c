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

void dump_data(FILE* output_stream, DATA_TYPE* output_data, int size)
{
	for (int i = 0; i < size; i++)
	{
		fprintf(output_stream, "%08X\n", output_data[i]);
	}
}

void dump_byte_data(FILE* output_stream, char* output_data)
{
	for (int i = 0; i < DISK_SECTORS * SECTOR_SIZE_IN_BYTES; i++)
	{
		fprintf(output_stream, "%02hhX\n", output_data[i]);
	}
}

void dump_pixels_string(FILE* output_stream, char* output_data)
{
	for (int i = 0; i < SCREEN_X * SCREEN_Y; i++)
	{
		fprintf(output_stream, "%02hhX\n", output_data[i]);
	}
}

void dump_pixels_binary(FILE* binary_stream, char* output_data)
{
	fwrite(output_data, 1, DISK_SECTORS * SECTOR_SIZE_IN_BYTES, binary_stream);
}

int find_dmemory_index(DATA_TYPE* data_memory, int max_size)
{
	int last_non_zero_index = 0;
	for (int i = 0; i < max_size; i++)
	{
		if (data_memory[i] != 0)
		{
			last_non_zero_index = i;
		}
	}
	return last_non_zero_index;
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

void update_rd(int rd, int value, int* registers)
{
	if (rd != ZERO && rd != IMM1 && rd != IMM2)
		registers[rd] = value;
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
void execute(Instruction* inst, unsigned short* PC, int* registers, int* IOregisters, DATA_TYPE* memory, BOOL* halt, BOOL* in_interrupt)
{
	unsigned int temp = 0;
	int value = 0;

	switch (inst->opcode)
	{
		case (ADD):
			value = registers[inst->rs] + registers[inst->rt] + registers[inst->rm];
			update_rd(inst->rd, value, registers);
			(*PC)++;
			break;

		case (SUB):
			value = registers[inst->rs] - registers[inst->rt] - registers[inst->rm];
			update_rd(inst->rd, value, registers);
			(*PC)++;
			break;

		case (MAC):
			value = registers[inst->rs] * registers[inst->rt] + registers[inst->rm];
			update_rd(inst->rd, value, registers);
			(*PC)++;
			break;

		case (AND):
			value = registers[inst->rs] & registers[inst->rt] & registers[inst->rm];
			update_rd(inst->rd, value, registers);
			(*PC)++;
			break;

		case (OR):
			value = registers[inst->rs] | registers[inst->rt] | registers[inst->rm];
			update_rd(inst->rd, value, registers);
			(*PC)++;
			break;

		case (XOR):
			value = registers[inst->rs] ^ registers[inst->rt] ^ registers[inst->rm];
			update_rd(inst->rd, value, registers);
			(*PC)++;
			break;

		case (SLL):
			value = registers[inst->rs] << registers[inst->rt];
			update_rd(inst->rd, value, registers);
			(*PC)++;
			break;

		case (SRA):
			value = registers[inst->rs] >> registers[inst->rt];
			update_rd(inst->rd, value, registers);
			(*PC)++;
			break;

		case (SRL):
			temp = (unsigned)registers[inst->rs];
			temp >>= registers[inst->rt];
			registers[inst->rd] = (signed)temp;
			update_rd(inst->rd, value, registers);
			(*PC)++;
			break;

		case (BEQ):
			if (registers[inst->rs] == registers[inst->rt])
				*PC = (registers[inst->rm] & 0xfff);
			else (*PC)++;
			break;

		case (BNE):
			if (registers[inst->rs] != registers[inst->rt])
				*PC = (registers[inst->rm] & 0xfff);
			else (*PC)++;
			break;

		case (BLT):
			if (registers[inst->rs] < registers[inst->rt])
				*PC = (registers[inst->rm] & 0xfff);
			else (*PC)++;
			break;

		case (BGT):
			if (registers[inst->rs] > registers[inst->rt])
				*PC = (registers[inst->rm] & 0xfff);
			else (*PC)++;
			break;

		case (BLE):
			if (registers[inst->rs] <= registers[inst->rt])
				*PC = (registers[inst->rm] & 0xfff);
			else (*PC)++;
			break;

		case (BGE):
			if (registers[inst->rs] >= registers[inst->rt])
				*PC = (registers[inst->rm] & 0xfff);
			else (*PC)++;
			break;

		case (JAL):
			value = *PC + 1;
			update_rd(inst->rd, value, registers);
			*PC = registers[inst->rm] & 0xfff;
			break;

		case (LW):
			value = memory[registers[inst->rs] + registers[inst->rt]] + registers[inst->rm];
			update_rd(inst->rd, value, registers);
			(*PC)++;
			break;

		case (SW):
			memory[registers[inst->rs] + registers[inst->rt]] = registers[inst->rm] + registers[inst->rd];
			(*PC)++;
			break;

		case (RETI):
			*PC = IOregisters[IRQRETURN];
			*in_interrupt = FALSE;
			break;

		case (IN):
			if (registers[inst->rs] + registers[inst->rt] == MONITORCMD)
				value = 0;
			value = IOregisters[registers[inst->rs] + registers[inst->rt]];
			update_rd(inst->rd, value, registers);
			(*PC)++;
			break;

		case (OUT):
			IOregisters[registers[inst->rs] + registers[inst->rt]] = registers[inst->rm];
			(*PC)++;
			break;

		case (HALT):
			*halt = 1;
			(*PC)++;
			break;
	}
}

/// <summary>
/// reads irq2 activation cycles
/// </summary>
/// <param name="input"></param>
/// <param name="output"></param>
long long int* load_irq2_cycles(FILE* input)
{
	char line[LINE_MAX_LENGTH_IN_BYTES];
	int size = 1000;
	int cur_size = 0;
	long long int* output = (long long int*)malloc(sizeof(long long int) * size);
	long long int num = 0;
	
	while (read_line(input, line) > 0)
	{
		sscanf(line, "%lld", &num);
		output[cur_size] = num;
		cur_size++;
		if (cur_size > size)
		{
			size *= 2;
			realloc(output, size * sizeof(long long int*));
		}
	}
	output = (long long int*)realloc(output, (cur_size+1) * sizeof(long long int*));
	output[cur_size] = -1;
	return output;
}

BOOL irq2triggered(long long cycle_num, long long* irq2cycles)
{
	long long c;
	long long* p = irq2cycles;
	while ((c = *(p++)) != -1)
	{
		if (cycle_num == c) return TRUE;
	}
	return FALSE;
}

void update_trace(int PC, INSTRUCTION_TYPE instruction, DATA_TYPE* registers, FILE* output)
{
	fprintf(output, "%03X %012llX", PC, instruction);
	for (int i = 0; i < NUM_REGISTERS; i++)
	{
		fprintf(output, " %08x", registers[i]);
	}
	fprintf(output, "\n");
}

void update_hwtrace(long long int cycle, OpCode code, IOHWRegister name, DATA_TYPE data, FILE* output)
{
	char str[100];
	get_string_from_opcode(name, str);

	fprintf(output, "%ld ", cycle);
	if (code == IN)
		fprintf(output, "%s ", "READ");
	else if (code == OUT)
		fprintf(output, "%s ", "WRITE");
	fprintf(output, "%s ", str);
	fprintf(output, "%08lx\n", data);
}

void get_string_from_opcode(IOHWRegister code, char str[])
{
	if (code == 0)
		memcpy(str, "irq0enable", 1 + strlen("irq0enable"));

	else if (code == 1)
		memcpy(str, "irq1enable", 1 + strlen("irq1enable"));

	else if (code == 2)
		memcpy(str, "irq2enable", 1 + strlen("irq2enable"));

	else if (code == 3)
		memcpy(str, "irq0status", 1 + strlen("irq0status"));

	else if (code == 4)
		memcpy(str, "irq1status", 1 + strlen("irq1status"));

	else if (code == 5)
		memcpy(str, "irq2status", 1 + strlen("irq2status"));

	else if (code == 6)
		memcpy(str, "irqhandler", 1 + strlen("irqhandler"));

	else if (code == 7)
		memcpy(str, "irqreturn", 1 + strlen("irqreturn"));

	else if (code == 8)
		memcpy(str, "clks", 1 + strlen("clks"));

	else if (code == 9)
		memcpy(str, "leds", 1 + strlen("leds"));

	else if (code == 10)
		memcpy(str, "display7seg", 1 + strlen("display7seg"));

	else if (code == 11)
		memcpy(str, "timerenable", 1 + strlen("timerenable"));

	else if (code == 12)
		memcpy(str, "timercurrent", 1 + strlen("timercurrent"));

	else if (code == 13)
		memcpy(str, "timermax", 1 + strlen("timermax"));

	else if (code == 14)
		memcpy(str, "diskcmd", 1 + strlen("diskcmd"));

	else if (code == 15)
		memcpy(str, "disksector", 1 + strlen("disksector"));

	else if (code == 16)
		memcpy(str, "diskbuffer", 1 + strlen("diskbuffer"));

	else if (code == 17)
		memcpy(str, "diskstatus", 1 + strlen("diskstatus"));

	else if (code == 18)
		memcpy(str, "reserved1", 1 + strlen("reserved1"));

	else if (code == 19)
		memcpy(str, "reserved2", 1 + strlen("reserved2"));

	else if (code == 20)
		memcpy(str, "monitoraddr", 1 + strlen("monitoraddr"));

	else if (code == 21)
		memcpy(str, "monitordata", 1 + strlen("monitordata"));

	else if (code == 22)
		memcpy(str, "monitorcmd", 1 + strlen("monitorcmd"));
}