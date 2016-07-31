/***
 * name your cat
 *
 * reverses only particular letter in a given word
 ***/

#include <stdio.h>
#include "cs50.h"

// returns 1 if the given char is among the special ones, else 0
int is_special_char(char c)
{
    char letters[] = "aceiknt";

    for (int i = 0; i < 7; i++) {
        if (letters[i] == c) {
            return 1;
        }
    }

    return 0;
}

int main(void)
{
    int length = GetInt();
    char *input = GetString();

    int i;
    int j = length - 1;
    char tmp;

    for (i = 0; i < length; i++) {
        // move i forward to the next special character
        if (is_special_char(input[i])) {

            // move j backwards to the next special character
            while (!is_special_char(input[j])) {
                j--;
            }

            // swap if necessary
            if (i < j) {
                tmp = input[j];
                input[j] = input[i];
                input[i] = tmp;
                j--;
            }
        }
    }

    printf("%s\n", input);

    free(input);
    return 0;
}

