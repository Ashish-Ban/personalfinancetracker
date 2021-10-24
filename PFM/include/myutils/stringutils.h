#ifndef STRINGUTILS_H
#define STRINGUTILS_H

/// <summary>
/// @depreacted. Removes all the mulitiple new lines in a string. Prefer standard library functions instead
/// </summary>
/// <param name="string_to_clean"></param>
/// <returns></returns>
char* clean_multi_new_line(char* stringToClean);

/// <summary>
/// Used as a easy function to loop and concatenate. Takes any number of strings to concatenate and returns the full stirng 
/// </summary>
/// <param name="destination">First string, also the result will be stored here. Allocate the necessary memory for this String</param>
/// <param name="num_values">Number of strings to concatenate excluding the char * destination parameter</param>
/// <param name="">... strings to concatenate</param>
void replace_with_values(char* destination, int num_values, ...);

/// <summary>
/// Converts a long int into a string
/// </summary>
/// <param name="destination">String to store the converted result</param>
/// <param name="number">Long int number to convert</param>
void long_to_string(char* destination, long int number);

/// <summary>
/// User ltoa function from standard library. This function is used to convert a number into a char
/// ltoa from std library will handle all the cases.;
/// </summary>
/// <param name="number">Number to convert to string</param>
/// <returns>char</returns>
char get_corresponding_char(int number);
#endif

