// Assembler's main function - entry point of the program
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "functions.h"

int read_file(char* buffer, FILE* stream)
{
    if (stream == NULL)
        return 0;
    int eof = 0;
    int i = 0;
    char c;
    while ((c = fgetc(stream)) != EOF)
    {
        buffer[i] = c;
        i++;
    }
    buffer[i] = '\0';
    return i + 1;
}

int main(int argc, char* argv[])
{
    char buffer[1000];
    int last;
    for (int i = 0; i < argc; i++)
    {
        FILE* input = fopen(argv[i], "r");
        last = read_file(buffer, input);
        fclose(input);
        printf(buffer);
        printf("\n");
    }
}