#ifndef CHILD_TOKENS_H
#define CHILD_TOKENS_H

/**
 * @file child_tokens.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief Defines string, relational, and logical tokens that are used to process SQL queries.
 * @version 0.1
 * @date 2021-08-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "token.h"
#include "constants.h"

class TokenStr: public Token
{
    public:
        /**
         * @brief Construct a new TokenStr (literal) object
         * 
         * @param value the literal value of the Token
         */
        TokenStr(std::string value)
        {
            _value = value;
        }

        /**
         * @brief Returns the type value corresponding to a TokenStr
         * 
         * @return int the constant TOKEN_STR
         */
        int type() const override { return TOKEN_STR; }

        /**
         * @brief Returns the literal value of the Token
         * 
         * @return std::string the literal value of the Token
         */
        std::string value() const override { return _value; }

    private:
        std::string _value;
};

class Relational: public Token
{
    public:
        /**
         * @brief Construct a new Relational (<, >, =) object
         * 
         * @param value the relational value for this Token
         */
        Relational(std::string value)
        {
            _value = value;
        }

        /**
         * @brief Returns the type value corresponding to a Relational Token
         * 
         * @return int the constant RELATIONAL
         */
        int type() const override { return RELATIONAL; }

        /**
         * @brief Returns the relational value of the Token
         * 
         * @return std::string the relational value of the Token
         */
        std::string value() const override { return _value; }

    private:
        std::string _value;
};

class Logical: public Token
{
    public:
        /**
         * @brief Construct a new Logical (and, or) Token object
         * 
         * @param value the logcal operator for this Token
         */
        Logical(std::string value)
        {
            _value = value;
        }

        /**
         * @brief Return the type value corresponding to a Logical token
         * 
         * @return int the constant LOGICAL
         */
        int type() const override { return LOGICAL; }
        
        /**
         * @brief Returns the logical value of the Token
         * 
         * @return std::string the logical value of the Token
         */
        std::string value() const override { return _value; }

    private:
        std::string _value;
};


#endif