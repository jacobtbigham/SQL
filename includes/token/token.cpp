/**
 * @file token.cpp
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief Tokken implementation
 * @version 0.1
 * @date 2021-08-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "./token.h"
#include "./constants.h"
#include <iostream>
#include <vector>
using namespace std;

Token::Token()
{
    _token = "";
    _type = 0;
}

Token::Token(string str, int type)
{
    _token = str;
    _type = type;
}

ostream& operator<<(ostream& outs, const Token& t)
{
    outs << "|" + t.value() << "|";
    return outs;
}

int Token::type() const
{
    return _type;
}

string Token::type_string() const
{
    switch (_type)
    {
        case WORD_STATE:
            return "ALPHA";
        case NUMBER_STATE:
            return "NUMBER";
        case PUNC_STATE:
            return "PUNC";
        case SPACE_STATE:
            return "SPACE";
        case UNKNOWN_STATE:
            return "UNKNOWN";
        default:
            return "NO STATE";
    }
}