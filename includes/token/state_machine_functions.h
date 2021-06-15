#ifndef _STATE_MACHINE_FUNCTIONS_H_
#define _STATE_MACHINE_FUNCTIONS_H_

#include "./constants.h"

void add_rule(int (&table)[MAX_ROWS][MAX_COLUMNS], int start_state, int input, int end_state); // adds a rule to table:
                                                                                               // when encountering input in start_state
                                                                                               // go to end_state

int next_state(int const (&table)[MAX_ROWS][MAX_COLUMNS], int start_state, int input);         // grabs the next state given
                                                                                               // the start_state and an input

void initialize_table(int (&table)[MAX_ROWS][MAX_COLUMNS]); //set all values in the table to ERROR_STATE

void print_table(int const (&table)[MAX_ROWS][MAX_COLUMNS]); //prints the adjacency table
#endif