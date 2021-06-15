#include "./constants.h"
#include "./state_machine_functions.h"
#include <iostream>

void initialize_table(int (&table)[MAX_ROWS][MAX_COLUMNS])
{
    for (int row = 0; row < MAX_ROWS; ++row)
    {
        for (int col = 0; col < MAX_COLUMNS; ++col)
        {
            table[row][col] = UNKNOWN_STATE;
        }
    }
}

void add_rule(int (&table)[MAX_ROWS][MAX_COLUMNS], int start_state, int input, int end_state)
{
    table[start_state][input] = end_state;
}

int next_state(int const (&table)[MAX_ROWS][MAX_COLUMNS], int start_state, int input)
{
    return table[start_state][input];
}

void print_table(int const (&table)[MAX_ROWS][MAX_COLUMNS])
{
    for (int row = 0; row < MAX_ROWS; ++row)
    {
        for (int col = 0; col < MAX_COLUMNS; ++col)
        {
            std::cout << table[row][col];
        }
        std::cout << std::endl;
    }
}