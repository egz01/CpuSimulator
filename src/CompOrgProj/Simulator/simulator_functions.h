#ifndef SIM_FUNCTIONS
#define SIM_FUNCTIONS
#define _CRT_SECURE_NO_WARNINGS
#include <definitions.h>
#include <structs.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void parse_instruction(INSTRUCTION_TYPE instruction_bytes, Instruction* inst);
void load_instruction_bytes(FILE*, INSTRUCTION_TYPE*);
void load_data_bytes(FILE*, DATA_TYPE*);
INSTRUCTION_TYPE get_numeric_value_from_hex_instruction(const char* field);
DATA_TYPE get_numeric_value_from_hex_data(const char* field);
int read_line(FILE* input, char* line);
void execute(Instruction* instruction, unsigned short* PC, int* registers, int* IOregisters, DATA_TYPE* memory, BOOL* halt, BOOL* in_interrupt);
long long int* load_irq2_cycles(FILE* input);
void update_trace(int PC, INSTRUCTION_TYPE instruction, DATA_TYPE* registers, FILE* output);
void update_hwtrace(long long int cycle, OpCode code, IOHWRegister name, DATA_TYPE data, FILE* output);
BOOL irq2triggered(long long cycle_num, long long* irq2cycles);
void get_string_from_opcode(IOHWRegister code, char str[]);
void dump_data(FILE* output_stream, DATA_TYPE* output_data, int size);
void dump_byte_data(FILE* output_stream, char* output_data);
void dump_pixels_string(FILE* output_stream, char* output_data);
void dump_pixels_binary(FILE* binary_stream, char* output_data);
int find_dmemory_index(DATA_TYPE* data_memory);

#endif