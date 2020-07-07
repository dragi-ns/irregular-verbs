#ifndef VERB_H
#define VERB_H

#include <stdbool.h>

typedef struct {
    char *base;
    char *past_simple;
    char *past_participle;
} Verb;

/*
 * Parses a csv file.
 * 
 * @param filename String representing a file name of a csv file to parse.
 * @return Pointer to a dynamically allocated array of Verb structs or NULL for error.
 */
Verb *verb_load_verbs(const char *filename);

/*
 * Frees the dynamically alocated array of Verb structs given by 'load_verbs' function.
 * 
 * @param verbs The dynamically allocated array of Verb structs to be freed.
 */
void verb_unload_verbs(Verb *verbs);

/*
 * Checks if a given verb form name is valid.
 * 
 * @param form_name String representing a verb form name to be checked.
 * @return true if verb form name is valid, otherwise false
 */
bool verb_is_valid_form_name(const char *form_name);

/*
 * Gets a random element from an array of Verb structs.
 * 
 * @param verbs An array of Verb structs.
 * @return Returns a pointer to a random element from the array 'verbs' or
 * NULL if there are no more elements.
 */
const Verb *verb_get_random(const Verb *verbs);

/*
 * Returns 'verb' in the requested 'form_name'.
 * 
 * @param verb Struct with mulitple verb forms.
 * @param form_name String representing the verb form name.
 * @return Returns a string representing a verb in 'form_name' or NULL
 * if 'form_name' is invalid.
 */
const char *verb_get_form(const Verb *verb, const char *form_name);

#endif
