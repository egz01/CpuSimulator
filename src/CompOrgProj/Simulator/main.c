// Simulator's main function - entry point of the program.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <structs.h>
#include <definitions.h>
#include "simulator_functions.h"

int main(int argc, char* argv[])
{
    // BOOT 
    // open input files
    FILE* imemin = fopen(argv[1], "r");
    FILE* dmemin = fopen(argv[2], "r");
    FILE* diskin = fopen(argv[3], "r");
    FILE* irq2in = fopen(argv[4], "r");

    // open output files
    FILE* dmemout     = fopen(argv[5], "w");
    FILE* regout      = fopen(argv[6], "w");
    FILE* trace       = fopen(argv[7], "w");
    FILE* hwregtrace  = fopen(argv[8], "w");
    FILE* cycles      = fopen(argv[9], "w");
    FILE* leds        = fopen(argv[10], "w");
    FILE* display7seg = fopen(argv[11], "w");
    FILE* diskout     = fopen(argv[12], "w");
    FILE* monitor     = fopen(argv[13], "w");
    FILE* monitor_yuv = fopen(argv[14], "w");

    // TODO: parse irq2in.txt into some array
    
    INSTRUCTION_TYPE* instructions_memory = (INSTRUCTION_TYPE*)malloc(sizeof(INSTRUCTION_TYPE) * INSTRUCTIONS_DEPTH);
    DATA_TYPE* data_memory = (DATA_TYPE*)malloc(sizeof(DATA_TYPE) * MEMORY_DEPTH);
    
    DATA_TYPE** arr = (DATA_TYPE**)malloc(SCREEN_X * sizeof(DATA_TYPE*));
    for (int i = 0; i < SCREEN_X; i++)
        arr[i] = (int*)calloc(SCREEN_X, sizeof(DATA_TYPE));

    Instruction current;
    DATA_TYPE registers[NUM_REGISTERS] = { 0 };
    DATA_TYPE IOregisters[NUM_IOREGISTERS] = { 0 };
    unsigned short PC = 0;
    BOOL halt = FALSE;
    unsigned long long int cycles_counter = 0;

    load_instruction_bytes(imemin, instructions_memory);
    load_data_bytes(dmemin, data_memory);

    while (!halt && PC < INSTRUCTIONS_DEPTH)
    {
        printf("executing instruction: %d\r\n", PC);

        // fetch and decode:
        parse_instruction(instructions_memory[PC++], &current);
        
        execute(&current, &PC, registers, IOregisters, data_memory, &halt);

        // TODO: advance timer

        // TODO: udpate monitor
        
        // TODO: update disk

        /*
        if (cycles_counter in irq2list):
            enable irq2

        execute();
        
        irq = (irq0enable & irq0status) | (irq1enable & irq1status) | (irq2enable & irq2status)

        if (irq && not_in_interrupt())
        {
            
        }
        */

        // TODO: update trace.txt
        // 
        if (current.opcode == OUT || current.opcode == IN);
            // TODO: update hwregtrace.txt (only if out or in!)
        cycles_counter++;
    }

    // TODO: write data_memory to dmemout.txt
    // TODO: write registers to regout.txt
    // TODO: write to cycles_counter to cycles.txt
    // TODO: write diskout.txt
    // TODO: write screen_buffer to monitor.txt
    // TODO: write screen_buffer to monitor.yuv

    fclose(imemin);
    fclose(dmemin);
    fclose(diskin);
    fclose(irq2in);
    fclose(dmemout);
    fclose(regout);
    fclose(trace);
    fclose(hwregtrace);
    fclose(cycles);
    fclose(leds);
    fclose(display7seg);
    fclose(diskout);
    fclose(monitor);
    fclose(monitor_yuv);
}