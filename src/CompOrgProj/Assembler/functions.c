#include "functions.h"

void copy_temp() {
	FILE* dmemin = fopen("dmemin.txt", "w");
	FILE* temp = fopen("temp.txt", "r");
	char copy_data[10];
	while (fgets(copy_data, 10, temp) != NULL)
	{
		fputs(copy_data, dmemin);

	}
	fclose(temp);
	fclose(dmemin);
}

void word(int address, long data, FILE* dmemin) {
	int counter = -1;
	char copy_data[10];
	FILE* temp = fopen("temp.txt", "w");
	while (fgets(copy_data, 10, dmemin) != NULL)
	{
		counter += 1;
		if (counter == address)
		{
			fputs(data, temp);
			fputs("\n", temp);
		}
		else
		{
			//printf("copydata:%s", copy_data);
			fputs(copy_data, temp);
			//fputs("\n" ,temp);
		}
	}
	fclose(dmemin);
	if (counter < address)
	{
		if (counter != -1) {
			fputs("\n", temp);
		}
		counter += 1;
		while (counter < address)
		{
			fputs("00000000\n", temp);
			counter += 1;
		}
		fputs(data, temp);
	}
	fclose(temp);
	copy_temp();
}

/// <summary>
/// fixes trailing, leading, inbetweening whitespaces and tabs in given string
/// </summary>
/// <param name="to_fix">input string with various extra spaces and tabs</param>
/// <param name="fixed">output string (should be at least of equal size to input string) with no extra whitespaces or tabs</param>
void remove_extra_spaces_and_tabs(char* to_fix, char* fixed)
{
	char c;
	int i = 0;
	int j = 0;
	int spaced = 0;
	int first = 1;
	char temp[INSTRUCTIONS_MAX_SIZE];
	temp[INSTRUCTIONS_MAX_SIZE - 1] = '\0';
	char* ptr;

	while ((c = to_fix[i]) != '\0') {
		if (spaced)
		{
			if (c == '\t' || c == ' ') {}
			else
			{
				// exited from space state
				spaced = 0;
			}
		}

		if (!spaced)
		{
			if (c == '\t' || c == ' ')
			{
				if (first)
				{
					i++;
					continue;
				}
				else
					fixed[j] = ' ';
				spaced = 1;
			}
			else
			{
				first = 0;
				fixed[j] = c;
			}
			j++;
		}
		i++;
	}

	// remove trailing spaces:
	if (fixed[j - 1] == ' ')
		fixed[j - 1] = '\0';
	else
		fixed[j] = '\0';

	while ((ptr = strstr(fixed, " ,")) != NULL)
	{
		int len = strlen(fixed);
		strncpy(temp, fixed, ptr - fixed);
		strncpy(temp + (ptr - fixed), ptr + 1, len - (ptr - fixed) + 1);
		temp[len - 1] = '\0';
		strncpy(fixed, temp, strlen(temp));
		fixed[len - 1] = '\0';
	}
}

/// <summary>
/// use this function to parse an input line into an instruction struct.
/// if the given line doesn't contain an instruction (i.e. pseudo / 
/// </summary>
/// <param name="instruction"> - instruction string as read from file</param>
/// <param name="output"> - output insturction struct</param>
/// <returns>type of line received, i.e. {regular instruction, pseudo instruction, label}</returns>
LineType parse_line(char* line, int length, Instruction* output, short* labels) {
	LineType retval = REGULAR;
	char cleaned_line[LINE_MAX_LENGTH_IN_BYTES];
	remove_extra_spaces_and_tabs(line, cleaned_line);
	
	if (is_label(cleaned_line)) {
		// handle label, do nothing?
		retval = LABEL;
	}
	else if (is_pseudo(cleaned_line)) {
		// handle pseudo, call some function to alter dmemin.txt
		retval = PSEUDO;
	}
	else if (is_all_comment(cleaned_line)) {
		// handle comment, do nothing?
		retval = COMMENT;
	}
	else { // handle actual instruction
		parse_instruction(cleaned_line, output, labels);
		retval = REGULAR;
	}

	return retval;
}

/// <summary>
/// use this function toparse an instruction string into n instruction struct
/// </summary>
/// <param name="line"> - instruction string</param>
/// <param name="output"> - instruction struct output</param>
/// <param name="labels"> - pointer to labels "dictionary"</param>
void parse_instruction(const char* line, Instruction* output, short* labels)
{
	char* field;
	char* delim = " ,";

	// get opcode
	field = strtok(line, delim);
	output->opcode = get_op_code_from_string(field);

	// get rd
	field = strtok(NULL, delim);
	output->rd = get_register_from_string(field);

	// get rs
	field = strtok(NULL, delim);
	output->rs = get_register_from_string(field);

	// get rt
	field = strtok(NULL, delim);
	output->rt = get_register_from_string(field);

	// get rm
	field = strtok(NULL, delim);
	output->rm = get_register_from_string(field);

	// get imm1
	field = strtok(NULL, delim);
	output->immediate1 = get_numeric_value(field, labels);

	// get imm2
	field = strtok(NULL, delim);
	output->immediate2 = get_numeric_value(field, labels);
}

/// <summary>
/// receives an insturction input, encodes instruction into hexadecimal representation
/// </summary>
/// <param name="input"> - Instruction struct to be encoded</param>
/// <param name="output"> - encoded instruction</param>
/// <returns>1 on success, 0 on failure</returns>
int encode_instruction(Instruction* input, char* output) {
	return 0;
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
/// 
/// </summary>
/// <param name="input"></param>
/// <param name="output"></param>
void string_to_lower(const char* input, char* output) {
	char c;
	int i = 0;
	while ((c = input[i]) != '\0')
	{
		output[i] = tolower(input[i]);
		i++;
	}
	output[i] = '\0';
}

BOOL is_label(const char* line) {
	int i = 0;
	char c;
	while ((c = line[i]) != '\0')
	{
		if (c == ':')
			return TRUE;
		i++;
	}
	return FALSE;
}

BOOL is_pseudo(const char* line) {
	return (line[0] == '.');
}

BOOL is_all_comment(const char* line) {
	return (line[0] == '#');
}

OpCode get_op_code_from_string(const char* opcode)
{
	string_to_lower(opcode, opcode);
	if (opcode == "add")
		return ADD;

	else if (opcode == "sub")
		return SUB;

	else if (opcode == "mac")
		return MAC;

	else if (opcode == "and")
		return AND;

	else if (opcode == "or")
		return OR;

	else if (opcode == "xor")
		return XOR;

	else if (opcode == "sll")
		return SLL;

	else if (opcode == "sra")
		return SRA;

	else if (opcode == "srl")
		return SRL;

	else if (opcode == "beq")
		return BEQ;

	else if (opcode == "bne")
		return BNE;

	else if (opcode == "blt")
		return BLT;

	else if (opcode == "bgt")
		return BGT;

	else if (opcode == "ble")
		return BLE;

	else if (opcode == "bge")
		return BGE;

	else if (opcode == "jal")
		return JAL;

	else if (opcode == "lw")
		return LW;

	else if (opcode == "sw")
		return SW;

	else if (opcode == "reti")
		return RETI;

	else if (opcode == "in")
		return IN;

	else if (opcode == "out")
		return OUT;

	else if (opcode == "halt")
		return HALT;
}

Register get_register_from_string(const char* reg) {
	string_to_lower(reg, reg);
	if (reg == "$zero")
		return ZERO;

	else if (reg == "$immm1")
		return IMM1;

	else if (reg == "$imm2")
		return IMM2;

	else if (reg == "$v0")
		return V0;

	else if (reg == "$a0")
		return A0;

	else if (reg == "$a1")
		return A1;

	else if (reg == "$a2")
		return A2;

	else if (reg == "$t0")
		return T0;

	else if (reg == "$t1")
		return T1;

	else if (reg == "$t2")
		return T2;

	else if (reg == "$s0")
		return S0;

	else if (reg == "$s1")
		return S1;

	else if (reg == "$s2")
		return S2;

	else if (reg == "$gp")
		return GP;

	else if (reg == "$sp")
		return SP;

	else if (reg == "$ra")
		return RA;
}

int get_numeric_value(const char* field, short* labels) {
	char temp[LINE_MAX_LENGTH_IN_BYTES];
	int retval;
	string_to_lower(field, temp);
	if (temp[0] == '0' && temp[1] == 'x') // hex
	{
		sscanf(temp+2, "%x", &retval);
	}
	else // decimal
	{
		retval =  atoi(temp);
	}
	return retval;
}