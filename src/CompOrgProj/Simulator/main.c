// Simulator's main function - entry point of the program.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <structs.h>
#include <definitions.h>
#include "simulator_functions.h"

#define DEBUG

#ifdef DEBUG
#define log printf
#else
#define log
#endif

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
    FILE* monitor_yuv = fopen(argv[14], "wb");

    long long int* irq2cycles = NULL;
    irq2cycles = load_irq2_cycles(irq2in);

#ifdef TEST
    while ((*irq2cycles) != 0)
    {
        printf("%lld\n", *irq2cycles);
        irq2cycles++;
    }
#endif

    INSTRUCTION_TYPE* instructions_memory = (INSTRUCTION_TYPE*)malloc(sizeof(INSTRUCTION_TYPE) * INSTRUCTIONS_DEPTH);
    DATA_TYPE* data_memory = (DATA_TYPE*)malloc(sizeof(DATA_TYPE) * MEMORY_DEPTH);
    DATA_TYPE* screen_buffer = (DATA_TYPE*)calloc(SCREEN_X * SCREEN_Y, sizeof(DATA_TYPE));

    Instruction current;
    DATA_TYPE registers[NUM_REGISTERS] = { 0 };
    DATA_TYPE IOregisters[NUM_IOREGISTERS] = { 0 };
    unsigned short PC = 0;
    BOOL halt = FALSE;
    BOOL irq = 0;
    BOOL in_interrupt = 0;
    long long int cycles_counter = 0;
    unsigned int timer_counter;

    load_instruction_bytes(imemin, instructions_memory);
    load_data_bytes(dmemin, data_memory);

    while (!halt && PC < INSTRUCTIONS_DEPTH)
    {
        log("executing instruction: %x\r\n", PC);

        // fetch and decode:
        parse_instruction(instructions_memory[PC], &current);

        // hard-wired values:
        registers[ZERO] = 0;
        registers[IMM1] = current.immediate1;
        registers[IMM2] = current.immediate2;

        update_trace(PC, instructions_memory[PC], registers, trace);

        execute(&current, &PC, registers, IOregisters, data_memory, &halt, &in_interrupt);

        // timer handling
        if (IOregisters[TIMERENABLE])
        {
            if (IOregisters[TIMERCURRENT] == IOregisters[TIMERMAX])
            {
                IOregisters[TIMERCURRENT] = 0;
                IOregisters[IRQ0STATUS] = 1;
            }
            else
                IOregisters[TIMERCURRENT]++;
        }

        if (IOregisters[MONITORCMD])
        {
            screen_buffer[IOregisters[MONITORADDR]] = IOregisters[MONITORDATA];
        }

        if (irq2triggered(cycles_counter, irq2cycles))
        {
            IOregisters[IRQ2STATUS] = 1;
        }

        // TODO: update disk

        irq = (IOregisters[IRQ0ENABLE] && IOregisters[IRQ0STATUS] ||
               IOregisters[IRQ1ENABLE] && IOregisters[IRQ1STATUS] ||
               IOregisters[IRQ2ENABLE] && IOregisters[IRQ2STATUS]);

        if (irq)
        {
            if (!in_interrupt)
            {
                in_interrupt = TRUE;
                IOregisters[IRQRETURN] = PC;
                PC = IOregisters[IRQHANDLER];
            }
            else
            {
                // do nothing
            }
        }

        if (current.opcode == OUT || current.opcode == IN)
            update_hwtrace(cycles_counter, current.opcode, registers[current.rs] + registers[current.rt], IOregisters[registers[current.rs] + registers[current.rt]], hwregtrace);
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
    free(irq2cycles);
}