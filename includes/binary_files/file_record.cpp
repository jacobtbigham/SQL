#include "./file_record.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

long FileRecord::num_records = -1;

int FileRecord::write(fstream& outs)
{
    outs.seekg(0, outs.end);
    for (int i = 0; i < MAX; ++i)
    {
        outs.write(_record[i], sizeof(_record[i]));
    }
    return recno;
}

long FileRecord::read(fstream& ins, long _recno)
{
    long pos = _recno * MAX * MAX;
    ins.seekg(0, ins.end);
    long size = ins.tellg();
    if (pos >= size) { return 0; } // reached the end of the file
    ins.seekg(pos, ios_base::beg);
    int items_in_record = 0;
    for (int i = 0; i < MAX; ++i)
    {
        string next_in = "";
        for (int j = 0; j < MAX; ++j)
        {
            char next;
            ins.get(next);
            next_in += next;
        }
        if (next_in.at(0) == '\0')
        {
            break;
        }
        info.push_back(next_in);
        strncpy(_record[i], next_in.c_str(), MAX);
        ++items_in_record;
    }
    return MAX * items_in_record;
}


ostream& operator<<(ostream& outs, const FileRecord& print_me)
{
    for (int i = 0; i < print_me.MAX; ++i)
    {
        if (print_me._record[i][0] == '\0') { break; }
        for (int j = 0; j < print_me.MAX; j++)
        {
            outs << print_me._record[i][j];
        }
    }
    return outs;
}