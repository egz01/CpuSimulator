// Assembler's main function - entry point of the program
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "structs.h"
#include "definitions.h"
#include "assembler_functions.h"

int main(int argc, char* argv[])
{
#define TEST "circle"
#undef TEST
#if defined(TEST)
	const char* directory = "..\\..\\..\\test_programs";
	sprintf(argv[1], "%s\\%s\\%s.asm", directory, TEST, TEST);
	sprintf(argv[2], "%s\\%s\\%s", directory, TEST, "imemin.txt");
	sprintf(argv[3], "%s\\%s\\%s", directory, TEST, "dmemin.txt");
#endif

	char line[LINE_MAX_LENGTH_IN_BYTES];
	char cleaned_line[LINE_MAX_LENGTH_IN_BYTES];

	Instruction sInstruction;

	FILE* fProgram = fopen(argv[1], "r");
	FILE* fImemin = fopen(argv[2], "w");
	FILE* fDmemin = fopen(argv[3], "w");
	int instruction_counter = 0;
	LineType instType = 0;
	char* temp_label;
	char* sLabelAddresses[INSTRUCTIONS_DEPTH] = {0};
	int* data_memory = (int*)calloc(MEMORY_DEPTH, sizeof(int));
	
	// create a label list
	int i = 0;
	while (!feof(fProgram)) {
		if (read_line(fProgram, line) == 0) continue; // empty line, not interesting

		BOOL inline_label = 0;
		temp_label = parse_label(line, cleaned_line, &inline_label);
		if (temp_label) { // this line is a label, doesn't count as instruction
			sLabelAddresses[instruction_counter] = temp_label;
			instruction_counter += (int)inline_label;
		}
		else if (is_pseudo(cleaned_line))
		{
			handle_pseudo(cleaned_line, data_memory);
		}
		else {
			// actual instruction
			instruction_counter++;
		}
	}
	
	fseek(fProgram, 0, 0);
	int line_counter = 1;
	while (!feof(fProgram)) {
		if (read_line(fProgram, line) == 0) continue; // empty line, not interesting

		instType = parse_line(line, &sInstruction, sLabelAddresses);

		switch (instType)
		{
		case REGULAR:
			encode_instruction(&sInstruction, fImemin);
			line_counter++;
		case PSEUDO:
		case LABEL:
		case COMMENT:
			break;
		}
	}
	fflush(fImemin);

	int depth = find_dmemory_index(data_memory, MEMORY_DEPTH);
	dump_data(fDmemin, data_memory, depth + 1);

	// clean up dynamic memory
	for (int i = 0; i < INSTRUCTIONS_DEPTH; i++)
		free(sLabelAddresses[i]);

	fclose(fImemin);
	fclose(fDmemin);
	fclose(fProgram);

	return 0;
}