/***
 * airport arbitrage
 *
 * determines whether or not the currency exchange rates
 * of airport kiosks are consistent
 ***/

#include <stdio.h>
#include <math.h>
#include "cs50.h"

// an offer to exchange to currency of given id for a certain rate
typedef struct offer
{
    int currency_id;
    double rate;
    struct offer *next;
}
offer;

/***
 * reads in data and constructs an adjacency-list of exchange rates
 * returns offers:
 *     array of pointers to exchange offers, each array
 *     index corresponding to a currency
 ***/
offer** generate_exchange_rates_graph(int num_currencies)
{
    // initialize array with currency-nodes
    offer **offers = malloc(sizeof(offer*) * num_currencies);

    int i;
    for (i = 0; i < num_currencies; i++) {
        offers[i] = NULL;
    }

    // read in currency exchange offers
    int from_currency, to_currency;
    double rate;

    for (i = 0; i < num_currencies; i++) {
        scanf("%d %d %lf", &from_currency, &to_currency, &rate);

        // add edge to the currency offered for exchange
        offer *new_offer = malloc(sizeof(offer));
        new_offer->currency_id = to_currency;
        new_offer->rate = rate;
        new_offer->next = offers[from_currency];
        offers[from_currency] = new_offer;

        // add the reverse edge as well
        offer *reverse_offer = malloc(sizeof(offer));
        reverse_offer->currency_id = from_currency;
        reverse_offer->rate = 1.0 / rate;
        reverse_offer->next = offers[to_currency];
        offers[to_currency] = reverse_offer;
    }

    return offers;
}

// returns the exchange rate for the currencies as saved in the graph
double find_rate(int currency_1, int currency_2, offer **offers)
{
    offer *cursor = offers[currency_1];
    double rate = -1.0;

    while (cursor != NULL) {
        if (cursor->currency_id == currency_2) {
            rate = cursor->rate;
            break;
        }

        cursor = cursor->next;
    }

    return rate;
}

// returns 1 if path for currency ids is consistent (or inexistent), else 0
int is_consistent_path(int cur1, int cur2, int cur3, offer **offers)
{
    double rate12 = find_rate(cur1, cur2, offers);
    double rate23 = find_rate(cur2, cur3, offers);
    double rate31 = find_rate(cur3, cur1, offers);

    // if a path does not exist, the graph will not be inconsistent,
    // so count this as success
    if (rate12 < 0 || rate23 < 0 || rate31 < 0) {
        return 1;
    }

    double roundtrip_value = rate12 * rate23 * rate31;
    return fabs(roundtrip_value - 1.0) < 0.003;
}

int main(void)
{
    int num_currencies = GetInt();
    offer **exchange_offers = generate_exchange_rates_graph(num_currencies);

    /***
     * note: the graph will be consistent if all triples are consistent
     * (well, I hope my proof for this is correct...)
     ***/

    // generate all possible paths of length 3
    int i, j, k;
    int consistent_offers = 1;

    for (i = 0; i < num_currencies - 2; i++) {
        for (j = i + 1; j < num_currencies - 1; j++) {
            for (k = j + 1; k < num_currencies; k++) {

                // a single inconsistent path is enough to break the system
                if (is_consistent_path(i, j, k, exchange_offers) == 0) {
                    consistent_offers = 0;
                }
            }
        }
    }

    printf("%d\n", consistent_offers);
    // free graph memory
    return 0;
}

