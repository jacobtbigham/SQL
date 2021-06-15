#ifndef TABLE_H
#define TABLE_H

#include "../bplustree/map.h"
#include "../bplustree/multimap.h"
#include "typedefs.h"
#include "../stack/MyStack.h"
#include "../queue/MyQueue.h"
#include "../token/child_tokens.h"
#include "../binary_files/file_record.h"
#include "../binary_files/utilities.h"

#include <string>
#include <iostream>

using namespace std;


class Table
{
    public:
        // CTORS
        Table();
        Table(string _name);
        Table(string _name, vectorstr fields);

        // MODIFIERS
        void insert_into(vectorstr row); // write function

        // ACCESSORS
        Table select(vectorstr fields, string field, string op, string operand);
        Table select(vectorstr fields, vectorstr conditions);
        Table select(vectorstr fields, Queue<Token*> args); // queue already in postfix
        Table select_all(); // get the whole table
        vector<long> select_recnos();

        friend ostream& operator<<(ostream& outs, const Table& print_me);

    private:
        vector< MMap<string, int> > data;
        vector<vectorstr> data_list;
        Map<string, int> column_to_index;
        vectorstr index_to_column;
        string name;
        int num_vals;
        static int select_number;
        vector<long> last_selected_recnos;
        void insert_no_write(vectorstr row, int _recno);
};


#endif