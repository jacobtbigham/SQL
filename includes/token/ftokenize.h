#ifndef F_TOKENIZE_H
#define F_TOKENIZE_H

/**
 * @file ftokenize.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief Tokenizes input files into words defined by a state machine
 * @version 0.1
 * @date 2021-08-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "./token.h"
#include "./constants.h"
#include "./stokenize.h"
#include <fstream>


/* The FTokenizer class splits the text from a file into chunks.
   An FTokenizer object reads the text from a file in groups of MAX_BUFFER characters.
   Once the current buffer is tokenized (exhausted), then the buffer is refreshed with more
   characters from the file——until the file has no characters remaining for extraction.
   Because the tokenizing is carried out by an STokenizer object, the FTokenizer will
   tokenize text based on the rules outlined in the STokenizer class——which, in this repository,
   is adapted to split SQL query input from formatted files (see README.txt).
*/

/* FUNCTIONS/VARIABLES IN THE PUBLIC INTERFACE:
    static const int MAX_BLOCK
    FTokenizer(char* fname)
    FTokenizer(std::string fname)
    FTokenizer()
    Token next_token()
    bool more()
    int pos()
    int block_pos()
    friend FTokenizer& operator >> (FTokenizer& f, Token& t)
*/

class FTokenizer
{
public:
    /**
     * @brief Defines the maximum buffer size
     * 
     */
    static const int MAX_BLOCK = MAX_BUFFER;

    /**
     * @brief Construct a new FTokenizer object
     * 
     * @param fname the file name
     */
    FTokenizer(char* fname);

    /**
     * @brief Construct a new FTokenizer object
     * 
     * @param fname the filename
     */
    FTokenizer(std::string fname);

    /**
     * @brief Construct a new FTokenizer object
     * 
     */
    FTokenizer();

    /**
     * @brief Retrieve and return the next Token from the file
     * 
     * @return Token 
     */
    Token next_token();

    /**
     * @brief Return whether there exist more Tokens to extract
     * 
     * @return true there are more Tokens to extract
     * @return false the FTokenizer has reached the end of the file
     */
    bool more() { return _more; }

    /**
     * @brief Returns the current position in the file
     * 
     * @return int the current position in the file
     */
    int pos() { return _pos; }

    /**
     * @brief Returns the current position in the block
     * 
     * @return int 
     */
    int block_pos() { return _blockPos; } 

    /**
     * @brief Extract operator for retrieving the next token
     * 
     * @param f the FTokenizer to extract from
     * @param t the Token to extract into
     * @return FTokenizer& the FTokenizer from which the Token was extract
     */
    friend FTokenizer& operator >> (FTokenizer& f, Token& t);
    
private:
    bool get_new_block();  // gets the new block from the file
    std::ifstream _f;      // file being tokenized
    STokenizer _stk;       // the STokenizer to tokenize the current block
    int _pos;              // current position in the file
    int _blockPos;         // current position in the current block
    bool _more;            // false only when the end is reached and no tokens remain
};

#endif