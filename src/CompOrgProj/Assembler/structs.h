#ifndef _STRUCTS
#define _STRUCTS

typedef struct instruction_struct
{
	char opcode;
	char rd;
	char rs;
	char rt;
	char rm;
	short immediate1;
	short immediate2;

}Instruction_struct;

#endif