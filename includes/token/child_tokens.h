#ifndef CHILD_TOKENS_H
#define CHILD_TOKENS_H

#include "token.h"
#include "constants.h"

class TokenStr: public Token
{
    public:
        TokenStr(std::string value)
        {
            _value = value;
            _type = TOKEN_STR;
        }

        int type() const override { return _type; }

        std::string value() override { return _value; }

    private:
        int _type;
        std::string _value;
};

class Relational: public Token
{
    public:
        Relational(std::string value)
        {
            _value = value;
            _type = RELATIONAL;
        }

        int type() const override { return _type; }

        std::string value() override { return _value; }

    private:
        int _type;
        std::string _value;
};

class Logical: public Token
{
    public:
        Logical(std::string value)
        {
            _value = value;
            _type = LOGICAL;
        }

        int type() const override { return _type; }

        std::string value() override { return _value; }

    private:
        int _type;
        std::string _value;
};

class ResultSet: public Token
{
    public:
        ResultSet(vector<long> values)
        {
            _values = values;
            _type = RESULT_SET;
        }

        int type() const override { return _type; }

        vector<long> values() const override { return _values; }

    private:
        int _type;
        vector<long> _values;
};




#endif