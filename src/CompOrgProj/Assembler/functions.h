#ifndef _FUNCTIONS
#define _FUNCTIONS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void word(int address, char data[8]) {
	int counter = -1;
	char copy_data[10];
	FILE* dmemin = fopen("dmemin.txt", "r");
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


#endif