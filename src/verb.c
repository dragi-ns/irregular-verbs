#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "verb.h"
#include "util.h"

#define LINE_LEN 64
#define VERBS_LEN 138
#define VERB_FORM_LEN 3

const char *VALID_VERB_FORMS_NAMES[] = {"base", "past simple", "past participle"};

Verb *verb_load_verbs(const char *filename) {
    FILE *stream; 
    Verb *verbs;
    int verbs_count = 0;
    int verb_form_count = 0;
    char line[LINE_LEN] = {0};
    char line_trimmed[LINE_LEN] = {0};
    char *token;
    char separator[] = ",";
    
    stream = fopen(filename, "r");
    if (stream == NULL) {
        fprintf(stderr, "Error reading %s in %s.", filename, __FILE__);
        return NULL;
    }

    verbs = calloc(VERBS_LEN, sizeof(Verb));
    if (verbs == NULL) {
        fprintf(stderr, "Error allocating a dynamic memory in %s.", __FILE__);
        fclose(stream);
        return NULL;
    }

    while (verbs_count < VERBS_LEN && fgets(line, LINE_LEN, stream)) {
        trim_lr_whitespaces(line_trimmed, line, LINE_LEN);

        token = strtok(line_trimmed, separator);
        while (verb_form_count < VERB_FORM_LEN && token != NULL) {
            switch (verb_form_count) {
                case 0:
                    verbs[verbs_count].base = duplicate_string(token);
                    break;
                case 1:
                    verbs[verbs_count].past_simple = duplicate_string(token);
                    break;
                case 2:
                    verbs[verbs_count].past_participle = duplicate_string(token);
                    break;
            }
            token = strtok(NULL, separator);
            verb_form_count++;
        }
        verbs_count++;
        verb_form_count = 0;
    }

    fclose(stream);

    return verbs;
}

void verb_unload_verbs(Verb *verbs) {
    if (verbs == NULL) {
        return;
    }

    for (int i = 0; i < VERBS_LEN; i++) {
        free(verbs[i].base);
        free(verbs[i].past_simple);
        free(verbs[i].past_participle);
    }

    free(verbs);
}

bool verb_is_valid_form_name(const char *form_name) {
    for (int i = 0; i < VERB_FORM_LEN; i++) {
        if (strcmp(form_name, VALID_VERB_FORMS_NAMES[i]) == 0) {
            return true;
        }
    }
    return false;
}

const Verb *verb_get_random(const Verb *verbs) {
    int random_index;
    static int verbs_count = 0;
    static bool seen[VERBS_LEN] = {0};

    if (verbs_count >= VERBS_LEN) {
        return NULL;
    }

    random_index = rand() % VERBS_LEN;
    while(seen[random_index]) {
        random_index = rand() % VERBS_LEN;
    }
    seen[random_index] = true;
    verbs_count++;

    return verbs + random_index;
}

const char *verb_get_form(const Verb *verb, const char *form_name) {
    if (strcmp(form_name, VALID_VERB_FORMS_NAMES[0]) == 0) {
        return verb->base;
    }
    
    if (strcmp(form_name, VALID_VERB_FORMS_NAMES[1]) == 0) {
        return verb->past_simple;
    }

    if (strcmp(form_name, VALID_VERB_FORMS_NAMES[2]) == 0) {
        return verb->past_participle;
    }

    return NULL;
}
