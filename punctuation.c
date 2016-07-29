#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cs50.h"

int main(void)
{
    char *input = GetString();

    // find out how much space to allocate for the final string
    int length = 0;
    int extra_space = 0;
    char *cursor, *result_cursor;

    for (cursor = input; *cursor != '\0'; cursor++) {
        length++;

        if (*cursor == ',' || *cursor == '.') {
            extra_space++;
        }
    }

    // edit input string
    char *result = malloc(length + extra_space + 1);
    result[length + extra_space] = '\0';

    // flag to decide when to capitalize next letter after a period
    int capitalize = 0;

    for (cursor = input, result_cursor = result; *cursor != '\0'; cursor++) {
        // capitalize letters after periods, but only letters
        if (capitalize && isalpha(*cursor)) {
            *result_cursor = *cursor & ~('a' - 'A');
            capitalize = 0;
        }
        else {
            *result_cursor = *cursor;
        }

        // add space for every comma or period
        if (*cursor == ',' || *cursor == '.') {
            result_cursor++;
            *result_cursor = ' ';
        }

        // capitalize next letter for every period
        if (*cursor == '.') {
            capitalize = 1;
        }

        result_cursor++;
    }

    printf("%s\n", result);
    return 0;
}
