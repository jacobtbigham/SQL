#ifndef _TOKEN_H
#define _TOKEN_H

#include <iostream>
#include <vector>

class Token
{
public:
    Token();
    Token(std::string str, int type);
    friend std::ostream& operator <<(std::ostream& outs, const Token& t);
    virtual int type() const;
    virtual std::string value() { return ""; }
    virtual std::vector<long> values() const { return {}; }
    std::string type_string() const;
    std::string token_str() const;
private:
    std::string _token;
    int _type;
};

#endif