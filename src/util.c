#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util.h"

char *duplicate_string(const char *string) {
    char *duplicate;

    if (string == NULL) {
        return NULL;
    }

    duplicate = malloc(strlen(string) + 1);
    if (duplicate == NULL) {
        return NULL;
    }

    strcpy(duplicate, string);

    return duplicate;
}

void trim_lr_whitespaces(char *trimmed, const char *string, int max_len) {
    char *end;

    if (!strlen(string)) {
        return;
    }

    /*
     * Starting from the start of the non-trimmed string,
     * looks from the first non-whitespace character.
     */
    while (string != NULL && isspace(*string)) {
        string++;
    }

    /*
     * Copies a non-trimmed string staring from the first
     * non-whitespace character to the destionation.
     * Doesn't copy more than 'max_len' characters.
     */
    strncpy(trimmed, string, max_len);

    /*
     * Starting from the end of the left trimmed string,
     * looks for the first non-whitespace character.
     * 
     * Also, makes sure the string is null-terminated.
     */
    end = trimmed + (strlen(trimmed) - 1);
    while (end > trimmed && isspace(*end)) {
        end--;
    }
    *++end = '\0';
}

int get_user_input(char *user_answer, int max_len) {
    int ch, len;

    fgets(user_answer, max_len, stdin);

    /*
     * If the fgets function didn't read the new-line character from the stdin,
     * drop all extra characters till the new-line character (inclusive).
     */
    len = (int) strlen(user_answer);
    if (user_answer[len - 1] != '\n') {
        while ((ch = getchar()) != '\n' && ch != EOF) {
            ;
        }
    }

    return len;
}
