// Assembler's main function - entry point of the program
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "functions.h"
#include "structs.h"
#include "definitions.h"

int main(int argc, char* argv[])
{
//#define TEST
#ifdef TEST
	edit_memory(256, 7, NULL);


	if (is_pseudo("     .test."))
		printf("pseudo\n");
	char output[LINE_MAX_LENGTH_IN_BYTES];
	char* input = "\t      add     $t1, $t2,  $t3\t, 0, 0    ";
	remove_extra_spaces_and_tabs(input, output);
	printf("\"%s\" -> \"%s\"\n", input, output);

	char* input2 = "TeSt+++12312";
	char output2[LINE_MAX_LENGTH_IN_BYTES];
	string_to_lower(input2, output2);
	printf("\"%s\" -> \"%s\"\n", input2, output2);

	const char* input3 = "0xABC";
	int output3 = get_numeric_value(input3, NULL);
	printf("\"%s\" -> %d\n", input3, output3);

	const char* input4 = "0xFFFF";
	int output4 = get_numeric_value(input4, NULL);
	printf("\"%s\" -> %d\n", input4, output4);

	const char* input5 = "1234";
	int output5 = get_numeric_value(input5, NULL);
	printf("\"%s\" -> %d\n", input5, output5);

	char* c = "daniel";
	char temp[500];
	c += 3;
	strncpy(temp, c, strlen(c));
	temp[1] = '\0';
	printf("%s", temp);
	return 0;
#endif

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
		temp_label = parse_label(line, cleaned_line);
		if (temp_label) // this line is a label, doesn't count as instruction
			sLabelAddresses[instruction_counter] = temp_label;
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

	for (int i = 0; i < INSTRUCTIONS_DEPTH; i++)
		free(sLabelAddresses[i]);

	fclose(fImemin);
	fclose(fDmemin);
	fclose(fProgram);

	return 0;
}