/***
 * x.c
 *
 * prints a "X" to the screen the size of a given integer
 ***/

#include <stdio.h>
#include "cs50.h"

// returns 1 if a star should be printed at the given position, else 0
int is_star(int i, int j, int n)
{
    return i == 0 ||
           j == 0 ||
           i == j ||
           i == (n - 1) ||
           j == (n - 1) ||
           i == (n - 1 - j);
}

int main(void)
{
    int n = GetInt();

    // only print odd numbered inputs within [3, 23]
    if (n < 3 || n > 23 || n % 2 == 0) {
        return 0;
    }

    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%c", is_star(i, j, n) ? '*' : ' ');
        }

        printf("\n");
    }

    return 0;
}

