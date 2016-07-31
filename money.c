/***
 * money.c
 *
 * computes anually compunded interest
 ***/

#include <stdio.h>
#include <math.h>
#include "cs50.h"

int main(void)
{
    double balance = GetDouble();
    double rate = GetDouble();
    int years = GetInt();

    printf("%.2f\n", (float)(balance * pow((1 + rate), years)));

    return 0;
}

