#ifndef PARSER_H
#define PARSER_H

/**
 * @file parser.h
 * @author Jacob Bigham (jacob@jacobtbigham.com)
 * @brief Parses text into Tokens based on type (relational, literal, etc.)
 * @version 0.1
 * @date 2021-08-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../bplustree/multimap.h"
#include "../table/typedefs.h"
#include "../token/stokenize.h"
#include "../token/token.h"

/* The member STokenizer breaks an input into SQL-intelligible chunks:
   The parse_tree() function creates and returns a multimap that maps textual
   input from a SQL query to information that can be understood by the Table class.
   For example, the query 'insert into students Jonathan Gomez 3.7 1460', would
   pass to the Table class a multimap of the following form:
   {
       "command": ["insert"],
       "table_name": ["students"],
       "values": ["Jonathan", "Gomez", "3.7", "1460"]
   }
   The Table class processes this multimap accordingly.
*/

class Parser
{
    public:
        /**
         * @brief Construct a new, empty Parser object
         * 
         */
        Parser();

        /**
         * @brief Construct a new Parser object with a given string to parse
         * 
         * @param s the SQL string to parse
         */
        Parser(char s[]);

        /**
         * @brief Parse the input string and create a multimap relating command, table_name, and values
         * 
         * @return mmap_ss the multimap of command, table_name, and relevant values
         */
        mmap_ss parse_tree();

        /**
         * @brief Set the input string
         * 
         * @param s the new input string to process
         */
        void set_string(char s[]);

    private:
        STokenizer tokenizer;   // tokenizes the input string
        mmap_ss parsed;         // the multimap output from the last parse
        char curr_input[300];    // the input string to parse
};

#endif