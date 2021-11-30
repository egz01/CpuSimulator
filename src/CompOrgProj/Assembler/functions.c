#include "functions.h"

void copy_temp() {
	FILE* dmemin = fopen("dmemin.txt", "w");
	FILE* temp = fopen("temp.txt", "r");
	char copy_data[10];
	while (fgets(copy_data, 10, temp) != NULL)
	{
		fputs(copy_data, dmemin);

	}
	fclose(temp);
	fclose(dmemin);
}

void word(int address, long data, FILE* dmemin) {
	int counter = -1;
	char copy_data[10];
	FILE* temp = fopen("temp.txt", "w");
	while (fgets(copy_data, 10, dmemin) != NULL)
	{
		counter += 1;
		if (counter == address)
		{
			fputs(data, temp);
			fputs("\n", temp);
		}
		else
		{
			//printf("copydata:%s", copy_data);
			fputs(copy_data, temp);
			//fputs("\n" ,temp);
		}
	}
	fclose(dmemin);
	if (counter < address)
	{
		if (counter != -1) {
			fputs("\n", temp);
		}
		counter += 1;
		while (counter < address)
		{
			fputs("00000000\n", temp);
			counter += 1;
		}
		fputs(data, temp);
	}
	fclose(temp);
	copy_temp();
}

void remove_extra_spaces_and_tabs(char* to_fix, char* fixed)
{
	char c;
	int i = 0;
	int j = 0;
	int spaced = 0;
	char temp[INSTRUCTIONS_MAX_SIZE];
	temp[INSTRUCTIONS_MAX_SIZE - 1] = '\0';
	char* ptr;

	while ((c = to_fix[i]) != '\0') {
		if (spaced)
		{
			if (c == '\t' || c == ' ') {}
			else
			{
				// exited from space state
				spaced = 0;
			}
		}

		if (!spaced)
		{
			fixed[j] = c;
			if (c == '\t' || c == ' ')
			{
				fixed[j] = ' ';
				spaced = 1;
			}
			j++;
		}
		i++;
	}
	if (fixed[j - 1] == ' ')
		fixed[j - 1] = '\0';
	else
		fixed[j] = '\0';

	while ((ptr = strstr(fixed, " ,")) != NULL)
	{
		int len = strlen(fixed);
		strncpy(temp, fixed, ptr - fixed);
		strncpy(temp + (ptr - fixed), ptr + 1, len - (ptr - fixed) + 1);
		temp[len - 1] = '\0';
		strncpy(fixed, temp, strlen(temp));
		fixed[len - 1] = '\0';
	}
}

/// <summary>
/// use this function to parse an instruction string into an instruction struct
/// </summary>
/// <param name="instruction"> - instruction string as read from file</param>
/// <param name="output"> - output insturction struct</param>
/// <returns>type of line received, i.e. {regular instruction, pseudo instruction, label}</returns>
LineType parse_instruction(char* instruction, int length, Instruction* output) {
	char cleaned_instruction[LINE_MAX_LENGTH_IN_BYTES];
	remove_extra_spaces_and_tabs(instruction, cleaned_instruction);

	strtok(cleaned_instruction, " ");

	output->immediate1 = 12;
	output->immediate2 = 0;
	output->opcode = 0x01;
	output->rd = 8;
	output->rm = 8;
	output->rs = 8;
	output->rt = 8;
	return REGULAR;
}
/// <summary>
/// receives an insturction input, encodes instruction into hexadecimal representation
/// </summary>
/// <param name="input"> - Instruction struct to be encoded</param>
/// <param name="output"> - encoded instruction</param>
/// <returns>1 on success, 0 on failure</returns>
int encode_instruction(Instruction* input, char* output) {

}

/// <summary>
/// reads a single line from a given input file
/// </summary>
/// <param name="input"> - file to read lines from</param>
/// <param name="line"> - line will be written here</param>
/// <returns>number of bytes in output line</returns>
int read_line(FILE* input, char* line)
{
	if (input == NULL) {
		line[0] = '\0'; 
		return 0;
	}

	char c;
	int stop = 0;
	int i = 0;
	while (!stop && (c = fgetc(input)) != EOF)
	{
		if (c == '\n' || c == '\0')
			stop = 1;
		else
		{
			line[i] = c;
			i++;
		}
	}
	line[i] = '\0';
	return i;
}