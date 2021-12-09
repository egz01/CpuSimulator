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
    FILE* imemin = open(argv[1], "r");
    FILE* dmemin = open(argv[2], "r");
    FILE* diskin = open(argv[3], "r");
    FILE* irq2in = open(argv[4], "r");

    // open output files
    FILE* dmemout     = open(argv[5], "w");
    FILE* regout      = open(argv[6], "w");
    FILE* trace       = open(argv[7], "w");
    FILE* hwregtrace  = open(argv[8], "w");
    FILE* cycles      = open(argv[9], "w");
    FILE* leds        = open(argv[10], "w");
    FILE* display7seg = open(argv[11], "w");
    FILE* diskout     = open(argv[12], "w");
    FILE* monitor     = open(argv[13], "w");
    FILE* monitor_yuv = open(argv[14], "w");

    int registers[NUM_REGISTERS] = { 0 };
    int HWregisters[NUM_IOREGISTERS] = { 0 };
    unsigned short PC = 0;
    INSTRUCTION* instructions_memory = (INSTRUCTION*)malloc(sizeof(INSTRUCTION) * INSTRUCTIONS_DEPTH);
    INSTRUCTION* data_memory = (unsigned int*)malloc(sizeof(unsigned int) * MEMORY_DEPTH);
    
    Instruction current;

    BOOL halt = FALSE;
    int cycles_counter = 0;

    for (int i = 0; i++; i < INSTRUCTIONS_DEPTH)
    {
        // instructions_memory[i] = parse line from imemin.txt
        //data_memory[i] = parse line from dmemin.txt
    }

    parse_instruction(0x140021001002, &current);

    /*
    while (!halt)
    {
        if (cycles_counter in irq2list):
            enable irq2

        fetch();
        decode();
        execute();
        
        irq = (irq0enable & irq0status) | (irq1enable & irq1status) | (irq2enable & irq2status)

        if (irq && not_in_interrupt())
        {
            
;       }
    }
    */

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