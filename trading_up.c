/***
 * trading up
 *
 * find a way to trade in intial items so that you end up with
 * ones you desire
 ***/

#include <stdio.h>
#include <stdlib.h>
#include "cs50.h"

typedef struct state
{
    int num_items;
    int *items;
}
state;

typedef struct list_item
{
    state *s;
    struct list_item *next;
}
list_item;

typedef struct list
{
    list_item *head;
}
list;

typedef struct offer
{
    int from;
    int to_1;
    int to_2;
    unsigned int time;
    struct offer *next;
}
offer;

typedef struct node
{
    state *s;
    unsigned int time;
    int depth;
    struct node *next;
}
node;

typedef struct fringe
{
    node *head;
    int size;
}
fringe;

// function declarations
int* get_item_input(int *num_desired_items);
offer* get_offer_input(int *number_of_offers);
offer* make_offer(unsigned int time, int from, int to_1, int to_2, offer *next);
state* make_state(int num_items, int items[]);
offer* duplicate_offer(offer *o);
offer* find_offers(state *s, offer *offers);
node* make_node(state *s, unsigned int time, int depth);
void sort(int *arr, int num_elements);
int compare(const void *x1, const void *x2);
int is_goal_state(state *s, int *desired_items, int num_desired_items);
fringe* fringe_init();
void fringe_push(fringe *f, node *n);
node* fringe_pop(fringe *f);
int is_empty(fringe *f);
list* init_closed_list(void);
void insert_into_closed_list(list *closed_list, state *s);
int is_same_state(state *s1, state *s2);
int is_in_closed_list(list *closed_list, state *s);
state* apply(state *s, offer *o);


int main(void)
{
    // read in input data
    int num_different_items = GetInt();
    int num_desired_items;
    int *desired_items = get_item_input(&num_desired_items);

    int num_offers;
    offer *offers = get_offer_input(&num_offers);

    // DFS setup
    int items[] = {0};
    state *start_state = make_state(1, items);
    fringe *f = fringe_init();
    fringe_push(f, make_node(start_state, 0, 0));
    list *closed_list = init_closed_list();

    unsigned int optimal_time = INT_MAX;
    node *current_node;
    state *next_state;
    offer *available_offers;
    offer *previous_offer;
    offer *current_offer;
    unsigned int time;
    int depth;

    while (!is_empty(f)) {
        current_node = fringe_pop(f);

        // goal test
        if (is_goal_state(current_node->s, desired_items, num_desired_items)) {
            optimal_time = current_node->time;
            break;
        }

        if (is_in_closed_list(closed_list, current_node->s)) {
            continue;
        }

        if (current_node->depth > num_desired_items - 1) {
            continue;
        }

        // add state to closed list
        insert_into_closed_list(closed_list, current_node->s);

        // expand state
        available_offers = find_offers(current_node->s, offers);
        current_offer = available_offers;

        while (current_offer != NULL) {
            next_state = apply(current_node->s, current_offer);
            time = current_node->time + current_offer->time;
            depth = current_node->depth + 1;

            fringe_push(f, make_node(next_state, time, depth));

            current_offer = current_offer->next;
        }

        // free available offers
        previous_offer = available_offers;

        while (available_offers != NULL) {
            available_offers = available_offers->next;
            free(previous_offer);
            previous_offer = available_offers;
        }
    }

    if (optimal_time == INT_MAX) {
        printf("%d\n", -1);
    }
    else {
        printf("%d\n", optimal_time);
    }

    return 0;
}



// inserts a new element so that the smallest times are in the front
void fringe_push(fringe *f, node *n)
{
    node **cursor = &(f->head);

    while (*cursor != NULL) {
        if (n->time < (*cursor)->time) {
            n->next = *cursor;
            *cursor = n;
            break;
        }

        cursor = &((*cursor)->next);
    }

    // insert n if the end of the list is reached
    if (*cursor == NULL) {
        n->next = NULL;
        *cursor = n;
    }

    f->size += 1;
}




// returns the resulting state after accepting the offer
state* apply(state *s, offer *o)
{
    // replace resp. item in s with the two items gained
    int *new_items = malloc(sizeof(int) * (s->num_items + 1));

    for (int i = 0; i < s->num_items; i++) {
        if (s->items[i] == o->from) {
            new_items[i] = o->to_1;
        }
        else {
            new_items[i] = s->items[i];
        }
    }

    new_items[s->num_items] = o->to_2;
    return make_state(s->num_items + 1, new_items);
}

// returns an empty closed list
list* init_closed_list(void)
{
    list *closed_list = malloc(sizeof(list));
    closed_list->head = NULL;
    return closed_list;
}

// prepends the given state to the list
void insert_into_closed_list(list *closed_list, state *s)
{
    list_item *li = malloc(sizeof(list_item));
    li->s = s;
    li->next = NULL;

    if (closed_list->head == NULL) {
        closed_list->head = li;
    }
    else {
        li->next = closed_list->head;
        closed_list->head = li;
    }

    return;
}

// returns 1 if two given states have the same content
int is_same_state(state *s1, state *s2)
{
    // quick check: same number of elements?
    if (s1->num_items != s2->num_items) {
        return 0;
    }

    // extensive check: same elements? (note: items are sorted)
    for (int i = 0; i < s1->num_items; i++) {
        if (s1->items[i] != s2->items[i]) {
            return 0;
        }
    }

    return 1;
}

// returns 1 if the given state is in the closed list
int is_in_closed_list(list *closed_list, state *s)
{
    list_item *cursor = closed_list->head;
    int found = 0;

    while (cursor != NULL) {
        if (is_same_state(cursor->s, s)) {
            found = 1;
            break;
        }

        cursor = cursor->next;
    }

    return found;
}
// returns 1 if there are no nodes on the fringe, else 0
int is_empty(fringe *f)
{
    return f->size == 0;
}

// returns an empty fringe
fringe* fringe_init()
{
    fringe *f = malloc(sizeof(fringe));
    f->head = NULL;
    f->size = 0;
    return f;
}

// returns the first element from the fringe
node* fringe_pop(fringe *f)
{
    if (f->size == 0) {
        return NULL;
    }

    node *first = f->head;
    f->head = first->next;
    f->size -= 1;
    return first;
}

// returns 1 if exactly the desired items are present in the state
int is_goal_state(state *s, int *desired_items, int num_desired_items)
{
    // quick check: the amount has to be the same
    if (s->num_items != num_desired_items) {
        return 0;
    }

    // extensive check: are all the items the same?
    for (int i = 0; i < num_desired_items; i++) {
        if (s->items[i] != desired_items[i]) {
            return 0;
        }
    }

    return 1;
}

// returns a new state with given items
state* make_state(int num_items, int items[])
{
    state *new_state = malloc(sizeof(state));
    new_state->num_items = num_items;
    new_state->items = malloc(sizeof(int) * num_items);

    for (int i = 0; i < num_items; i++) {
        new_state->items[i] = items[i];
    }

    sort(new_state->items, new_state->num_items);
    return new_state;
}

// returns a new node filled with the given data
node* make_node(state *s, unsigned int time, int depth)
{
    node *new_node = malloc(sizeof(node));

    new_node->s = s;
    new_node->time = time;
    new_node->depth = depth;
    new_node->next = NULL;

    return new_node;
}

// returns an new offer with copied data and empty next-pointer
offer* duplicate_offer(offer *o)
{
    offer *copy = malloc(sizeof(offer));

    copy->from = o->from;
    copy->time = o->time;
    copy->to_1 = o->to_1;
    copy->to_2 = o->to_2;
    copy->next = NULL;

    return copy;
}

// returns a linked list of offers available in the given state
offer* find_offers(state *s, offer *offers)
{
    offer *first = NULL;
    offer *previous = NULL;
    offer *cursor = offers;
    int i;

    while (cursor != NULL) {
        for (i = 0; i < s->num_items; i++) {

            // append relevant offers to the results-list
            if (cursor->from == s->items[i]) {
                offer *new_offer = duplicate_offer(cursor);

                if (first == NULL) {
                    first = new_offer;
                    previous = new_offer;
                }
                else {
                    previous->next = new_offer;
                    previous = new_offer;
                }
            }
        }

        cursor = cursor->next;
    }

    return first;
}

// returns a new offer-struct
offer* make_offer(unsigned int time, int from, int to_1, int to_2, offer *next)
{
    offer *new_offer = malloc(sizeof(offer));

    new_offer->time = time;
    new_offer->from = from;
    new_offer->to_1 = to_1;
    new_offer->to_2 = to_2;
    new_offer->next = next;

    return new_offer;
}

// reads in information about items
int* get_item_input(int *num_desired_items)
{
    int num_items = GetInt();
    int *desired_items = malloc(sizeof(int) * num_items);

    for (int i = 0; i < num_items; i++) {
        desired_items[i] = GetInt();
    }

    *num_desired_items = num_items;
    sort(desired_items, num_items);
    return desired_items;
}

// reads in information about offer
offer* get_offer_input(int *number_of_offers)
{
    int num_offers = GetInt();

    if (num_offers < 1) {
        return NULL;
    }

    int from, to_1, to_2, time;
    offer *first = NULL;
    offer *previous = NULL;

    for (int i = 0; i < num_offers; i++) {
        scanf("%d %d %d %d", &time, &from, &to_1, &to_2);
        offer *new_offer = make_offer(time, from, to_1, to_2, NULL);

        if (first == NULL) {
            first = new_offer;
            previous = new_offer;
        }
        else {
            previous->next = new_offer;
            previous = new_offer;
        }
    }


    *number_of_offers = num_offers;
    return first;
}

int compare(const void *x1, const void *x2)
{
    return *(int*)x1 - *(int*)x2;
}

// returns the given int-array sorted in ascending order
void sort(int *arr, int num_elements)
{
    // quick check 1: array has to hold multiple elements
    if (num_elements < 2) {
        return;
    }

    // quick check 2: nothing to do if already sorted
    int sorted = 1;

    for (int i = 0; i < num_elements - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            sorted = 0;
        }
    }

    if (sorted) {
        return;
    }

    qsort(arr, num_elements, sizeof(int), compare);
    return;
}

