#ifndef STRINGUTILS_H
#define STRINGUTILS_H

char* clean_multi_new_line(char* stringToClean);
void replace_with_values(char* destination, int num_values, ...);
void long_to_string(char* destination, long int number);
char get_corresponding_char(int number);
#endif

