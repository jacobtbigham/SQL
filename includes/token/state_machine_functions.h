#ifndef STATE_MACHINE_FUNCTIONS_H
#define STATE_MACHINE_FUNCTIONS_H

/**
 * @file state_machine_functions.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief Defines the state machine functions that dictate a Tokenizer's behavior
 * @version 0.1
 * @date 2021-08-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "./constants.h"

/**
 * @brief Adds a rule to the table: when input is in start_state, go to end_state
 * 
 * @param start_state the current state
 * @param input the input
 * @param end_state the new state
 */
void add_rule(int (&table)[MAX_ROWS][MAX_COLUMNS],
              int start_state,
              int input,
              int end_state);

/**
 * @brief Find the next state given an input and a start_state
 * 
 * @param start_state the current state
 * @param input the input
 * @return int the next state
 */
int next_state(int const (&table)[MAX_ROWS][MAX_COLUMNS],
               int start_state,
               int input);

/**
 * @brief Create a new adjacency table
 * 
 */
void initialize_table(int (&table)[MAX_ROWS][MAX_COLUMNS]);

/**
 * @brief Print the adjacency table
 * 
 */
void print_table(int const (&table)[MAX_ROWS][MAX_COLUMNS]);

#endif