/**
 * @file parser.cpp
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */
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

//helpers for parse_tree()
void map_from_command(STokenizer& tokenizer, Token& t, mmap_ss& parsed, string command);
void map_from_select(STokenizer& tokenizer, Token& t, mmap_ss& parsed);

mmap_ss Parser::parse_tree()
{
    parsed.clear();
    tokenizer.set_string(curr_input);
    Token t;
    tokenizer >> t;
    if (t.value() == "make") // make table
    {
        map_from_command(tokenizer, t, parsed, "make");
    }
    else if (t.value() == "create") // make table
    {
        map_from_command(tokenizer, t, parsed, "create");
    }
    else if (t.value() == "insert") // insert into
    {
        map_from_command(tokenizer, t, parsed, "insert");
    }
    else // select
    {
        map_from_select(tokenizer, t, parsed);
    }
    return parsed;
}

void map_from_command(STokenizer& tokenizer, Token& t, mmap_ss& parsed, string command)
{
    parsed.insert("command", command);
    for (int i = 0; i < 3; ++i) tokenizer >> t; // eat " table "
    tokenizer >> t;
    parsed.insert("table_name", t.value());
    for (int i = 0; i < 3; ++i) tokenizer >> t; // eat " fields "
    if (command == "make" || command == "create")
    {
        while (tokenizer.more())
        {
            tokenizer >> t;
            if (t.type_string() == "ALPHA")
            {
                parsed.insert("col", t.value());
            }
        }
    }
    else // insert
    {
        while (tokenizer.more())
        {
            tokenizer >> t;
            if (t.type_string() == "ALPHA" || t.type_string() == "NUMBER")
            {
                parsed.insert("values", t.value());
            }
        }
    }
}

void map_from_select(STokenizer& tokenizer, Token& t, mmap_ss& parsed)
{
    parsed.insert("command", "select");
    tokenizer >> t; // eat space
    tokenizer >> t;
    while (t.value() != "from")
    {
        if (t.type_string() == "ALPHA" || t.value() == "*")
        {
            parsed.insert("fields", t.value());
        }
        tokenizer >> t;
    }
    tokenizer >> t; // eat space
    tokenizer >> t;
    parsed.insert("table_name", t.value());
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
                parsed.insert("condition", t.value());
            }
            tokenizer >> t;
        }
    }
}