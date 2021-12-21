#include "assembler_functions.h"

/// <summary>
/// used to alter data to value in memory in given addres
/// </summary>
/// <param name="address"> - address to alter in memory</param>
/// <param name="value"> - value to edit memory to</param>
/// <param name="dmemin"> - output stream</param>
void edit_memory(int address, int value, FILE* dmemin) {
	char empty_cell[MEMORY_SIZE_IN_CHARS + 1] = "00000000";
	char hex[MEMORY_SIZE_IN_CHARS + 1];
	int line_counter = 0;
	BOOL reached = 0;

	sprintf(hex, "%X", value);
	int to_pad = MEMORY_SIZE_IN_CHARS - strlen(hex);

	if (to_pad)
	{
		memcpy(empty_cell + to_pad, hex, MEMORY_SIZE_IN_CHARS - to_pad + 1);
		memcpy(hex, empty_cell, MEMORY_SIZE_IN_CHARS + 1);
		memcpy(empty_cell, "00000000", MEMORY_SIZE_IN_CHARS);
	}

	while (line_counter < MEMORY_DEPTH && !reached)
	{
		if (line_counter == address)
		{
			fwrite(hex, 1, strlen(hex), dmemin);
			fwrite("\n", 1, strlen("\n"), dmemin);
			reached = 1; // maybe skip this, and then output a 4096 memory cells long dmemin.txt
		}
		else
		{
			fwrite(empty_cell, 1, strlen(empty_cell), dmemin);
			fwrite("\n", 1, strlen("\n"), dmemin);
		}
		line_counter++;
	}
}

/// <summary>
/// used to handle .word insturctions
/// </summary>
/// <param name="line"> - a stripped line containing a .word instruction</param>
/// <param name="dmemin"> - stream open for writing</param>
void handle_pseudo(const char* line, FILE* dmemin) {
	char* field;
	char* delim = " ";
	Word instruction;

	// skip instruction (known to be .word at this point)
	field = strtok(line, delim);

	field = strtok(NULL, delim);
	instruction.address = get_numeric_value(field, NULL);

	field = strtok(NULL, delim);
	instruction.value = get_numeric_value(field, NULL);

	edit_memory(instruction.address, instruction.value, dmemin);
}

/// <summary>
/// fixes trailing, leading, inbetweening whitespaces and tabs in given string
/// </summary>
/// <param name="to_fix">input string with various extra spaces and tabs</param>
/// <param name="fixed">output string (should be at least of equal size to input string) with no extra whitespaces or tabs</param>
int remove_extra_spaces_and_tabs(char* to_fix, char* fixed)
{
	char c;
	int i = 0;
	int j = 0;
	int spaced = 0;
	int first = 1;
	char temp[LINE_MAX_LENGTH_IN_BYTES];
	temp[LINE_MAX_LENGTH_IN_BYTES - 1] = '\0';
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

	return j;
}

/// <summary>
/// use this function to parse an input line into an instruction struct.
/// if the given line doesn't contain an instruction (i.e. pseudo / 
/// </summary>
/// <param name="instruction"> - instruction string as read from file</param>
/// <param name="output"> - output insturction struct</param>
/// <returns>type of line received, i.e. {regular instruction, pseudo instruction, label}</returns>
LineType parse_line(char* line, Instruction* output, char* labels[INSTRUCTIONS_DEPTH]) {
	LineType retval = REGULAR;
	char cleaned_line[LINE_MAX_LENGTH_IN_BYTES];
	int cleaned_length = remove_extra_spaces_and_tabs(line, cleaned_line);

	if (cleaned_length == 0)
		retval = COMMENT;
	else if (is_label(cleaned_line)) {
		// do nothing
		// check if inline label:
		strtok(cleaned_line, ":");
		char* maybe_inst = strtok(NULL, ":");
		if (maybe_inst)
			retval = parse_line(maybe_inst, output, labels);
		else
			retval = LABEL;
	}
	else if (is_pseudo(cleaned_line)) {
		// already handled
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
/// use this function to parse an instruction string into an instruction struct
/// </summary>
/// <param name="line"> - instruction string</param>
/// <param name="output"> - instruction struct output</param>
/// <param name="labels"> - pointer to labels "dictionary"</param>
void parse_instruction(const char* line, Instruction* output, char* labels[INSTRUCTIONS_DEPTH])
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
/// returns a malloced string - has to be freed!!!
/// </summary>
/// <param name="line">input raw line</param>
/// <param name="cleaned_line">outputs a byproduct cleaned line, if necessary for pseudo instructions</param>
/// <returns>pointer to new malloced string containing label string</returns>
char* parse_label(char* line, char* cleaned_line, BOOL* inline_label)
{
	int label_length = 0;
	//char cleaned_line[LINE_MAX_LENGTH_IN_BYTES];
	char* npLabel = NULL;
	remove_extra_spaces_and_tabs(line, cleaned_line);
	if (is_label(cleaned_line)) {
		char* p = strtok(cleaned_line, ":");
		label_length = strlen(p);
		npLabel = malloc((label_length+1) * sizeof(char));
		strncpy(npLabel, p, label_length+1);
		string_to_lower(npLabel, npLabel);

		p = strtok(NULL, ":");
		if (p) { // have something after label
			remove_extra_spaces_and_tabs(p, cleaned_line);
			if (!is_all_comment(cleaned_line))
				*inline_label = TRUE;
		}
	}
	return npLabel;
}

/// <summary>
/// receives an insturction input, encodes instruction into hexadecimal representation
/// </summary>
/// <param name="input"> - Instruction struct to be encoded</param>
/// <param name="output"> - encoded instruction</param>
/// <returns>1 on success, 0 on failure</returns>
int encode_instruction(Instruction* input, FILE* imemin) {
	INSTRUCTION_TYPE num = 0;
	int bit_index = INSTRUCTION_SIZE_IN_BITS;
	char leading_zeros[INSTRUCTION_SIZE_IN_CHARS + 1] = "000000000000";
	char hex[INSTRUCTION_SIZE_IN_CHARS + 1];
	
	bit_index -= OPCODE_BITS;
	num |= ((INSTRUCTION_TYPE)(input->opcode) << bit_index);
	
	bit_index -= RD_BITS;
	num |= ((INSTRUCTION_TYPE)(input->rd) << bit_index);

	bit_index -= RS_BITS;
	num |= ((INSTRUCTION_TYPE)(input->rs) << bit_index);
	
	bit_index -= RT_BITS;
	num |= ((INSTRUCTION_TYPE)(input->rt) << bit_index);

	bit_index -= RM_BITS;
	num |= ((INSTRUCTION_TYPE)(input->rm) << bit_index);

	bit_index -= IMM1_BITS;
	num |= (((INSTRUCTION_TYPE)(input->immediate1) << bit_index) & ((INSTRUCTION_TYPE)0xfff << bit_index)); // account for sign extension
	
	bit_index -= IMM2_BITS;
	num |= (((INSTRUCTION_TYPE)(input->immediate2) << bit_index) & ((INSTRUCTION_TYPE)0xfff << bit_index)); // account for sign extension

	sprintf(hex, "%llX", num);
	int to_pad = INSTRUCTION_SIZE_IN_CHARS - strlen(hex);

	if (to_pad)
	{
		memcpy(leading_zeros + to_pad, hex, INSTRUCTION_SIZE_IN_CHARS - to_pad);
		fwrite(leading_zeros, 1, INSTRUCTION_SIZE_IN_CHARS, imemin);
	}
	else
		fwrite(hex, 1, INSTRUCTION_SIZE_IN_CHARS, imemin);

	fwrite("\n", 1, strlen("\n"), imemin);

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
/// copies the data in input to output while altering every ascii char to lowercase
/// </summary>
/// <param name="input"> - any alphanumerical text</param>
/// <param name="output"> - same text, every ascii in input will be lowercase</param>
void string_to_lower(const char* input, char* output) {
	char c;
	int i = 0;
	if (input)
	{
		while ((c = input[i]) != '\0')
		{
			output[i] = tolower(input[i]);
			i++;
		}
		output[i] = '\0';
	}
}

/// <summary>
/// return TRUE if given line is a label
/// </summary>
/// <param name="line"></param>
/// <returns></returns>
BOOL is_label(const char* line) {
	BOOL retval = 0;
	char c;
	while ((c = line[0]) != '\0' && !retval)
	{
		if (c == '#')
			break;

		retval = c == ':';
		line++;
	}
	return retval;
}

/// <summary>
/// returns TRUE if given line is a pseudo instruction
/// </summary>
/// <param name="line"></param>
/// <returns></returns>
BOOL is_pseudo(const char* line) {
	BOOL retval = 0;
	char c;
	while ((c = line[0]) != '\0' && !retval)
	{
		if (c == '#')
			break;

		retval = c == '.';
		line++;
	}
	return retval;
}

/// <summary>
/// returns TRUE if the whole line is a comment (no instruction)
/// </summary>
/// <param name="line"></param>
/// <returns></returns>
BOOL is_all_comment(const char* line) {
	return (line[0] == '#');
}

/// <summary>
/// returns an enum value according to string
/// </summary>
/// <param name="opcode"></param>
/// <returns></returns>
OpCode get_op_code_from_string(const char* opcode)
{
	string_to_lower(opcode, opcode);
	if (strcmp(opcode, "add") == 0)
		return ADD;

	else if (strcmp(opcode, "sub") == 0)
		return SUB;

	else if (strcmp(opcode, "mac") == 0)
		return MAC;

	else if (strcmp(opcode, "and") == 0)
		return AND;

	else if (strcmp(opcode, "or") == 0)
		return OR;

	else if (strcmp(opcode, "xor") == 0)
		return XOR;

	else if (strcmp(opcode, "sll") == 0)
		return SLL;

	else if (strcmp(opcode, "sra") == 0)
		return SRA;

	else if (strcmp(opcode, "srl") == 0)
		return SRL;

	else if (strcmp(opcode, "beq") == 0)
		return BEQ;

	else if (strcmp(opcode, "bne") == 0)
		return BNE;

	else if (strcmp(opcode, "blt") == 0)
		return BLT;

	else if (strcmp(opcode, "bgt") == 0)
		return BGT;

	else if (strcmp(opcode, "ble") == 0)
		return BLE;

	else if (strcmp(opcode, "bge") == 0)
		return BGE;

	else if (strcmp(opcode, "jal") == 0)
		return JAL;

	else if (strcmp(opcode, "lw") == 0)
		return LW;

	else if (strcmp(opcode, "sw") == 0)
		return SW;

	else if (strcmp(opcode, "reti") == 0)
		return RETI;

	else if (strcmp(opcode, "in") == 0)
		return IN;

	else if (strcmp(opcode, "out") == 0)
		return OUT;

	else if (strcmp(opcode, "halt") == 0)
		return HALT;
}

/// <summary>
/// returns an enum value according to string
/// </summary>
/// <param name="reg"></param>
/// <returns></returns>
Register get_register_from_string(const char* reg) {
	string_to_lower(reg, reg);
	if (strcmp(reg, "$zero") == 0)
		return ZERO;

	else if (strcmp(reg, "$imm1") == 0)
		return IMM1;

	else if (strcmp(reg, "$imm2") == 0)
		return IMM2;

	else if (strcmp(reg, "$v0") == 0)
		return V0;

	else if (strcmp(reg, "$a0") == 0)
		return A0;

	else if (strcmp(reg, "$a1") == 0)
		return A1;

	else if (strcmp(reg, "$a2") == 0)
		return A2;

	else if (strcmp(reg, "$t0") == 0)
		return T0;

	else if (strcmp(reg, "$t1") == 0)
		return T1;

	else if (strcmp(reg, "$t2") == 0)
		return T2;

	else if (strcmp(reg, "$s0") == 0)
		return S0;

	else if (strcmp(reg, "$s1") == 0)
		return S1;

	else if (strcmp(reg, "$s2") == 0)
		return S2;

	else if (strcmp(reg, "$gp") == 0)
		return GP;

	else if (strcmp(reg, "$sp") == 0)
		return SP;

	else if (strcmp(reg, "$ra") == 0)
		return RA;
}

/// <summary>
/// returns a numeric value inside field, using labels as a map to labels indices
/// </summary>
/// <param name="field"> - a stripped field containing either a hexadecimal number, a decimal number or a label</param>
/// <param name="labels"> - labels map</param>
/// <returns>a numeric value hidden in field</returns>
int get_numeric_value(const char* field, char* labels[INSTRUCTIONS_DEPTH]) {
	char temp[LINE_MAX_LENGTH_IN_BYTES];
	char* ptr;
	int retval;
	string_to_lower(field, temp);
	if (temp[0] == '0' && temp[1] == 'x') // hex
	{
		sscanf(temp+2, "%x", &retval);
	}
	else if ('a' <= temp[0] && temp[0] <= 'z' && labels) // label
	{
		int i = 0;
		for (; i < INSTRUCTIONS_DEPTH; i++)
		{
			ptr = labels[i];
			if (ptr && strcmp(ptr, temp) == 0)
				break;
		}
		retval = i;
	}
	else // decimal
	{
		retval =  atoi(temp);
	}
	return retval;
}