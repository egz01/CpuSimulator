#ifndef _FUNCTIONS
#define _FUNCTIONS

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "structs.h"
#include "definitions.h"

void edit_memory(int address, int value, FILE* dmemin);
int read_line(FILE* input, char* line);
LineType parse_line(char* line, Instruction* output, char* labels[INSTRUCTIONS_DEPTH]);
int encode_instruction(Instruction* input, FILE* imemin);
int remove_extra_spaces_and_tabs(const char* to_fix, char* fixed);
OpCode get_op_code_from_string(const char* opcode);
Register get_register_from_string(const char* field);
void string_to_lower(const char* input, char* output);
int get_numeric_value(const char* field, char* labels[INSTRUCTIONS_DEPTH]);
void parse_instruction(const char* line, Instruction* output, char* labels[INSTRUCTIONS_DEPTH]);
char* parse_label(char* line, char* cleaned_line, BOOL* inline_label);
void handle_pseudo(const char* line, int* memory_data);
void dump_data(FILE* output_stream, DATA_TYPE* output_data, int size);
int find_dmemory_index(DATA_TYPE* data_memory, int max_size);

BOOL is_label(const char* line);
BOOL is_pseudo(const char* line);
BOOL is_all_comment(const char* line);
#endif