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
long long int* load_irq2_cycles(FILE* input);
void update_trace(int PC, INSTRUCTION_TYPE instruction, DATA_TYPE* registers, FILE* output);
void update_hwtrace(unsigned long long int cycle, OpCode code, IOHWRegister name, DATA_TYPE data, FILE* output);
BOOL irq2triggered(long long cycle_num, long long* irq2cycles);

#endif