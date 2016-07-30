/***
 * word_reverse.c
 *
 * reverses every word in a string individually
 ***/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cs50.h"

int main(void)
{
    char *input = GetString();
    int length = strlen(input);

    char buffer[length];
    char output[length];

    // setup cursors for input, buffer, and output
    int i;
    int buf_idx = 0;
    int out_idx = 0;

    for (i = 0; i < length; i++) {
        if (isalnum(input[i])) {
            buffer[buf_idx] = input[i];
            buf_idx++;
        }
        else {
            while (buf_idx > 0) {
                output[out_idx++] = buffer[buf_idx - 1];
                buf_idx--;
            }

            output[out_idx++] = input[i];
        }
    }

    // do not forget the last buffer
    if (out_idx < length) {
        while (buf_idx > 0) {
            output[out_idx++] = buffer[buf_idx - 1];
            buf_idx--;
        }
    }

    // print the result
    for (i = 0; i < length; i++) {
        printf("%c", output[i]);
    }

    free(input);
    return 0;
}

