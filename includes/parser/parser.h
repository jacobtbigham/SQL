#ifndef PARSER_H
#define PARSER_H

#include "../bplustree/multimap.h"
#include "../table/typedefs.h"
#include "../token/stokenize.h"
#include "../token/token.h"

class Parser
{
    public:
        Parser();
        Parser(char s[]);
        mmap_ss parse_tree();
        void set_string(char s[]);
    private:
        STokenizer tokenizer;
        mmap_ss parsed;
        char curr_input[300];
};

#endif