#ifndef UTIL_H
#define UTIL_H

/*
 * Duplicates a string.
 * 
 * @param string String to be duplicated.
 * @return Returns a dynamically allocated string or NULL for error.
 */
char *duplicate_string(const char *string);

/*
 * Trims extra whitespaces from the start (left) and end (right) of the string.
 * 
 * @param trimmed A destination string, where the trimmed string is going to be copied.
 * @param string A non-trimmed string.
 * @param max_len Maximum number of characters to copy to the destination.
 */
void trim_lr_whitespaces(char *trimmed, const char *string, int max_len);

/*
 * Reads the user input from the stdin, ignoring extract characters.
 * 
 * @param user_answer A destionation string to read user input into.
 * @param max_len Maximum number of characters to read from the stdin.
 * @return int Number of read characters.
 */
int get_user_input(char *user_answer, int max_len);

#endif
