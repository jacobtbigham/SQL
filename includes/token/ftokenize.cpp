/**
 * @file ftokenize.cpp
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief File Tokenizer implmentation
 * @version 0.1
 * @date 2021-08-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "./ftokenize.h"
#include "./stokenize.h"
#include "./constants.h"
using namespace std;

FTokenizer::FTokenizer(char* fname)
{
    _f = ifstream(fname);
    _pos = 0;
    _blockPos = 0;
    _stk = STokenizer();
    get_new_block();
}

Token FTokenizer::next_token()
{
    Token t;
    _stk >> t;
    if (_stk.done())
    {
        if (! get_new_block())
        {
            _more = false;
        }
    }
    return t;
}

FTokenizer& operator >> (FTokenizer& f, Token& t)
{
    t = f.next_token();
    return f;
}

bool FTokenizer::get_new_block()
{
    if (_f)
    {
        char* block = new char[MAX_BLOCK + 1];
        for (int i = 0; i < MAX_BLOCK + 1; i++) // otherwise read() doesn't flush the memory
        {
            block[i] = '\0';
        }
        _f.read(block, MAX_BLOCK);
        int block_size = strlen(block);
        _pos += block_size;
        if (block_size == 0)
        {
            delete[] block;
            return false;
        }
        else
        {
            _stk.set_string(block);
            _blockPos = 0;
            delete[] block;
            return true;
        }
    }
    else
    {
        return false;
    }
}