#include "parser.h"
#include <string>
#include <cstring>
#include <iomanip>

using namespace std;

Parser::Parser()
{
    tokenizer = STokenizer();
    parsed = mmap_ss{};
}

Parser::Parser(char s[])
{
    strcpy(curr_input, s);
    tokenizer = STokenizer(s);
    parsed = parse_tree();
}

void Parser::set_string(char s[])
{
    strcpy(curr_input, s);
    tokenizer.set_string(s);
}

mmap_ss Parser::parse_tree()
{
    parsed.clear();
    tokenizer.set_string(curr_input);
    Token t;
    tokenizer >> t;
    if (t.token_str() == "make") // make table
    {
        parsed.insert("command", "make");
        for (int i = 0; i < 3; ++i) tokenizer >> t; // eat " table "
        tokenizer >> t;
        parsed.insert("table_name", t.token_str());
        for (int i = 0; i < 3; ++i) tokenizer >> t; // eat " fields "
        while (tokenizer.more())
        {
            tokenizer >> t;
            if (t.type_string() == "ALPHA")
            {
                parsed.insert("col", t.token_str());
            }
        }
    }
    else if (t.token_str() == "create") // make table
    {
        parsed.insert("command", "create");
        for (int i = 0; i < 3; ++i) tokenizer >> t; // eat " table "
        tokenizer >> t;
        parsed.insert("table_name", t.token_str());
        for (int i = 0; i < 3; ++i) tokenizer >> t; // eat " fields "
        while (tokenizer.more())
        {
            tokenizer >> t;
            if (t.type_string() == "ALPHA")
            {
                parsed.insert("col", t.token_str());
            }
        }
    }
    else if (t.token_str() == "insert") // insert into
    {
        parsed.insert("command", "insert");
        for (int i = 0; i < 3; ++i) tokenizer >> t; // eat " into "
        tokenizer >> t;
        parsed.insert("table_name", t.token_str());
        for (int i = 0; i < 3; ++i) tokenizer >> t; // eat " values "
        while (tokenizer.more())
        {
            tokenizer >> t;
            if (t.type_string() == "ALPHA" || t.type_string() == "NUMBER")
            {
                parsed.insert("values", t.token_str());
            }
        }
    }
    else // select
    {
        parsed.insert("command", "select");
        tokenizer >> t; // eat space
        tokenizer >> t;
        while (t.token_str() != "from")
        {
            if (t.type_string() == "ALPHA" || t.token_str() == "*")
            {
                parsed.insert("fields", t.token_str());
            }
            tokenizer >> t;
        }
        tokenizer >> t; // eat space
        tokenizer >> t;
        parsed.insert("table_name", t.token_str());
        tokenizer >> t;
        tokenizer >> t;
        if (tokenizer.more()) // where present
        {
            parsed.insert("where", "yes");
            for (int i = 0; i < 2; ++i) tokenizer >> t; // eat where
            while (tokenizer.more())
            {
                if (t.type_string() != "SPACE")
                {
                    parsed.insert("condition", t.token_str());
                }
                tokenizer >> t;
            }
        }
    }
    return parsed;
}