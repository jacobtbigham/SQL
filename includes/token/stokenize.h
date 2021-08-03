#ifndef STOKENIZE_H
#define STOKENIZE_H

/**
 * @file stokenize.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief The STokenizer class (String Tokenizer)
 * @version 0.1
 * @date 2021-08-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <iostream>
#include "./constants.h"
#include "./token.h"
#include <cstring>
#include <string>

/* The STokenizer class chunks text input into Tokens based on rules defined
   in the implementation. This specific class has been tuned to process SQL
   commands.
*/

/* FUNCTIONS IN THE PUBLIC INTERFACE:
    STokenizer()
    STokenizer(char str[])
    bool done() const
    bool more() const
    friend STokenizer& operator >> (STokenizer& s, Token& t)
    void set_string(char str[])
    void print_buffer()
*/


class STokenizer
{
public:
    /**
     * @brief Construct a new, empty STokenizer object
     * 
     */
    STokenizer();

    /**
     * @brief Construct a new STokenizer object with given text to process
     * 
     * @param str the text to process/tokenize
     */
    STokenizer(char str[]);

    /**
     * @brief Returns whether there are no more tokens to extract from the text
     * 
     * @return true there are NOT more tokens
     * @return false there are more tokens
     */
    bool done() const { return ! more(); }

    /**
     * @brief Returns whether there are more tokens to extract from the text
     * 
     * @return true there are more tokens
     * @return false there are NOT more tokens
     */
    bool more() const { return _pos <= strlen(_buffer); }

    /**
     * @brief Extracts the next token from the STokenizer
     * 
     * @param s the STokenizer to extract from
     * @param t the Token to extract into
     * @return STokenizer& the STokenizer from which the Token was extracted
     */
    friend STokenizer& operator >> (STokenizer& s, Token& t);

    /**
     * @brief Set the STokenizer's text to process
     * 
     * @param str the new text for the STokenizer to process/tokenize
     */
    void set_string(char str[]);

    /**
     * @brief Print the buffer/current text being processed
     * 
     */
    void print_buffer() { std::cout << _buffer << std::endl; }

private:
    void make_table();
    bool get_token(int start_state, std::string& token);
    char _buffer[MAX_BUFFER];           //input string
    int _pos;                           //current position in the string
    int _table[MAX_ROWS][MAX_COLUMNS];
    bool _last_token_served;
};

#endif