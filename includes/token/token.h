#ifndef TOKEN_H
#define TOKEN_H

/**
 * @file token.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief The Token class
 * @version 0.1
 * @date 2021-08-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <iostream>
#include <vector>

/* A Token encapsulates text extracted from a file or other text source.
   It contains a value and a type (as defined in constants.h).
 */

/* FUNCTIONS IN THE PUBLIC INTERFACE:
    Token()
    Token(std::string str, int type)
    friend std::ostream& operator <<(std::ostream& outs, const Token& t)
    virtual int type() const
    virtual std::string value()
    virtual std::vector<long> values() const
    std::string type_string() const
    std::string token_str() const
*/

class Token
{
public:
    /**
     * @brief Construct a new Token object
     * 
     */
    Token();

    /**
     * @brief Construct a new Token object
     * 
     * @param str the value of the Token
     * @param type the type of the Token
     */
    Token(std::string str, int type);

    /**
     * @brief Prints the value and type of a Token to the given output stream
     * 
     * @param outs the output stream
     * @param t the Token
     * @return std::ostream& the output stream to which the Token was printed
     */
    friend std::ostream& operator <<(std::ostream& outs, const Token& t);

    /**
     * @brief Returns the Token type
     * 
     * @return int the Token type
     */
    virtual int type() const;

    /**
     * @brief Returns the value stored in the Token
     * 
     * @return std::string the value stored in the Token
     */
    virtual std::string value() const { return _token; }

    
    // WHY IS THIS HERE?
    // virtual std::vector<long> values() const { return {}; }

    /**
     * @brief Returns a string corresponding to the Token type
     * 
     * @return std::string the type of Token as text
     */
    std::string type_string() const;

private:
    std::string _token; // the value stored in the Token
    int _type;          // the type of the Token
};

#endif