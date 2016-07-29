/***
 * wait for it
 * computes the optimal arrival time of 3 buses within an
 * hour that minimizes passengers' waiting times
 **/

#include <stdio.h>
#include "cs50.h"
#define MAX_MINUTES 60

// returns the total waiting time for the given arrival time of the buses
int compute_waiting_time(int bus1, int bus2, int bus3, int *arrivals, int num_passengers)
{
    int waiting_time = 0;
    int bus_arrivals[] = {bus1, bus2, bus3};
    int current_bus = 0;
    int i, passenger;

    for (passenger = 0; passenger < num_passengers; passenger++) {
        // people can miss up to 3 busses
        for (i = 0; i < 3; i++) {
            if (arrivals[passenger] > bus_arrivals[current_bus]) {
                current_bus++;
            }
        }

        // no passenger left behind
        if (current_bus >= 3) {
            waiting_time = INT_MAX;
            break;
        }

        waiting_time += bus_arrivals[current_bus] - arrivals[passenger];
    }

    return waiting_time;
}


int main(void)
{
    // read in passenger arrival times
    int num_passengers = GetInt();
    int *arrivals = malloc(sizeof(int) * num_passengers);

    for (int passenger = 0; passenger < num_passengers; passenger++) {
        arrivals[passenger] = GetInt();
    }

    // generate allowed permutations of arrival times
    int i, j, k, l;
    int waiting_time;
    int optimal_bus_times[3];
    int min_waiting_time = INT_MAX;

    for (i = 0; i < MAX_MINUTES - 2; i++) {
        for (j = i + 1; j < MAX_MINUTES - 1; j++) {
            for (k = j + 1; k < MAX_MINUTES; k++) {
                waiting_time = compute_waiting_time(i, j, k, arrivals, num_passengers);

                // keep track of current best times
                if (waiting_time < min_waiting_time) {
                    min_waiting_time = waiting_time;

                    optimal_bus_times[0] = i;
                    optimal_bus_times[1] = j;
                    optimal_bus_times[2] = k;
                }
            }
        }
    }

    // report result
    for (int i = 0; i < 3; i++) {
        printf("%d\n", optimal_bus_times[i]);
    }

    free(arrivals);
    return 0;
}

