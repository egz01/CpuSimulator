#ifndef SIM_FUNCTIONS
#define SIM_FUNCTIONS
#define _CRT_SECURE_NO_WARNINGS
#include <definitions.h>
#include <structs.h>
#include <stdlib.h>
#include <stdio.h>

void parse_instruction(INSTRUCTION_TYPE instruction_bytes, Instruction* inst);
void load_instruction_bytes(FILE*, INSTRUCTION_TYPE*);
void load_data_bytes(FILE*, DATA_TYPE*);
INSTRUCTION_TYPE get_numeric_value_from_hex_instruction(const char* field);
DATA_TYPE get_numeric_value_from_hex_data(const char* field);
int read_line(FILE* input, char* line);
void execute(Instruction* instruction, unsigned short* PC, int* registers, int* IOregisters, DATA_TYPE* memory, BOOL* halt, BOOL* in_interrupt);
unsigned long long int* load_irq2_cycles(FILE* input);

#endif