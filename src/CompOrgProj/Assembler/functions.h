#ifndef _FUNCTIONS
#define _FUNCTIONS

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "structs.h"
#include "definitions.h"

void copy_temp();
void word(int address, long data, FILE* dmemin);
int read_line(FILE* input, char* line);
int parse_line(char* line, int length, Instruction* output);
int encode_instruction(Instruction* input, char* output);
void remove_extra_spaces_and_tabs(const char* to_fix, char* fixed);
OpCode get_op_code_from_string(const char* opcode);
void string_to_lower(const char* input, char* output);

BOOL is_label(const char* line)
{
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
BOOL is_pseudo(const char* line) { return (line[0] == '.'); }
BOOL is_comment(const char* line) { return (line[0] == '#'); }

#endif