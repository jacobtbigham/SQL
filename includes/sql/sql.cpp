#include "./sql.h"
#include <cstring>
#include <string>

using namespace std;


SQL::SQL()
{
    parser = Parser{};
    last_recnos = vector<long>{};
}

Table SQL::command(string s)
{
    char c_s[300];
    strcpy(c_s, s.c_str());
    parser.set_string(c_s);
    MMap<string, string> parsed = parser.parse_tree();
    //parsed.print_lookup();
    string command = parsed.get("command").at(0);
    string name = parsed.get("table_name").at(0);
    if (command == "create" || command == "make")
    {
        vector<string> fields = parsed.get("col");
        Table table = Table(name, fields);
        return table;
    }
    else if (command == "insert")
    {
        Table table = Table(name);
        table.insert_into(parsed.get("values"));
        return table;
    }
    else //command = select
    {
        Table table = Table(name);
        vector<string> fields = parsed.get("fields");
        vector<string> conditions{};
        if (parsed.contains("where") && parsed.get("where").at(0) == "yes")
        {
            conditions = parsed.get("condition");
        }
        Table result = table.select(fields, conditions);
        last_recnos = table.select_recnos();
        return result;
    }
}