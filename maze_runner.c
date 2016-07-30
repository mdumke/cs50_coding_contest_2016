/***
 * maze_runner.c
 *
 * finds the path through a maze read from standard input
 ***/

#include <stdio.h>
#include <string.h>
#include "cs50.h"

#define MAX_N 1000
#define DOWN 1
#define UP 2
#define LEFT 3
#define RIGHT 4

// datastructures to perform DFS with
typedef struct action
{
    int name;
    struct action *next;
}
action;

typedef struct state
{
    int row;
    int col;
}
state;

typedef struct fringe_item
{
    state *state;
    action *strategy;
    struct fringe_item *next;
}
fringe_item;

typedef struct fringe
{
    int size;
    fringe_item *head;
    fringe_item *tail;
}
fringe;

// function declarations
char** read_maze(int *n, int *m);
int* get_actions(state *s, char **maze, int *num_actions);
int coord(int maze_coordinate);
state* make_state(int row, int col);
fringe* fringe_init();
void fringe_enqueue(fringe *f, fringe_item *item);
fringe_item* fringe_dequeue(fringe *f);
int is_goal_state(state *s, int n, int m);
int is_empty(fringe *f);

// returns a new fringe item filled with the given data
fringe_item* make_fringe_item(state *state, action *strategy)
{
    fringe_item *new_item = malloc(sizeof(fringe_item));
    new_item->state = state;
    new_item->strategy = strategy;
    new_item->next = NULL;
    return new_item;
}

// returns a new action-item that can be part of a strategy
action* make_action(int name)
{
    action *new_action = malloc(sizeof(action));
    new_action->name = name;
    new_action->next = NULL;
    return new_action;
}

// returns the resulting state when applying the given action
state* apply(state *s, int action_name)
{
    state *new_state = malloc(sizeof(state));
    new_state->row = s->row;
    new_state->col = s->col;

    switch(action_name) {
        case DOWN:
            new_state->row += 1;
            break;
        case UP:
            new_state->row -= 1;
            break;
        case LEFT:
            new_state->col -= 1;
            break;
        case RIGHT:
            new_state->col += 2;
            break;
    }

    return new_state;
}


int main(void)
{
    int n, m;
    char **maze = read_maze(&n, &m);

    // initialize the fringe
    fringe *f = fringe_init();
    fringe_enqueue(f, make_fringe_item(make_state(0, 0), NULL));

    // set up search helper-variables
    fringe_item *current_item;
    state *current_state;
    state *next_state;
    int num_actions, i;
    int *available_actions;
    int strategy_found = 0;
    action *strategy;

    // perform breadth first search
    while (!is_empty(f)) {
        current_item = fringe_dequeue(f);
        current_state = current_item->state;

        // goal state-check
        if (is_goal_state(current_state, n, m)) {
            strategy = current_item->strategy;
            strategy_found = 1;
            break;
        }

        // put the current node into the closed set


        // expand current node
        available_actions = get_actions(current_state, maze, &num_actions);

        for (i = 0; i < num_actions; i++) {
            next_state = apply(current_state, available_actions[i]);

            // add this state/action-pair to the fringe
        }

    }


    if (!strategy_found) {
        printf("Could not solve this maze :(\n");
    }



    // free maze memory
    return 0;
}










// returns 1 if the fringe is of size 0, else 0
int is_empty(fringe *f)
{
    return f->size == 0;
}

// returns 1 if the given state is the goal state, else 0
int is_goal_state(state *s, int n, int m)
{
    return s->row == (n - 1) && s->col == (m - 1);
}

// sets up a new, empty fringe
fringe* fringe_init()
{
    fringe *new_fringe = malloc(sizeof(fringe));
    new_fringe->size = 0;
    new_fringe->head = NULL;
    new_fringe->tail = NULL;
    return new_fringe;
}

// inserts a new item at the end of the fringe
void fringe_enqueue(fringe *f, fringe_item *item)
{
    if (f->size == 0) {
        f->head = item;
        f->tail = item;
        f->size = 1;
    }
    else {
        f->tail->next = item;
        f->tail = item;
        f->size += 1;
    }
}

// returns the first item from the fringe
fringe_item* fringe_dequeue(fringe *f)
{
    if (f->size == 0) {
        return NULL;
    }

    fringe_item *item = f->head;

    if (f->size == 1) {
        f->head = NULL;
        f->tail = NULL;
    }
    else {
        f->head = item->next;
    }

    f->size -= 1;
    return item;
}

// creates a new state to be used in the search
state* make_state(int row, int col)
{
    state *new_state = malloc(sizeof(state));
    new_state->row = row;
    new_state->col = col;
    return new_state;
}

// returns the actions available at coordinate (i, j)
int* get_actions(state *s, char **maze, int *num_actions)
{
    int i = s->row;
    int j = s->col;

    // there will be 4 actions at max
    int *actions = malloc(sizeof(int) * 4);
    int position = 0;

    if (maze[coord(i) - 1][coord(j)] == ' ' && i != 0) {
        actions[position] = UP;
        position++;
    }

    if (maze[coord(i) + 1][coord(j)] == ' ') {
        actions[position] = DOWN;
        position++;
    }

    if (maze[coord(i)][coord(j) - 1] == ' ') {
        actions[position] = LEFT;
        position++;
    }

    if (maze[coord(i)][coord(j) + 1] == ' ') {
        actions[position] = RIGHT;
        position++;
    }

    *num_actions = position;
    return actions;
}

// reads maze representation from stdin and returns an array of arrays
char** read_maze(int *n, int *m)
{
    char **maze = malloc(sizeof(char*) * MAX_N);
    char *buffer;
    int rows = 0;
    int cols = 0;

    while (1) {
        buffer = GetString();

        if (buffer == NULL) {
            break;
        }

        maze[rows] = buffer;
        rows++;
    }

    // the character matrix and the abstract maze have different dimensions!
    *n = (rows - 1) / 2;
    *m = (strlen(maze[0]) - 1) / 2;
    return maze;
}

// transforms the maze-coordinate the coordinate in the character-matrix
int coord(int maze_coordinate)
{
    return 2 * maze_coordinate + 1;
}


