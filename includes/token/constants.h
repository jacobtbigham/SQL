#ifndef CONSTANTS_H
#define CONSTANTS_H

/**
 * @file constants.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief Defines the constants for Token and associated classes
 * @version 0.1
 * @date 2021-08-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <string>

// Constants that define the size of the table and the states for the Tokenizer classes
const int MAX_COLUMNS = 129;
const int MAX_BUFFER = 500;
const int MAX_ROWS = 50;

const int UNKNOWN_STATE = -10;
const int COMPLETE_STATE = 40;
const int COMPLETE_STATE_QUOTE = 41;

const int WORD_STATE = 1;
const int WORD_STATE_QUOTE = 2;
const int NUMBER_STATE = 10;
const int NUMBER_FIRST_DECIMAL_FOUND_STATE = 12;
const int PUNC_STATE = 20;
const int SPACE_STATE = 30;
const int PAREN_STATE = 31;

// Token constants
const int LOGICAL = 91;
const int RELATIONAL = 92;
const int TOKEN_STR = 93;

const std::string PUNC_MARKS = "!#$%&'*+,-./:;<=>?@[\\]^_`{|}~";
const std::string LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string NUMBERS = "0123456789";

#endif