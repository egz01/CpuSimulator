// Simulator's main function - entry point of the program.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(int argc, char* argv[])
{
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

}