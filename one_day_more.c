/***
 * one day more
 *
 * determines whether or not a year is a leap year
 ***/

#include <stdio.h>
#include "cs50.h"

int main(void)
{
    int year = GetInt();

    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
        printf("Leap Year\n");
    }
    else {
        printf("Not Leap Year\n");
    }

    return 0;
}

