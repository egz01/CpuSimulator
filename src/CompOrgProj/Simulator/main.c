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

    INSTRUCTION_TYPE* instructions_memory = (INSTRUCTION_TYPE*)malloc(sizeof(INSTRUCTION_TYPE) * INSTRUCTIONS_DEPTH);
    DATA_TYPE* data_memory = (DATA_TYPE*)calloc(MEMORY_DEPTH, sizeof(DATA_TYPE));
    char* screen_buffer = (char*)calloc(SCREEN_X * SCREEN_Y, sizeof(char));
    DATA_TYPE* disk = (DATA_TYPE*)calloc(DISK_SECTORS*SECTOR_SIZE_IN_BYTES, sizeof(DATA_TYPE));

    load_data_bytes(diskin, disk);
    load_instruction_bytes(imemin, instructions_memory);
    load_data_bytes(dmemin, data_memory);

    Instruction current;
    DATA_TYPE registers[NUM_REGISTERS] = { 0 };
    DATA_TYPE IOregisters[NUM_IOREGISTERS] = { 0 };
    unsigned short PC = 0;
    BOOL halt = FALSE;
    BOOL irq = 0;
    BOOL in_interrupt = 0;
    long long int cycles_counter = 0;
    unsigned int timer_counter;

    while (!halt && PC < INSTRUCTIONS_DEPTH)
    {
        log("executing instruction: %x\r\n", PC);

        // fetch & decode instruction
        parse_instruction(instructions_memory[PC], &current);

        // hard-wired values:
        registers[ZERO] = 0;
        registers[IMM1] = current.immediate1;
        registers[IMM2] = current.immediate2;

        update_trace(PC, instructions_memory[PC], registers, trace);

        // execute instruction
        execute(&current, &PC, registers, IOregisters, data_memory, &halt, &in_interrupt);

        // hardware tracing
        if (current.opcode == OUT || current.opcode == IN)
        {
            update_hwtrace(cycles_counter, current.opcode, registers[current.rs] + registers[current.rt],
                IOregisters[registers[current.rs] + registers[current.rt]], hwregtrace);
        }

        // timer logic
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

        // disk logic (irq1)


        // external line interrupt (irq2)
        if (irq2triggered(cycles_counter, irq2cycles))
        {
            IOregisters[IRQ2STATUS] = 1;
        }

        // monitor logic
        if (IOregisters[MONITORCMD])
        {
            screen_buffer[IOregisters[MONITORADDR]] = IOregisters[MONITORDATA];
            IOregisters[MONITORCMD] = 0; // handled monitor updating, toggle command bit
        }

        // interrupt polling
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
                // do nothing, wait for in_interrupt to toggle
            }
        }

        // leds and display7seg
        int reg = registers[current.rs] + registers[current.rt];
        if (current.opcode == OUT && (reg == LEDS || reg == DISPLAY7SEG))
        {
            if (reg == LEDS)
                fprintf(leds, "%ld %08x\n", cycles_counter, IOregisters[reg]);
            else
                fprintf(display7seg, "%ld %08x\n", cycles_counter, IOregisters[reg]);
        }

        // advance in cycles counting
        cycles_counter++;
    }
    
    int memory_depth = find_dmemory_index(data_memory);

    dump_data(dmemout, data_memory, memory_depth + 1);           // dump to dmemout.txt
    dump_data(regout, registers + 3, NUM_REGISTERS - 3);         // dump to regout.txt
    fprintf(cycles, "%ld\n", cycles_counter);                    // dump to cycles.txt
    dump_data(diskout, disk, DISK_SECTORS*SECTOR_SIZE_IN_BYTES); // dump to diskout.txt
    dump_pixels_string(monitor, screen_buffer);                  // dump to monitor.txt
    dump_pixels_binary(monitor_yuv, screen_buffer);              // dump to monitor.yuv

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