#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "verb.h"
#include "util.h"
#include "color.h"

#define USER_ANSWER_LEN 20

typedef struct {
    char *from_form;
    char *to_form_1;
    char *to_form_2;
} Mode;


/*
 * Checks if the user gave correct command line arguments.
 * 
 * @param argc Count of the command line arguments.
 * @param argv Array of the command line arguments.
 */
static void validate_command_line_arguments(int argc, char *argv[], Mode *mode);

/*
 * Checks for a duplicate verb form name.
 * 
 * @param form_name Verb form name to check for duplicate.
 * @return true for duplicate, otherwise false
 */
static bool check_for_duplicate_form_name(const char *form_name);

/*
 * Prints the instruction and the few examples
 * to the user.
 */
static void print_instruction(void);

/*
 * Prints a interrogation mode the user has chosen
 * and some extra instruction based on the chosen mode.
 * 
 * @param mode Pointer to the Mode struct.
 */
static void print_chosen_mode(const Mode *mode);

/*
 * Starts the interrogation with the mode that user chose.
 * 
 * @param mode Struct representing the interrogation mode.
 */
static void start_interrogation(const Mode *mode);

/*
 * Checks if the user input contains an exit code.
 * 
 * @param answer String representing the user input.
 * @param exit_code Number representing an exit code.
 * @return Returns true if the user input contains an exit code, otherwise false.
 */
static bool wants_exit(const char *answer, int exit_code);

/*
 * Checks if the user gave correct answer.
 * 
 * @param user_answer String representing the user answer.
 * @param verb Verb struct with multiple forms.
 * @param from_form String representing a verb from form name.
 * @param to_form String representing a verb to form name.
 * @return Returns true if the user gave correct answer, otherwise false
 */
static bool validate_user_answer(const char *user_answer, const Verb *verb,
                                 const char *from_form, const char *to_form);

/*
 * Prints the interrogation status at the end to the user.
 * 
 * @param question_count Integer representing total number of questions.
 * @param correct_count Integer representing total number of correct answers.
 */
static void print_interrogation_status(const int question_count, const int correct_count);

int main(int argc, char *argv[]) { 
    Mode mode = {0};

    srand(time(NULL));

    validate_command_line_arguments(argc, argv, &mode);
    print_chosen_mode(&mode);

    start_interrogation(&mode);

    exit(EXIT_SUCCESS);
}

static void validate_command_line_arguments(int argc, char *argv[], Mode *mode) {
    if (argc < 3) {
        fprintf(stderr, "Too few arguments!\n\n");
        goto INVALID;
    }

    /*
     * The first element in the array 'argv' is the program name.
     * 
     * Because the third verb form name is optional, there is 'i == argc' check,
     * which prevents accessing a non-existing element in the array.
     * 
     * Extra command line arguments (if any) are ignored.
     */
    for (int i = 1; i < 4; i++) {
        if (i == argc) {
            break;
        }

        if (!verb_is_valid_form_name(argv[i])) {
            fprintf(stderr, "'%s' is invalid form name!\n\n", argv[i]);
            goto INVALID;
        }

        if (check_for_duplicate_form_name(argv[i])) {
            fprintf(stderr, "'%s' form name is already listed!\n\n", argv[i]);
            goto INVALID;
        }

        switch (i) {
            case 1:
                mode->from_form = argv[i];
                break;
            case 2:
                mode->to_form_1 = argv[i];
                break;
            case 3:
                mode->to_form_2 = argv[i];
                break;
        }
    }

    return;

    INVALID:
        print_instruction();
        exit(EXIT_FAILURE);
}

static bool check_for_duplicate_form_name(const char *form_name) {
    /*
     * Static variables inside a functions maintain their values
     * throughout the program.
     */
    static int count = 0;
    static const char *seen[3] = {0};

    // TODO: I should have probably handle this better.
    /*
     * We run this function max three times throughout the program.
     * So, the 'count' should never be grather than or euqal to three at this point.
     * 
     */
    assert(count < 3);

    for (int i = 0; i < count; i++) {
        if (strcmp(form_name, seen[i]) == 0) {
            return true;
        }
    }

    seen[count++] = form_name;

    return false;
}

static void print_instruction(void) {
    printf("An interrogation program for irregular verbs.\n\n");
    printf("Usage:\n");
    printf("\tir-verbs <from_form_name> <to_form_name_1> [to_form_name_2]\n");
    printf("Examples:\n");
    printf("\tir-verbs base \"past simple\"\n");
    printf("\tir-verbs base \"past participle\"\n");
    printf("\tir-verbs \"past particple\" base \"past simple\"\n");
}

static void print_chosen_mode(const Mode *mode) {
    printf("You have chosen " BOLD "%s -> %s", mode->from_form, mode->to_form_1);
    if (mode->to_form_2 != NULL) {
        printf(" -> %s", mode->to_form_2);
    }
    printf(RESET " interrogation mode!\n");

    printf("Given a verb in the " BOLD "%s" RESET " form, you need to enter the " BOLD "%s" RESET, 
           mode->from_form, mode->to_form_1);
    if (mode->to_form_2 != NULL) {
        printf(" and the " BOLD "%s" RESET, mode->to_form_2);
    }
    printf(" form of a verb.\n");

    printf("If you want to " BOLD "exit" RESET " the program type " BOLD "0 (zero)" RESET " and press enter.\n");
    printf(BOLD CYAN "Good luck!\n\n" RESET);
}

static void start_interrogation(const Mode *mode) {
    Verb *verbs;
    const Verb *random_verb;
    char user_answer[USER_ANSWER_LEN] = {0};
    int correct_count = 0, question_count = 0;
    bool correct_1 = false, correct_2 = false;

    verbs = verb_load_verbs("data/verbs.csv");

    for (;;) {
        random_verb = verb_get_random(verbs);
        if (random_verb == NULL) {
            // There are no more verbs :(
            break;
        }

        printf("%s (%s form):\n", verb_get_form(random_verb, mode->from_form), mode->from_form);

        printf("%s >> ", mode->to_form_1);
        get_user_input(user_answer, USER_ANSWER_LEN);
        if (wants_exit(user_answer, 0)) {
            break;
        }
        correct_1 = validate_user_answer(user_answer, random_verb, mode->from_form, mode->to_form_1);

        if (mode->to_form_2 != NULL) {
            printf("%s >> ", mode->to_form_2);

            get_user_input(user_answer, USER_ANSWER_LEN);
            if (wants_exit(user_answer, 0)) {
                break;
            }
            correct_2 = validate_user_answer(user_answer, random_verb, mode->from_form, mode->to_form_2);
        } else {
            // There is no second part of the question so mark it as true
            correct_2 = true;
        }

        if (correct_1 && correct_2) {
            correct_count++;
        }
        question_count++;
    }

    print_interrogation_status(question_count, correct_count);
    verb_unload_verbs(verbs);
}

static bool wants_exit(const char *user_answer, int exit_code) {
    int num;

    if (sscanf(user_answer, "%d", &num) == 1) {
        if (num == exit_code) {
            return true;
        }
    }

    return false;
}

static bool validate_user_answer(const char *user_answer, const Verb *verb,
                                 const char *from_form, const char *to_form)
{
    const char *second_part;
    char user_answer_trimmed[USER_ANSWER_LEN] = {0};
    const char *correct_answer = verb_get_form(verb, to_form);

    trim_lr_whitespaces(user_answer_trimmed, user_answer, USER_ANSWER_LEN);

    /*
     * Some verbs can have two past forms.
     * 
     * First we compare the first possible form, second we check second  form.
     * If there is no match with the user answer, the user answer is wrong.
     * 
     */
    if (strncmp(user_answer_trimmed, correct_answer, strcspn(correct_answer, "/")) == 0 ||
        ((second_part = strchr(correct_answer, '/')) != NULL && strcmp(user_answer_trimmed, second_part + 1) == 0)) {
        printf(BOLD GREEN "\nCorrect!\n\n" RESET);
        return true;
    }

    printf(BOLD RED "\nIncorrect!\n" RESET);
    printf(
        BOLD "%s" RESET " of " BOLD "%s" RESET " is " BOLD "%s" RESET "!\n\n",
        to_form, verb_get_form(verb, from_form), correct_answer
    );
    return false;
}

static void print_interrogation_status(const int question_count, const int correct_count) {
    printf(BOLD "\nInterrogation status:\n" RESET);
    printf(BOLD "\tTotal questions:  %3d\n" RESET, question_count);
    printf(BOLD GREEN "\tCorrect answers:  %3d\n" RESET, correct_count);
    printf(BOLD RED "\tInorrect answers: %3d\n" RESET, question_count - correct_count);
}
