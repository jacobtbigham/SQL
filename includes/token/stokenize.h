#ifndef _STOKENIZE_H
#define _STOKENIZE_H

#include <iostream>
#include "./constants.h"
#include "./token.h"
#include <cstring>
#include <string>


class STokenizer
{
public:
    STokenizer();
    STokenizer(char str[]);
    bool done() const { return ! more(); }             //true: there are no more tokens
    bool more() const { return _pos <= strlen(_buffer); }             //true: there are more tokens
    //

    //---------------
    //extract one token (very similar to the way cin >> works)
    friend STokenizer& operator >> (STokenizer& s, Token& t);

    //set a new string as the input string
    void set_string(char str[]);
    void print_buffer() { std::cout << _buffer << std::endl; }

private:
    //create table for all the tokens we will recognize
    //                      (e.g. doubles, words, etc.)
    void make_table();

    //extract the longest string that matches
    //     one of the acceptable token types
    bool get_token(int start_state, std::string& token);
    //---------------------------------
    char _buffer[MAX_BUFFER];       //input string
    int _pos;                       //current position in the string
    int _table[MAX_ROWS][MAX_COLUMNS];
    bool _last_token_served;
};

#endif