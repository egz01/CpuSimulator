// Assembler's main function - entry point of the program
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <structs.h>
#include <definitions.h>
#include "assembler_functions.h"

int main(int argc, char* argv[])
{
	char line[LINE_MAX_LENGTH_IN_BYTES];
	char cleaned_line[LINE_MAX_LENGTH_IN_BYTES];

	Instruction sInstruction;

	FILE* fProgram = fopen(argv[1], "r");
	FILE* fImemin = fopen(argv[2], "w");
	FILE* fDmemin = fopen(argv[3], "w");
	int instruction_counter = 0;
	LineType instType = 0;
	int line_length;
	char* temp_label;
	char* sLabelAddresses[4096] = {0};
	
	int i = 0;
	while ((line_length = read_line(fProgram, line)) > 0) {
		BOOL inline_label = 0;
		temp_label = parse_label(line, cleaned_line, &inline_label);
		if (temp_label) { // this line is a label, doesn't count as instruction
			sLabelAddresses[instruction_counter] = temp_label;
			instruction_counter += (int)inline_label;
		}
		else if (is_pseudo(cleaned_line))
		{
			handle_pseudo(cleaned_line, fDmemin);
		}
		else // actual instruction
			instruction_counter++;
	}
	
	fseek(fProgram, 0, 0);

	while (line_length = read_line(fProgram, line) > 0) {
		instType = parse_line(line, &sInstruction, sLabelAddresses);

		switch (instType)
		{
		case REGULAR:
			encode_instruction(&sInstruction, fImemin);
			break;
		case PSEUDO:
			break;
		case LABEL:
			break;
		}
	}
	fflush(fImemin);

	// clean up dynamic memory
	for (int i = 0; i < INSTRUCTIONS_DEPTH; i++)
		free(sLabelAddresses[i]);

	fclose(fImemin);
	fclose(fDmemin);
	fclose(fProgram);

	return 0;
}