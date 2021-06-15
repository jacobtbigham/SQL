#ifndef _F_TOKENIZE_H
#define _F_TOKENIZE_H

#include "./token.h"
#include "./constants.h"
#include "./stokenize.h"
#include <fstream>

class FTokenizer
{
public:
    static const int MAX_BLOCK = MAX_BUFFER;
    FTokenizer(char* fname);
    FTokenizer(std::string fname);
    FTokenizer();
    Token next_token();
    bool more() { return _more; }        //returns the current value of _more
    int pos() { return _pos; }          //returns the value of _pos
    int block_pos() { return _blockPos; }     //returns the value of _blockPos
    friend FTokenizer& operator >> (FTokenizer& f, Token& t);
private:
    bool get_new_block(); //gets the new block from the file
    std::ifstream _f;   //file being tokenized
    STokenizer _stk;     //The STokenizer object to tokenize current block
    int _pos;           //Current position in the file
    int _blockPos;      //Current position in the current block
    bool _more;         //false if last token of the last block
                        //  has been processed and now we are at
                        //  the end of the last block.
};

#endif