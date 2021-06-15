#ifndef _SQL_H
#define _SQL_H

#include "../parser/parser.h"
#include "../table/table.h"

class SQL
{
    public:
        SQL();
        Table command(std::string s);
        std::vector<long> select_recnos() { return last_recnos; }
    private:
        Parser parser;
        std::vector<long> last_recnos;
};







#endif