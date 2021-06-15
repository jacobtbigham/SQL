#include "table.h"
#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <algorithm>
#include <vector>

#include "../queue/MyQueue.h"

using namespace std;
int Table::select_number = 1;

inline std::string trim(std::string& str)
{
    str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
    str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
    return str;
}

Table::Table()
{
    FileRecord::reset_record_number();
    name = "blank_table";
    num_vals = 0;
    data = {};
    data_list = {};
}

Table::Table(string _name)
{
    FileRecord::reset_record_number();
    data = {};
    data_list = {};
    name = _name;
    int rec_index = 0;
    fstream f;
    open_fileRW(f, (_name + ".bin").c_str());
    FileRecord rec;
    rec.read(f, rec_index);
    vectorstr fields = rec.get_info();
    for (int i = 0; i < fields.size(); ++i)
    {
        string field = trim(fields.at(i));
        data.push_back(MMap<string, int>());
        column_to_index.insert(field, i);
        index_to_column.push_back(field);
    }
    bool done = false;
    num_vals = 0;
    while (! done)
    {
        ++rec_index;
        rec = FileRecord();
        if (rec.read(f, rec_index) == 0) { done = true; }
        else
        {
            insert_no_write(rec.get_info(), rec_index - 1);
        }
    }
    f.close();
}

Table::Table(string _name, vectorstr fields)
{
    FileRecord::reset_record_number();
    data = {};
    data_list = {};
    name = _name;
    fstream f;
    open_fileW(f, (name + ".bin").c_str());
    FileRecord rec(fields);
    rec.write(f);
    for (int i = 0; i < fields.size(); ++i)
    {
        data.push_back(MMap<string, int>());
        column_to_index.insert(fields.at(i), i);
        index_to_column.push_back(fields.at(i));
    }
    num_vals = 0;
    f.close();
}

// MODIFIERS
void Table::insert_into(vectorstr row)
{
    fstream f;
    open_fileRW(f, (name + ".bin").c_str());
    FileRecord rec(row);
    int recno = rec.write(f);
    for (int i = 0; i < row.size(); ++i)
    {
        data.at(i).insert(row.at(i), recno);
    }
    data_list.push_back(row);
    ++num_vals;
    f.close();
}

void Table::insert_no_write(vectorstr row, int _recno)
{
    for (int i = 0; i < row.size(); ++i)
    {
        data.at(i).insert(trim(row.at(i)), _recno);
    }
    data_list.push_back(row);
    ++num_vals;
}

// ACCESSORS
Table Table::select(vectorstr fields, string field, string op, string operand)
{
    string name = "_select_table_" + to_string(select_number++);
    if (fields.at(0) == "*") { fields = index_to_column; }
    Table result = Table(name, fields);
    vector<int> selected_recnos{};
    int index_of_field = column_to_index.get(field);
    MMap<string, int> field_data = data.at(index_of_field);
    if (op == "=")
    {
        if (field_data.contains(operand))
        {
            selected_recnos = field_data.get(operand);
        }
        else { selected_recnos = {}; }
    }
    else
    {
        if (op == ">")
        {
            MMap<string, int>::Iterator start = field_data.upper_bound(operand);
            MMap<string, int>::Iterator end = field_data.end();
            while (start != end)
            {
                for (int i = 0; i < (*start).value_list.size(); ++i)
                {
                    selected_recnos.push_back((*start).value_list.at(i));
                }
                ++start;
            }
        }
        else if (op == "<")
        {
            MMap<string, int>::Iterator start = field_data.begin();
            MMap<string, int>::Iterator end = field_data.lower_bound(operand);
            while (start != end)
            {
                for (int i = 0; i < (*start).value_list.size(); ++i)
                {
                    selected_recnos.push_back((*start).value_list.at(i));
                }
                ++start;
            }
        }
        else if (op == "<=")
        {
            MMap<string, int>::Iterator start = field_data.begin();
            MMap<string, int>::Iterator end = field_data.upper_bound(operand);
            while (start != end)
            {
                for (int i = 0; i < (*start).value_list.size(); ++i)
                {
                    selected_recnos.push_back((*start).value_list.at(i));
                }
                ++start;
            }
        }
        else // op == ">="
        {
            MMap<string, int>::Iterator start = field_data.lower_bound(operand);
            MMap<string, int>::Iterator end = field_data.end();
            while (start != end)
            {
                for (int i = 0; i < (*start).value_list.size(); ++i)
                {
                    selected_recnos.push_back((*start).value_list.at(i));
                }
                ++start;
            }
        }
    }
    vector<int> fields_to_select{};
    if (fields.at(0) == "*") // select all
    {
        for (int i = 0; i < column_to_index.size(); ++i)
        {
            fields_to_select.push_back(i);
        }
    }
    else
    {
        for (int i = 0; i < fields.size(); ++i)
        {
            if (column_to_index.contains(fields.at(i)))
            {
                fields_to_select.push_back(column_to_index.get(fields.at(i)));
            }
            else // requested a column that isn't in the table
            {
                last_selected_recnos = vector<long>{};
                return result;
            }
        }
    }
    for (int i = 0; i < selected_recnos.size(); ++i)
    {
        vectorstr row_values{};
        for (int j = 0; j < fields_to_select.size(); ++j)
        {
            row_values.push_back(data_list.at(selected_recnos.at(i)).at(fields_to_select.at(j)));
        }
        result.insert_no_write(row_values, i);
    }
    last_selected_recnos = vector<long>{};
    if (selected_recnos.size() == 0) // the result set is empty
    {
        return result;
    }
    for (int i = 0; i < selected_recnos.size(); ++i)
    {
        last_selected_recnos.push_back(selected_recnos.at(i));
    }
    return result;
}

Table Table::select(vectorstr fields, Queue<Token*> args) // queue already in postfix
{
    string name = "_select_table_" + to_string(select_number++);
    if (fields.at(0) == "*") { fields = index_to_column; }
    Table result = Table(name, fields);
    Stack< vector<long> > selected_recnos{};
    vector<long> selection{};
    if (! args.empty())
    {
        while (! args.empty())
        {
            if (args.front()->type() == TOKEN_STR)
            {
                string field = args.pop()->value();
                if (! column_to_index.contains(field)) // requested a condition on a nonexistent column
                {
                    last_selected_recnos = vector<long>{};
                    return result;
                }
                string operand = args.pop()->value();
                string op = args.pop()->value();
                Table temp = select(fields, field, op, operand);
                selected_recnos.push(last_selected_recnos);
            }
            else // RELATIONAL
            {
                string rel = args.pop()->value();
                vector<long> first = selected_recnos.pop();
                std::sort(first.begin(), first.end());
                vector<long> second = selected_recnos.pop();
                std::sort(second.begin(), second.end());
                vector<long> new_recnos{};
                if (rel == "and")
                {
                    set_intersection(first.begin(), first.end(),
                                    second.begin(), second.end(),
                                    std::back_inserter(new_recnos));
                }
                else // OR
                {
                    set_union(first.begin(), first.end(),
                                    second.begin(), second.end(),
                                    std::back_inserter(new_recnos));
                }
                selected_recnos.push(new_recnos);
            }
        }
        if (! selected_recnos.empty())
        {
            selection = selected_recnos.pop();
        }
    }
    else // no conditions
    {
        for (int i = 0; i < data_list.size(); ++i)
        {
            selection.push_back(i);
        }
    }
    last_selected_recnos = selection; 
    vector<int> fields_to_select{};
    if (fields.at(0) == "*") // select all
    {
        for (int i = 0; i < column_to_index.size(); ++i)
        {   
            fields_to_select.push_back(i);
        }
    }
    else
    {
        for (int i = 0; i < fields.size(); ++i)
        {
            if (column_to_index.contains(fields.at(i)))
            {
                fields_to_select.push_back(column_to_index.get(fields.at(i)));
            }
            else // requested a column that isn't in the table
            {
                last_selected_recnos = vector<long>{};
                return result;
            }
        }
    }
    if (last_selected_recnos.size() == 0) // the result set is empty
    {
        return result;
    }
    for (int i = 0; i < last_selected_recnos.size(); ++i)
    {
        vectorstr row_values;
        for (int j = 0; j < fields_to_select.size(); ++j)
        {
            row_values.push_back(data_list.at(last_selected_recnos.at(i)).at(fields_to_select.at(j)));
        }
        result.insert_no_write(row_values, i);
    }
    return result;
}

Table Table::select(vectorstr fields, vectorstr conditions)
{
    Queue<Token*> output;
    if (conditions.size() == 0)
    {
        return select(fields, output);
    }
    Stack<Token*> operators;
    for (int i = 0; i < conditions.size(); ++i)
    {
        string curr = conditions.at(i);
        if (curr == "and")
        {
            while (! operators.empty())
            {
                string top_value = operators.top()->value();
                if (top_value != "(" && operators.top()->type() == LOGICAL)
                {
                    output.push(operators.pop());
                }
                else { break; }
            }
            operators.push(new Relational(curr));
        }
        else if (curr == "or")
        {
            while (! operators.empty())
            {
                string top_value = operators.top()->value();
                if (top_value != "(" && (operators.top()->type() == LOGICAL || top_value == "and"))
                {
                    output.push(operators.pop());
                }
                else { break; }
            }
            operators.push(new Relational(curr));
        }
        else if (curr == "=" || curr == "<" || curr == "<=" || curr == ">" || curr == ">=")
        {
            if (! operators.empty())
            {
                string top_value = operators.top()->value();
                if (top_value == "=" || top_value == "<" || top_value == "<=" || top_value == ">" || top_value == ">=")
                {
                    output.push(operators.pop());
                }
            }
            operators.push(new Logical(curr));
        }
        else if (curr == "(")
        {
            operators.push(new TokenStr("("));
        }
        else if (curr == ")")
        {
            while (! operators.empty())
            {
                if (operators.top()->value() != "(")
                {
                    output.push(operators.pop());
                }
                else { break; }
            }
            operators.pop(); // remove the top (
        }
        else { output.push(new TokenStr(curr)); }
        // cout << "After step " << i << " (" << curr << "): " << endl;
        // cout << "Output: ";
        // Queue<Token*>::Iterator start = output.begin();
        // Queue<Token*>::Iterator finish = output.end();
        // while (start != finish) { cout << (*start)->value(); start++;}
        // cout << endl;
        // cout << "Operators: ";
        // Stack<Token*>::Iterator start_op = operators.begin();
        // Stack<Token*>::Iterator finish_op = operators.end();
        // while (start_op != finish_op) { cout << (*start_op)->value(); start_op++;}
        // cout << endl << endl;

    }
    while (! operators.empty()) { output.push(operators.pop()); }
    // Queue<Token*>::Iterator start = output.begin();
    // Queue<Token*>::Iterator finish = output.end();
    // while (start != finish) { cout << (*start)->value(); start++;}
    return select(fields, output);
}

Table Table::select_all() // get the whole table
{
    return *this;
}

vector<long> Table::select_recnos()
{
    return last_selected_recnos;
}

ostream& operator<<(ostream& outs, const Table& print_me)
{
    outs << "Table name: " << print_me.name << ", records: " << print_me.num_vals << endl;
    outs << setw(25) << "record";
    for (int i = 0; i < print_me.index_to_column.size(); ++i)
    {
        outs << setw(25) << print_me.index_to_column.at(i);
    }
    outs << endl;
    for (int i = 0; i < print_me.data_list.size(); ++i)
    {
        vectorstr item = print_me.data_list.at(i);
        outs << setw(25) << i;
        for (int j = 0; j < item.size(); ++j)
        {
            outs << setw(25) << item.at(j);
        }
        outs << endl;
    }

    return outs;
}