#include "./stokenize.h"
#include "./constants.h"
#include "./token.h"
#include "./state_machine_functions.h"
#include <cassert>
#include <cctype>

using namespace std;

STokenizer::STokenizer()
{
    strcpy(_buffer, "");
    _pos = 0;
    make_table();
}

STokenizer::STokenizer(char str[])
{
    assert(strlen(str) <= MAX_BUFFER);
    strcpy(_buffer, str);
    _pos = 0;
    make_table();
}

void STokenizer::make_table()
{
    initialize_table(_table);
    //ALPHA --> ALPHA (in state ALPHA and encounter an ALPHA)
    for (char letter : LETTERS)
    {
        add_rule(_table, WORD_STATE, letter, WORD_STATE);
    }
    // ALPHA --> NUMERIC
    for (char number : NUMBERS)
    {
        add_rule(_table, WORD_STATE, number, COMPLETE_STATE);
    }
    // ALPHA --> PUNC
    for (char punc : PUNC_MARKS)
    {
        add_rule(_table, WORD_STATE, punc, COMPLETE_STATE);
    }
    // ALPHA --> SPACE
    add_rule(_table, WORD_STATE, ' ', COMPLETE_STATE);
    
    // NUMERIC --> NUMERIC
    for (char number : NUMBERS)
    {
        add_rule(_table, NUMBER_STATE, number, NUMBER_STATE);
    }
    // NUMERIC --> ALPHA
    for (char letter : LETTERS)
    {
        add_rule(_table, NUMBER_STATE, letter, COMPLETE_STATE);
    }
    // NUMERIC --> PUNC
    for (char punc : PUNC_MARKS)
    {
        add_rule(_table, NUMBER_STATE, punc, COMPLETE_STATE);
    }
    // NUMERIC --> SPACE
    add_rule(_table, NUMBER_STATE, ' ', COMPLETE_STATE);
    // NUMERIC --> FIRST DECIMAL
    add_rule(_table, NUMBER_STATE, '.', NUMBER_FIRST_DECIMAL_FOUND_STATE);
    
    // NUMERIC_DECIMAL --> NUMERIC_DECIMAL
    for (char number : NUMBERS)
    {
        add_rule(_table, NUMBER_FIRST_DECIMAL_FOUND_STATE, number, NUMBER_FIRST_DECIMAL_FOUND_STATE);
    }
    // NUMERIC_DECIMAL --> ALPHA
    for (char letter : LETTERS)
    {
        add_rule(_table, NUMBER_FIRST_DECIMAL_FOUND_STATE, letter, COMPLETE_STATE);
    }
    // NUMERIC_DECIMAL --> PUNC
    for (char punc : PUNC_MARKS)
    {
        add_rule(_table, NUMBER_FIRST_DECIMAL_FOUND_STATE, punc, COMPLETE_STATE);
    }
    // NUMERIC_DECIMAL --> SPACE
    add_rule(_table, NUMBER_FIRST_DECIMAL_FOUND_STATE, ' ', COMPLETE_STATE);

    // PUNC --> PUNC
    for (char punc : PUNC_MARKS)
    {
        add_rule(_table, PUNC_STATE, punc, PUNC_STATE);
    }
    // PUNC --> ALPHA
    for (char letter : LETTERS)
    {
        add_rule(_table, PUNC_STATE, letter, COMPLETE_STATE);
    }
    // PUNC --> NUMERIC
    for (char number : NUMBERS)
    {
        add_rule(_table, PUNC_STATE, number, COMPLETE_STATE);
    }
    // PUNC --> SPACE
    add_rule(_table, PUNC_STATE, ' ', COMPLETE_STATE);
    
    // SPACE --> SPACE
    add_rule(_table, SPACE_STATE, ' ', SPACE_STATE);
    // SPACE --> LETTER
    for (char letter : LETTERS)
    {
        add_rule(_table, SPACE_STATE, letter, COMPLETE_STATE);
    }
    // SPACE --> NUMBER
    for (char number : NUMBERS)
    {
        add_rule(_table, SPACE_STATE, number, COMPLETE_STATE);
    }
    // SPACE --> PUNC
    for (char punc : PUNC_MARKS)
    {
        add_rule(_table, SPACE_STATE, punc, COMPLETE_STATE);
    }

    //rules for quotation marks:
    add_rule(_table, WORD_STATE, '"', COMPLETE_STATE);
    add_rule(_table, NUMBER_STATE, '"', COMPLETE_STATE);
    add_rule(_table, NUMBER_FIRST_DECIMAL_FOUND_STATE, '"', COMPLETE_STATE);
    add_rule(_table, PUNC_STATE, '"', COMPLETE_STATE);
    add_rule(_table, SPACE_STATE, '"', COMPLETE_STATE);
    add_rule(_table, WORD_STATE_QUOTE, '"', COMPLETE_STATE_QUOTE);

    add_rule(_table, WORD_STATE_QUOTE, ' ', WORD_STATE_QUOTE);
    for (char letter : LETTERS)
    {
        add_rule(_table, WORD_STATE_QUOTE, letter, WORD_STATE_QUOTE);
    }
    for (char number : NUMBERS)
    {
        add_rule(_table, WORD_STATE_QUOTE, number, WORD_STATE_QUOTE);
    }
    for (char punc : PUNC_MARKS)
    {
        add_rule(_table, WORD_STATE_QUOTE, punc, WORD_STATE_QUOTE);
    }

    // parentheses
    add_rule(_table, WORD_STATE, '(', COMPLETE_STATE);
    add_rule(_table, NUMBER_STATE, '(', COMPLETE_STATE);
    add_rule(_table, NUMBER_FIRST_DECIMAL_FOUND_STATE, '(', COMPLETE_STATE);
    add_rule(_table, PUNC_STATE, '(', COMPLETE_STATE);
    add_rule(_table, SPACE_STATE, '(', COMPLETE_STATE);
    add_rule(_table, WORD_STATE_QUOTE, '(', COMPLETE_STATE_QUOTE);
    add_rule(_table, WORD_STATE, ')', COMPLETE_STATE);
    add_rule(_table, NUMBER_STATE, ')', COMPLETE_STATE);
    add_rule(_table, NUMBER_FIRST_DECIMAL_FOUND_STATE, ')', COMPLETE_STATE);
    add_rule(_table, PUNC_STATE, ')', COMPLETE_STATE);
    add_rule(_table, SPACE_STATE, ')', COMPLETE_STATE);
    add_rule(_table, WORD_STATE_QUOTE, ')', COMPLETE_STATE_QUOTE);

    add_rule(_table, PAREN_STATE, ' ', COMPLETE_STATE);
    for (char letter : LETTERS)
    {
        add_rule(_table, PAREN_STATE, letter, COMPLETE_STATE);
    }
    for (char number : NUMBERS)
    {
        add_rule(_table, PAREN_STATE, number, COMPLETE_STATE);
    }
    for (char punc : PUNC_MARKS)
    {
        add_rule(_table, PAREN_STATE, punc, COMPLETE_STATE);
    }

}

void STokenizer::set_string(char str[])
{
    assert(strlen(str) <= MAX_BUFFER);
    strcpy(_buffer, str);
    _pos = 0;
}

bool STokenizer::get_token(int start_state, string& token)
{
    if (done()) { return false; }
    
    char current_char = _buffer[_pos];
    string result = string(1, current_char);
    char next_char;
    int current_state = start_state;
    if (start_state == UNKNOWN_STATE)
    {
        ++_pos;
        token = string(1, current_char);
        return true;
    }
    while (current_state != UNKNOWN_STATE && current_state != COMPLETE_STATE && current_state != COMPLETE_STATE_QUOTE)
    {
        ++_pos;
        if (_pos >= strlen(_buffer))
        {
            // if we encounter a period after a number at the end the buffer
            if (current_state == NUMBER_FIRST_DECIMAL_FOUND_STATE)
            {
                if (result.back() == '.')
                {
                    result = result.substr(0, result.length() - 1);
                    token = result;
                    char period[] = ".";
                    set_string(period);
                    return true;
                }
            }
            else
            {
                token = result;
                return true;
            }
            
        }
        next_char = _buffer[_pos];
        int next = next_state(_table, current_state, next_char);
        //deal with periods after numbers that don't lead to more numbers
        if (current_state == NUMBER_FIRST_DECIMAL_FOUND_STATE)
        {
            if (result.back() == '.' && next != NUMBER_FIRST_DECIMAL_FOUND_STATE)
            {
                result = result.substr(0, result.length() - 1);
                token = result;
                --_pos;
                return true;
            }
        }

        current_state = next;
        if (current_state != UNKNOWN_STATE && current_state != COMPLETE_STATE && current_state != COMPLETE_STATE_QUOTE)
        {
            result += next_char;
        }
        if (current_state == COMPLETE_STATE_QUOTE)
        {
            _pos++; // don't ingest the end quote either
        }
    }
    token = result;
    return true;

}

STokenizer& operator>>(STokenizer& s, Token& t)
{
    if (s.done()) { return s; }
    int start_state;
    char current_char = s._buffer[s._pos];
    if (isdigit(current_char)) { start_state = NUMBER_STATE; }
    else if (current_char == '"') { start_state = WORD_STATE_QUOTE; ++s._pos;}
    else if (isspace(current_char)) { start_state = SPACE_STATE; }
    else if (isalpha(current_char)) { start_state = WORD_STATE; }
    else if (current_char == '(' || current_char == ')') { start_state = PAREN_STATE; }
    else if (ispunct(current_char)) { start_state = PUNC_STATE; }
    else { start_state = UNKNOWN_STATE; }
    string token_string;
    if (s.get_token(start_state, token_string))
    {
        char first_char = token_string.at(0);
        if (isdigit(first_char)) { t = Token(token_string, NUMBER_STATE); }
        else if (isspace(first_char)) { t = Token(token_string, SPACE_STATE); }
        else if (isalpha(first_char)) { t = Token(token_string, WORD_STATE); }
        else if (ispunct(first_char)) { t = Token(token_string, PUNC_STATE); }
        else { t = Token(token_string, UNKNOWN_STATE); }
    }
    return s;
}

