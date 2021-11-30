// Assembler's main function - entry point of the program
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "functions.h"
#include "structs.h"
#include "definitions.h"

int main(int argc, char* argv[])
{
#define TEST
#ifdef TEST
	char output[LINE_MAX_LENGTH_IN_BYTES];
	char* input = "\t      add     $t1, $t2,  $t3\t, 0, 0    ";
	remove_extra_spaces_and_tabs(input, output);
	printf("\"%s\" -> \"%s\"\n", input, output);

	char* input2 = "TeSt+++12312";
	char output2[LINE_MAX_LENGTH_IN_BYTES];
	string_to_lower(input2, output2);
	printf("\"%s\" -> \"%s\"\n", input2, output2);
	return 0;
#endif

	char line[LINE_MAX_LENGTH_IN_BYTES];
	char hex[INSTRUCTION_SIZE_IN_BITS / 8 + 1];
	Instruction sInstruction;

	FILE* fProgram = fopen(argv[1], "r");
	FILE* fImemin = fopen(argv[2], "w");
	FILE* fDmemin = fopen(argv[3], "w");
	int line_counter = 0;
	LineType instType = 0;
	int line_length;

	while (line_length = read_line(fProgram, line) > 0)
	{
		instType = parse_line(line, line_length, &sInstruction);

		switch (instType)\
		{
		case REGULAR:
			encode_instruction(&sInstruction, hex);
			line_counter += 1;
			break;
		case PSEUDO:
			break;
		case LABEL:
			break;
		}
	}
	return 0;
}