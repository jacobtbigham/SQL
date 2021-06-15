#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <string>

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

const int RESULT_SET = 91;
const int LOGICAL = 92;
const int RELATIONAL = 93;
const int TOKEN_STR = 94;

const std::string PUNC_MARKS = "!#$%&'*+,-./:;<=>?@[\\]^_`{|}~";
const std::string LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string NUMBERS = "0123456789";

#endif