#ifndef _FUNCTIONS
#define _FUNCTIONS

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "structs.h"
#include "definitions.h"

void copy_temp();
void word(int address, long data, FILE* dmemin);
int read_line(FILE* input, char* line);
int parse_instruction(char* instruction, int length, Instruction* output);
int encode_instruction(Instruction* input, char* output);
void remove_extra_spaces_and_tabs(char* to_fix, char* fixed);

#endif