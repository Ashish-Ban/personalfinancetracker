#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "stringutils.h"

#pragma warning(once : 4996)

char* clean_multi_new_line(char* string_to_clean) {
	for (int i = 0; i < (int)strlen(string_to_clean); i++) {
		if (string_to_clean[i] == '\n') {
			string_to_clean[i] = string_to_clean[i + 1];
		}
	}
	return string_to_clean;
}

void replace_with_values(char* destination, int num_values,  ...) {
	int i,j;
	char* temp;

	va_list valist;
	va_start(valist, num_values);
	
	for (i = 0; i < num_values; i++) {
		//printf("\n For Loop in replace_with_values %s ", va_arg(valist, char *));
		//strcat_s(destination, sizeof destination, va_arg(valist, char*));
		temp = va_arg(valist, char*);
		clean_multi_new_line(temp);
		strcat(destination, temp);
	}
	va_end(valist);

	printf("\n QUERY STRING : %s ", destination);
}

void long_to_string(char* destination, long int number) {
    int i, count = 0;
    while (number > 0) {
        i = (int)number % 10;
        destination[count] = get_corresponding_char(i);
        number /= 10;
        count++;
    }
    destination[count] = '\0';
}

char get_corresponding_char(int number) {
    switch (number)
    {
    case 1:
        return '1';
        break;
    case 2:
        return '2';
        break;
    case 3:
        return '3';
        break;
    case 4:
        return '4';
        break;
    case 5:
        return '5';
        break;
    case 6:
        return '6';
        break;
    case 7:
        return '7';
        break;
    case 8:
        return '8';
        break;
    case 9:
        return '9';
        break;
    case 0:
        return '0';
        break;
    default:
        return '0';
        break;
    }
}